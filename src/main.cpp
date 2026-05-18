#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <EasyNextionLibrary.h>

#include "utils/TimeUtils.h"
#include "utils/GenericUtils.h"
#include "utils/NextionUtils.h"
#include "Logger.h"
#include "sensors/Accelerometer.h"
#include "sensors/Barometer.h"
#include "sensors/IMUDriver.h"
#include "sensors/GPS.h"
#include "SdReader.h"
#include "processing/MotionFusion.h"
#include "processing/VibrationMeter.h"
#include "processing/SpeedGetter.h"
#include "processing/AltitudeFusion.h"
#include "processing/AccelerationMagnitude.h"

#include "display/DisplayCommunication.h"
#include "definitions.h"
#include "Settings.h"
#include "Bluetooth.h"

#define CS_PIN PA4
#define NEXTION_RX_PIN PB11
#define NEXTION_TX_PIN PB10
#define NEXTION_BAUD 115200

void handleEvents();

// Nextion display
HardwareSerial NextionSerial(NEXTION_RX_PIN, NEXTION_TX_PIN);
EasyNex nex = EasyNex(NextionSerial);

// Timers
HardwareTimer *timer = nullptr;
volatile uint32_t globalTickMs = 0;

// Interrupts
InterruptStruct baroInterrupt = InterruptStruct(500);
InterruptStruct IMUInterrupt = InterruptStruct(50);
InterruptStruct splitInterrupt = InterruptStruct(100000);

InterruptStruct fusionInterrupt = InterruptStruct(50);
InterruptStruct accelInterrupt = InterruptStruct(50);
InterruptStruct vibrationInterrupt = InterruptStruct(50);
InterruptStruct speedInterrupt = InterruptStruct(500);

InterruptStruct displayInterrupt = InterruptStruct(100);

InterruptStruct* interruptList[] = {
  &baroInterrupt,
  &IMUInterrupt,
  &splitInterrupt,
  &fusionInterrupt,
  &accelInterrupt,
  &vibrationInterrupt,
  &displayInterrupt,
  &speedInterrupt
};
const uint8_t INTERRUPT_COUNT = sizeof(interruptList) / sizeof(interruptList[0]);

StatusFlags statusFlags = {};
EventFlags eventFlags = {};

Barometer barometer = Barometer(statusFlags);
IMUDriver imuDriver = IMUDriver(statusFlags);
GPS gps = GPS(statusFlags);

MotionFusion motionFusion = MotionFusion(imuDriver);
VibrationMeter vibrationMeter = VibrationMeter(imuDriver, Settings::getCurrent().impactThresholdLevel);
SpeedGetter speedGetter = SpeedGetter(gps);

AltitudeFusion altitudeFusion = AltitudeFusion(gps, barometer);
AccelerationMagnitude accelMagnitude = AccelerationMagnitude(imuDriver);

Bluetooth bluetooth(statusFlags);

DisplayCommunication displayCommunication(nex, speedGetter, accelMagnitude, vibrationMeter, altitudeFusion, motionFusion, statusFlags, bluetooth);


// TODO smazat
void listDir(const char* path);

void onTimer() {
  globalTickMs++;
}


void setup() {
  Serial.begin(115200);
  Wire.begin((uint32_t) PB7, (uint32_t) PB6);

  nex.begin(115200);

  Logger::init(statusFlags);
  NextionUtils::init(nex);
  TimeUtils::init();

  if (!SdReader::init(CS_PIN, statusFlags)) {
      Serial.println("Could not init Logger");
  }

  if (!imuDriver.init()) {
    Serial.println("Could not init IMU driver");
  }

  if (!barometer.init()) {
    Serial.println("Could not init Barometer");
  }

  gps.init();

  timer = new HardwareTimer(TIM3);
  timer->setOverflow(1000, MICROSEC_FORMAT);
  timer->attachInterrupt(onTimer);
  timer->resume();

  bluetooth.init(timer);

  motionFusion.enable();

  eventFlags.loadCalibrationRequest = true;
  eventFlags.settingsChanged = true;
  eventFlags.timeChanged = true;
}

