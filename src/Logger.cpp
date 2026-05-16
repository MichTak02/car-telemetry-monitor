#include "Logger.h"
#include "utils/TimeUtils.h"
#include "utils/GenericUtils.h"

char Logger::lastErrorMsg[] = {0};
bool Logger::updatedLastErrorMsg = false;
StatusFlags* Logger::_statusFlags = nullptr;

void Logger::log(PreciseDateTime time, LogLevel logLevel, SensorType sensorType, const char *msg)
{
    static uint32_t lastMillis = millis();

    char buff[256] = {0};
    char timeStr[64] = {0};
    TimeUtils::convertPreciseTimeStr(time, timeStr);
    
    int writtenBytes = snprintf(buff, sizeof(buff), "%s|%d|%d|%s\n", timeStr, logLevel, sensorType, msg);
    
    if (logLevel == LOG_ERROR and sensorType != NO_SENSOR) {
        updatedLastErrorMsg = true;
        strncpy(lastErrorMsg, msg, sizeof(lastErrorMsg) - 1);
        lastErrorMsg[sizeof(lastErrorMsg) - 1] = '\0'; // Ensure null-termination
    }

    if (_statusFlags != nullptr && !_statusFlags->sdCard) {
        Serial1.println(buff);
        return;
    }

    SdReader::writeData(buff);
    
#ifdef DEBUG
    if (logLevel != LOG_DATA) {
        Serial1.write(buff);
    }
#endif

    // Log if message was truncated
    if (writtenBytes >= sizeof(buff)) {
        log(LOG_WARN, "Previous log was truncated");
    }
}

void Logger::log(LogLevel logLevel, SensorType sensorType, const char *msg)
{
    static uint32_t lastFlush = millis();
    char buff[256] = {0};
    char time[64] = {0};
    PreciseDateTime preciseTime = TimeUtils::getPreciseTime();

    log(preciseTime, logLevel, NO_SENSOR, msg);
}

void Logger::init(StatusFlags &statusFlags)
{
    _statusFlags = &statusFlags;
}

void Logger::log(LogLevel logLevel, const char *msg)
{
    log(logLevel, NO_SENSOR, msg);
}

void Logger::logIMUSample(IMUSample sample)
{
    char msg[128] = {0};
    float values[] = {
        sample.accel.x,
        sample.accel.y,
        sample.accel.z,
        sample.gyro.x,
        sample.gyro.y,
        sample.gyro.z
    };

    const char delim = ',';

    GenericUtils::floatsToStr(values, 6, delim, msg);
    log(sample.timestamp, LOG_DATA, SENSOR_IMU, msg);
}

const char* Logger::getLastErrorMsg()
{
    updatedLastErrorMsg = false;
    return lastErrorMsg;
}

bool Logger::hasUpdatedErrorMsg()
{
    return updatedLastErrorMsg;
}

void Logger::logPressure(FloatSample pressureSample)
{
    char msg[16] = {0};
    GenericUtils::floatsToStr(&pressureSample.value, 1, '_', msg);

    log(pressureSample.timestamp, LOG_DATA, SENSOR_BAROMETER, msg);
}
