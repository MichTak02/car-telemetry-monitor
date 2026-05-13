#include "CalibrationScreen.h"

void CalibrationScreen::disableProcessing() {}

void CalibrationScreen::init() {}

void CalibrationScreen::update()
{
    int32_t calibrating = _nex.readNumber(CALIBRATING_VAR);
    if (calibrating != 1) {
        return;
    }

    // Gyro calibration ended, update display with results
    if (!eventFlags.calibrationRequest) {
        SettingsData settings = Settings::getCurrent();
        _nex.writeNum(GYRO_X_VAR, (int32_t) (settings.gyroShift.x * DECIMAL_MULTIPLIER));
        _nex.writeNum(GYRO_Y_VAR, (int32_t) (settings.gyroShift.y * DECIMAL_MULTIPLIER));
        _nex.writeNum(GYRO_Z_VAR, (int32_t) (settings.gyroShift.z * DECIMAL_MULTIPLIER));

        _nex.writeNum(CALIBRATING_VAR, 0);
        _nex.writeNum(CALIBRATED_VAR, 1);
    }
}
