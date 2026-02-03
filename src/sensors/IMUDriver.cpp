#include "IMUDriver.h"
#include "Logger.h"

#define USE_FULL_ASSERT

bool IMUDriver::init()
{
    if (!_mpu.begin(0x69)) {
        return false;
    }
        
    // TODO zjistit co dělá a přenastavit (zkopírováno z dřívějšího testování)
    // setupt motion detection
    _mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
    _mpu.setMotionDetectionThreshold(1);
    _mpu.setMotionDetectionDuration(20);
    _mpu.setInterruptPinLatch(false);	// Keep it latched.  Will turn off when reinitialized.
    _mpu.setInterruptPinPolarity(true);
    _mpu.setMotionInterrupt(false);
    
    return true;
}

bool IMUDriver::readData(FloatTuple3& accelData, FloatTuple3& gyroData, float& temp)
{
    /* Get new sensor events with the readings */
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

    // TODO co vrací status?
    return a.acceleration.status & g.acceleration.status;
}

bool IMUDriver::readData()
{
    FloatTuple3 accel;
    FloatTuple3 gyro;
    float temp;

    bool result = readData(accel, gyro, temp);
    _timestamp = TimeUtils::getPreciseTime();

    _accelerometer.setValues(accel);
    _gyroscope.setValues(gyro);
    return result;
}

IMUSample IMUDriver::getIMUSample(Unit accelUnit, Unit gyroUnit, Unit magUnit)
{
    PreciseDateTime time = _timestamp;
    return {
        // TODO revert
        _accelerometer.getValues(accelUnit),
        _gyroscope.getValues(gyroUnit),
        // TODO add magnetometer
        {0, 0, 0},
        false,
        time
    };
}

void IMUDriver::logData()
{
    Logger::logIMUSample(getIMUSample());
}