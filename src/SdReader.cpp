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

    _isLocked = true;
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
