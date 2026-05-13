#ifndef SD_READER
#define SD_READER

#include <Arduino.h>
#include <SdFat.h>
#include "definitions.h"
#include "utils/TimeUtils.h"
#include "Logger.h"

class SdReader {
    private:
        static const char _logDir[16];
        static char _path[64];
        static char _lockedPath[64];
        static bool _isLocked;
        static File _file;
        static SdFat _sd;
        static char _fileTime[64];
        static StatusFlags* _statusFlags;

        static constexpr uint32_t LOW_SPACE_THRESHOLD_BYTES = 10UL * 1024UL * 1024UL;
        static constexpr char LOCK_SUFFIX[] = "LOCK.log";

    public:
        /**
         * @brief Initializes SD card module over SPI interface and creates log file
         * 
         * @param csPin CS pin of SD card module
         * @return true if initialization succeeds, otherwise false
         */
        static bool init(uint8_t csPin, StatusFlags& statusFlags);
        
        /**
         * @brief Closes currently used file and creates new one
         */
        static bool switchFile();

        /**
         * @brief Renames current file by adding "LOCK" suffix to a file name
         * 
         */
        static bool lockFile();

        /**
         * @brief Writes data to a SD card
         *
         * @param data Data to be written
         * @return number of bytes written, 0 on failure
         */
        static size_t writeData(const char* data);

        /**
         * @brief Frees space by deleting the oldest non-locked log file
         *        if free space on the SD card is below 10 MB
         *
         * @return true if a file was deleted, false otherwise
         */
        static bool cleanupIfLowSpace();
};

#endif