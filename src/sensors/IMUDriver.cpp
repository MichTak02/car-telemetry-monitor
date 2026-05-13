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

    if (!_magnetometer.init()) {
        Logger::log(LOG_ERROR, SENSOR_MAGNETOMETER, "Could not initialize magnetometer");
        return false;
    }

    _mpu.setHighPassFilter(MPU6050_HIGHPASS_DISABLE);
    _mpu.setMotionInterrupt(false);
    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);

    _statusFlags.imu = true;
    return true;
}

void IMUDriver::readData(FloatTuple3& accelData, FloatTuple3& gyroData, FloatTuple3& magData, float& temp)
{
    /* Get new sensor events with the readings */
    sensors_event_t a, g, t;
    _mpu.getEvent(&a, &g, &t);
    _magnetometer.readData();

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

    // microtesla
    magData = _magnetometer.getValue();

    temp = t.temperature;
}

void IMUDriver::readData()
{
    FloatTuple3 accel;
    FloatTuple3 gyro;
    FloatTuple3 mag;
    float temp;

    readData(accel, gyro, mag, temp);
    _timestamp = TimeUtils::getPreciseTime();

    _accelerometer.setValues(accel);
    _gyroscope.setValues(gyro);
}

IMUSample IMUDriver::getIMUSample(Unit accelUnit, Unit gyroUnit, Unit magUnit, bool calibrated)
{
    PreciseDateTime time = _timestamp;
    return {
        _accelerometer.getValues(accelUnit, calibrated),
        _gyroscope.getValues(gyroUnit, calibrated),
        _magnetometer.getValue(magUnit, calibrated),
        true,
        time
    };
}

IMUSample IMUDriver::getIMUSample(bool calibrated)
{
    return getIMUSample(UNIT_MS2, UNIT_RAD_S, UNIT_MICROTESLA, calibrated);
}

void IMUDriver::logData()
{
    Logger::logIMUSample(getIMUSample());
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
