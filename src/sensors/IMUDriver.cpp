#include "IMUDriver.h"
#include "Logger.h"
#include "Settings.h"

#define USE_FULL_ASSERT

bool IMUDriver::init()
{
    _statusFlags.imu = false;

    if (!_mpu.begin(I2C_ADDRESS)) {
        Logger::log(LOG_ERROR, SENSOR_IMU, "Could not initialize MPU6050");
        return false;
    }

    _mpu.setHighPassFilter(MPU6050_HIGHPASS_DISABLE);
    _mpu.setMotionInterrupt(false);
    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);

    _statusFlags.imu = true;
    return true;
}

void IMUDriver::readData(FloatTuple3& accelData, FloatTuple3& gyroData, float& temp)
{
    sensors_event_t a, g, t;
    _mpu.getEvent(&a, &g, &t);

    // m/s^2
    accelData = {
        a.acceleration.x,
        a.acceleration.y,
        a.acceleration.z
    };

    // rad/s
    gyroData = {
        g.gyro.x,
        g.gyro.y,
        g.gyro.z
    };

    temp = t.temperature;
}

void IMUDriver::readData()
{
    FloatTuple3 accelData;
    FloatTuple3 gyroData;
    float temp;

    sensors_event_t a, g, t;
    _mpu.getEvent(&a, &g, &t);

    // m/s^2
    accelData = {
        a.acceleration.x,
        a.acceleration.y,
        a.acceleration.z
    };

    // rad/s
    gyroData = {
        g.gyro.x,
        g.gyro.y,
        g.gyro.z
    };

    temp = t.temperature;

    _timestamp = TimeUtils::getPreciseTime();

    _accelerometer.setValues(accelData);
    _gyroscope.setValues(gyroData);
}

IMUSample IMUDriver::getIMUSample(Unit accelUnit, Unit gyroUnit, bool calibrated)
{
    PreciseDateTime time = _timestamp;
    return {
        _accelerometer.getValues(accelUnit, calibrated),
        _gyroscope.getValues(gyroUnit, calibrated),
        time
    };
}

IMUSample IMUDriver::getIMUSample(bool calibrated)
{
    return getIMUSample(UNIT_MS2, UNIT_RAD_S, calibrated);
}

void IMUDriver::logData()
{
    char msg[128] = {0};
    IMUSample sample = getIMUSample();
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
    Logger::log(sample.timestamp, LOG_DATA, SENSOR_IMU, msg);
}

void IMUDriver::calibrate()
{
    FloatTuple3 avg = {0.0f, 0.0f, 0.0f};
    uint32_t n = 0;
    uint32_t startMs = millis();

    while (millis() - startMs < CALIBRATION_DURATION_MS) {
        sensors_event_t a, g, t;
        _mpu.getEvent(&a, &g, &t);
        n++;
        avg.x += (g.gyro.x - avg.x) / (float) n;
        avg.y += (g.gyro.y - avg.y) / (float) n;
        avg.z += (g.gyro.z - avg.z) / (float) n;
        delay(CALIBRATION_SAMPLE_INTERVAL_MS);
    }

    Settings::setGyroShift(avg.x, avg.y, avg.z);
}
