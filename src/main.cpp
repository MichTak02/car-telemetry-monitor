#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

#include "utils/TimeUtils.h"
#include "utils/GenericUtils.h"
#include "Logger.h"
#include "sensors/Accelerometer.h"
#include "sensors/Barometer.h"
#include "sensors/IMUDriver.h"
#include "sensors/GPS.h"

#define CS_PIN PA4
#define PATH "logs"

HardwareSerial Serial2(PA3, PA2);

// Timers
HardwareTimer *timer = nullptr;
volatile uint32_t readIMU = 0;
volatile bool stackingInterrupts = false;

Barometer barometer;
IMUDriver imuDriver;
GPS gps;

// TODO modify timer(s)
void onTimerIMU() {
  readIMU += 1;
}


void setup() {
    Serial.begin(115200);
    Wire.begin((uint32_t) PB7, (uint32_t) PB6);

    TimeUtils::init();
    
    if (!Logger::init(CS_PIN, PATH)) {
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
    timer->setOverflow(50000, MICROSEC_FORMAT);
    timer->attachInterrupt(onTimerIMU);
    timer->resume();
}

void loop() {
  // Timer was triggered
  if (readIMU > 0) {
    GenericUtils::handleInterrupt(&readIMU, IMUDriver::MAX_INTERRUPTS);
    imuDriver.readData();
    imuDriver.logData();

    // TODO buď jiná proměnná (místo readIMU) nebo přejmenovat a dát vše sem -> MAX_INTERRUPTS dát jinam než do IMUDriver
    barometer.readData();
    barometer.logData();
  }

  gps.readData();
  
  TimeUtils::resyncTime();
}