void loop() {
  uint32_t now = globalTickMs;
  for (uint8_t i = 0; i < INTERRUPT_COUNT; i++) {
    GenericUtils::updateInterrupt(*interruptList[i], now);
  }

  // Timer was triggered
  if (IMUInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&IMUInterrupt.pendingTriggers, IMUDriver::MAX_INTERRUPTS);
    imuDriver.readData();
    imuDriver.logData();
  }

  if (baroInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&baroInterrupt.pendingTriggers, Barometer::MAX_INTERRUPTS);
    barometer.readData();
    barometer.logSample();
    altitudeFusion.update();
  }

  if (splitInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&splitInterrupt.pendingTriggers, 1);
    SdReader::switchFile();
  }

  if (fusionInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&fusionInterrupt.pendingTriggers, 1);
    motionFusion.update();
  }

  if (accelInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&accelInterrupt.pendingTriggers, 1);
    accelMagnitude.update();
  }

  if (vibrationInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&vibrationInterrupt.pendingTriggers, 1);
    vibrationMeter.update();
    vibrationMeter.checkVibrationLevel();
  }

  if (speedInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&speedInterrupt.pendingTriggers, 1);
    speedGetter.updateSpeed();
  }


  gps.readData();
  bluetooth.update();


  if (gps.hasUpdatedData() && gps.isValid()) {
    TimeUtils::syncFromGPS(gps.getSample());
    gps.logSample();
  }

  if (displayInterrupt.pendingTriggers > 0) {
    GenericUtils::handleInterrupt(&displayInterrupt.pendingTriggers, 1);
    displayCommunication.update();
  }

  handleEvents();
}


void handleEvents() {
  if (eventFlags.loadCalibrationRequest) {
    FloatTuple3 gyroShift;
    if (NextionUtils::getCalibrationSettings(gyroShift)) {
      Settings::setGyroShift(gyroShift.x, gyroShift.y, gyroShift.z);
      Logger::log(LOG_INFO, "Calibration settings loaded from Nextion display");
    } else {
      Logger::log(LOG_ERROR, "Failed to get calibration settings from Nextion display");
    }

    eventFlags.loadCalibrationRequest = false;
  }

  if (eventFlags.settingsChanged) {
    ImpactThresholdLevel impact;
    SegmentDurationLevel segment;
    if (NextionUtils::getImpactAndSegment(impact, segment)) {
      Settings::setImpactAndSegment(impact, segment);
      vibrationMeter.setImpactThresholdLevel(impact);
      splitInterrupt.periodTicks = GenericUtils::getSegmentDurationMs(segment);
      Logger::log(LOG_INFO, "Settings updated from Nextion display");
    } else {
      Logger::log(LOG_ERROR, "Failed to get updated settings from Nextion display");
    }
    eventFlags.settingsChanged = false;
  }

  if (eventFlags.timeChanged) {
    DateTime updatedTime;
    bool timeSync;
    uint32_t timeZone;
    if (NextionUtils::getTimeSettings(updatedTime, timeSync, timeZone)) {
      TimeUtils::setTime(updatedTime);
      Settings::setTimeSync(timeSync);
      Settings::setTimeZone(timeZone);
      SdReader::switchFile();
      Logger::log(LOG_INFO, "Time updated from Nextion display");
    } else {
      Logger::log(LOG_ERROR, "Failed to get updated time from Nextion display");
    }
    eventFlags.timeChanged = false;
  }

  if (eventFlags.calibrationRequest) {
    Logger::log(LOG_INFO, "Gyro calibration starting");
    timer->pause();
    imuDriver.calibrate();
    timer->resume();
    Logger::log(LOG_INFO, "Gyro calibration done");
    eventFlags.calibrationRequest = false;
  }
}
