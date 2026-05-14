#include "SdReader.h"

const char SdReader::_logDir[] = "logs";
char SdReader::_path[] = {0};
File SdReader::_file;
SdFat SdReader::_sd;
char SdReader::_lockedPath[] = {0};
bool SdReader::_isLocked = false;
char SdReader::_fileTime[] = {0};
StatusFlags* SdReader::_statusFlags = nullptr;

bool SdReader::init(uint8_t csPin, StatusFlags& statusFlags)
{
    _statusFlags = &statusFlags;
    _statusFlags->sdCard = false;

    pinMode(csPin, OUTPUT);

    delay(100);

    if (!_sd.begin(csPin, SD_SCK_MHZ(1))) {
        _sd.printSdError(&Serial1);
        return false;
    }

    _statusFlags->sdCard = true;

    return switchFile();
}

bool SdReader::switchFile()
{
    cleanupIfLowSpace();
    
    Logger::log(LOG_INFO, "Switching to new file");
    _file.close();
    _isLocked = false;

    TimeUtils::getTimeStrDashed(_fileTime);

    snprintf(_path, sizeof(_path), "%s/%s.log", _logDir, _fileTime);
    snprintf(_lockedPath, sizeof(_lockedPath), "%s/%sLOCK.log", _logDir, _fileTime);

    _file = _sd.open(_path, O_WRITE | O_CREAT);

    if (!_file) {
        if (_statusFlags != nullptr) {
            _statusFlags->sdCard = false;
        }
        Logger::log(LOG_ERROR, "Could not create log file");
        return false;
    }

    // Prevent SD card from corrupting
    _file.sync();
    Logger::log(LOG_INFO, "Switched to new file");
    return true;
}

bool SdReader::lockFile()
{
    // Do nothing if already locked
    if (_isLocked) {
        Logger::log(LOG_INFO, "File already locked, skipping lock");
        return true;
    }

    _file.close();

    if (!_sd.rename(_path, _lockedPath)) {
        if (_statusFlags != nullptr) {
            _statusFlags->sdCard = false;
        }
        Logger::log(LOG_ERROR, "Could not rename log file");
        return false;
    }

    _file = _sd.open(_lockedPath, O_WRITE | O_APPEND);
    if (!_file) {
        if (_statusFlags != nullptr) {
            _statusFlags->sdCard = false;
        }
        Logger::log(LOG_ERROR, "Could not reopen locked log file");
        return false;
    }
    Logger::log(LOG_INFO, "File locked");
    
    _isLocked = true;
    return true;

}

size_t SdReader::writeData(const char* data)
{
    size_t bytesWritten = _file.write(data);
    if (bytesWritten == 0) {
        Logger::log(LOG_ERROR, "Failed to write to log file");
        if (_statusFlags != nullptr) {
            _statusFlags->sdCard = false;
        }
        return 0;
    }

    return bytesWritten;
}

bool SdReader::cleanupIfLowSpace()
{
    if (_statusFlags == nullptr || !_statusFlags->sdCard) {
        return false;
    }

    uint64_t freeBytes = (uint64_t) _sd.vol()->freeClusterCount()
                       * (uint64_t) _sd.vol()->bytesPerCluster();

    if (freeBytes >= LOW_SPACE_THRESHOLD_BYTES) {
        return false;
    }

    Logger::log(LOG_INFO, "SD low on space, deleting oldest log");

    File dir = _sd.open(_logDir, O_RDONLY);
    if (!dir || !dir.isDir()) {
        Logger::log(LOG_ERROR, "Could not open log directory");
        return false;
    }

    // Extract current file name from path
    const char* currentName = strrchr(_path, '/');
    currentName = currentName ? currentName + 1 : _path;

    char oldestName[32] = {0};
    char entryName[32];
    File entry;

    // Find first file that is not locked and not the currently opened file
    while (entry.openNext(&dir, O_RDONLY)) {
        if (entry.isDir()) {
            entry.close();
            continue;
        }

        size_t len = entry.getName(entryName, sizeof(entryName));
        entry.close();

        if (len == 0) {
            continue;
        }

        // Skip locked files
        size_t suffixLen = sizeof(LOCK_SUFFIX) - 1;
        if (len >= suffixLen && strcmp(entryName + len - suffixLen, LOCK_SUFFIX) == 0) {
            continue;
        }

        // Skip the currently open log file
        if (strcmp(entryName, currentName) == 0) {
            continue;
        }

        strncpy(oldestName, entryName, sizeof(oldestName) - 1);
        oldestName[sizeof(oldestName) - 1] = '\0';
        break;
    }
    dir.close();

    if (oldestName[0] == '\0') {
        Logger::log(LOG_INFO, "No deletable log file found");
        return false;
    }

    char fullPath[64];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", _logDir, oldestName);

    if (!_sd.remove(fullPath)) {
        Logger::log(LOG_ERROR, "Failed to delete old log file");
        return false;
    }

    Logger::log(LOG_INFO, "Deleted old log file");
    return true;
}

bool SdReader::isActiveLogPath(const char* filename)
{
    const char* activePath = _isLocked ? _lockedPath : _path;
    const char* activeName = strrchr(activePath, '/');
    activeName = activeName ? activeName + 1 : activePath;
    return strcmp(filename, activeName) == 0;
}

void SdReader::sendFileNamesToSerial(Stream& out, bool lockedOnly)
{
    _file.sync();

    File dir = _sd.open(_logDir, O_RDONLY);
    if (!dir || !dir.isDir()) {
        out.print("END\n");
        return;
    }

    char entryName[64];
    File entry;

    while (entry.openNext(&dir, O_RDONLY)) {
        if (entry.isDir()) {
            entry.close();
            continue;
        }

        size_t len = entry.getName(entryName, sizeof(entryName));
        entry.close();

        if (len == 0 || isActiveLogPath(entryName)) {
            continue;
        }

        if (lockedOnly) {
            size_t suffixLen = sizeof(LOCK_SUFFIX) - 1;
            if (len < suffixLen || strcmp(entryName + len - suffixLen, LOCK_SUFFIX) != 0) {
                continue;
            }
        }

        out.print(entryName);
        out.print('\n');
    }

    dir.close();
    out.print("END\n");
}

bool SdReader::sendFile(Stream& out, const char* filename)
{
    if (isActiveLogPath(filename)) {
        return false;
    }

    char fullPath[80];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", _logDir, filename);

    File file = _sd.open(fullPath, O_READ);
    if (!file) {
        return false;
    }

    uint8_t buf[64];
    int n;
    while ((n = file.read(buf, sizeof(buf))) > 0) {
        out.write(buf, (size_t)n);
    }

    file.close();
    out.print("\nEND\n");
    return true;
}

uint32_t SdReader::getFileSize(const char* filename)
{
    char fullPath[80];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", _logDir, filename);

    File file = _sd.open(fullPath, O_READ);
    if (!file) {
        return 0;
    }
    uint32_t size = file.fileSize();
    file.close();
    return size;
}
