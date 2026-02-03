#include "Logger.h"
#include "utils/TimeUtils.h"
#include "utils/GenericUtils.h"

#define FLUSH_TIME_MS 1000

String Logger::_path;
File Logger::_file;
SdFat Logger::_sd;

bool Logger::init(uint8_t csPin, const String& folder)
{
    pinMode(csPin, OUTPUT);

    char time[64] = {0};
    TimeUtils::getTimeStrDashed(time);
    _path = folder + "/" + time + ".log";

    if (!_sd.begin(csPin, SD_SCK_MHZ(4))) {
        return false;
    }

    // TODO co když se nepodaří najít / vytvořit soubor
    // TODO vytvoření souboru do funkce (použití při vytváření dalších souborů)
    _file = _sd.open(_path, O_WRITE | O_CREAT);
    
    // Prevent SD card from corrupting
    _file.preAllocate(4 * 1024 * 1024); // Preallocate 4 MB
    _file.seekSet(0);
    _file.sync();

    return true;
}

void Logger::log(PreciseDateTime time, LogLevel logLevel, SensorType sensorType, const char *msg)
{
    static uint32_t lastMillis = millis();

    char buff[256] = {0};
    char timeStr[64] = {0};
    TimeUtils::convertPreciseTimeStr(time, timeStr);
    
    int writtenBytes = snprintf(buff, sizeof(buff), "%s,%d,%d,%s\n", timeStr, logLevel, sensorType, msg);
    
    _file.write(buff);
    
    // Log if message was truncated
    if (writtenBytes >= sizeof(buff)) {
        log(LOG_WARN, "Previous log was truncated");
    }

    // TODO dát do funkce, která bude volána periodicky pomocí timeru
    if (false || millis() - lastMillis > 1000) {
        _file.truncate();
        _file.sync();
        lastMillis = millis();
        Serial.print(buff);
    }
}


void Logger::log(LogLevel logLevel, const char *msg)
{
    static uint32_t lastFlush = millis();
    char buff[256] = {0};
    char time[64] = {0};
    PreciseDateTime preciseTime = TimeUtils::getPreciseTime();

    log(preciseTime, logLevel, NO_SENSOR, msg);
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
        sample.gyro.z,
        sample.mag.x,
        sample.mag.y,
        sample.mag.z
    };

    char delim = ',';
    
    GenericUtils::FloatsToStr(values, 9, delim, msg);
    log(sample.timestamp, LOG_DATA, IMU, msg);
}

void Logger::logPressure(FloatSample pressureSample)
{
    char msg[16] = {0};
    GenericUtils::FloatsToStr(&pressureSample.value, 1, '_', msg);

    log(pressureSample.timestamp, LOG_DATA, BAROMETER, msg);
}
