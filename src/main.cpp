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
// TODO dát do definitions?
HardwareTimer *timer = nullptr;
volatile uint32_t readIMU = 0;
volatile bool stackingInterrupts = false;

// SD card reader
SdFat sd;
SdFile dir;
SdFile entry;

Barometer barometer;
IMUDriver imuDriver;
GPS gps;

// TODO smazat
void listDir(const char* path);

// TODO make timers
void onTimerIMU() {
  readIMU += 1;
}

void onTimerGPS() {

}

void onTimerBarometer() {

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

    listDir("/logs");

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

  // static uint32_t counter = 0;
  // char buff[128];
  // sprintf(buff, "This is message number %d", counter);
  //Logger::log(LOG_INFO, buff);
  
  TimeUtils::resyncTime();
}


// TODO smazat, z internetu na testování
void listDir(const char* path) {
  if (!dir.open(path)) {
    Serial.println("Failed to open directory");
    return;
  }

  Serial.print("Listing: ");
  Serial.println(path);

  while (entry.openNext(&dir, O_RDONLY)) {

    char name[128];
    entry.getName(name, sizeof(name));
    Serial.print(name);

    if (entry.isDir()) {
      Serial.println("  <DIR>");
    } else {
      Serial.print("  ");
      Serial.print(entry.fileSize());
      Serial.println(" bytes");
    }
    entry.close();
  }
  dir.close();
}
