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

        static constexpr uint32_t LOW_SPACE_THRESHOLD_BYTES = 10UL * 1024UL * 1024UL; // 10 MB
        static constexpr uint32_t PREALLOC_SIZE = 2UL * 1024UL * 1024UL; // 2 MB
        static constexpr uint32_t SYNC_INTERVAL_MS = 500;
        static constexpr uint32_t LOCK_DELAY_MS = 500;
        static constexpr char LOCK_SUFFIX[] = "LOCK.log";

        static uint32_t _lastSyncTime;
        static uint32_t _lockRequestTime;

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
         */
        static bool lockFile();

        /**
         * @brief Schedules a deferred lock after LOCK_DELAY_MS; call on impact detection
         */
        static void scheduleLock();

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

        /**
         * @brief Write log filenames to out, one per line, ending with "END\n"
         * Skips the active log and, when lockedOnly is true, skips non-locked files
         */
        static void sendFileNamesToSerial(Stream& out, bool lockedOnly);

        /**
         * @brief Sends the content of logs/<filename> to out followed by "END\n"
         * Returns false if the file does not exist or is the active log
         */
        static bool sendFile(Stream& out, const char* filename);

        /**
         * @brief Returns true if filename matches the currently-open log file
         */
        static bool isActiveLogPath(const char* filename);

        /**
         * @brief Returns the file size of logs/<filename> in bytes, or 0 if missing
         */
        static uint32_t getFileSize(const char* filename);
};

#endif