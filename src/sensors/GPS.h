#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include "definitions.h"
#include "Logger.h"
#include "utils/GenericUtils.h"

// UBX command: set baud rate to 115200
const uint8_t setBaud115200[] = {
  0xB5, 0x62,
  0x06, 0x00,
  0x14, 0x00,

  0x01,
  0x00,
  0x00, 0x00,

  0xD0, 0x08, 0x00, 0x00,

  0x00, 0xC2, 0x01, 0x00,

  0x07, 0x00,
  0x03, 0x00,

  0x00, 0x00,
  0x00, 0x00,

  0xC0, 0x7E
};

// Disable GLL
const uint8_t disableGLL[] = {
  0xB5,0x62,0x06,0x01,0x08,0x00,
  0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,
  0x01,0x2B
};

// Disable GSA
const uint8_t disableGSA[] = {
  0xB5,0x62,0x06,0x01,0x08,0x00,
  0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,
  0x02,0x32
};

// Disable GSV
const uint8_t disableGSV[] = {
  0xB5,0x62,0x06,0x01,0x08,0x00,
  0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,
  0x03,0x39
};

// Disable VTG
const uint8_t disableVTG[] = {
  0xB5,0x62,0x06,0x01,0x08,0x00,
  0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,
  0x06,0x48
};


class GPS {
    private:
        TinyGPSPlus _gps;
        StatusFlags& _statusFlags;

    public:
        GPS(StatusFlags& statusFlags) : _statusFlags(statusFlags) {}

        /**
         * @brief Configures GPS baud rate and disables unused NMEA sentences
         *
         * @return true if initialization succeeds
         */
        bool init();

        /**
         * @brief Reads and feeds bytes from the GPS serial port to the parser
         */
        void readData();

        /**
         * @brief Returns the latest GPS sample
         *
         * @return GPSSample with current position, speed, and time
         */
        GPSSample getSample();

        /**
         * @brief Returns true if the GPS fix is valid
         *
         * @return true if GPS data is valid
         */
        bool isValid();

        /**
         * @brief Returns true if new GPS data was received since the last check
         *
         * @return true if data was updated
         */
        bool hasUpdatedData();

        /**
         * @brief Logs the current GPS sample to the SD card
         */
        void logSample();
};


#endif