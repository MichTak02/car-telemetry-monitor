#ifndef CALIBRATION_SCREEN_H
#define CALIBRATION_SCREEN_H
#include "GenericScreen.h"
#include "definitions.h"
#include "Settings.h"

class CalibrationScreen : public GenericScreen {
    private:
        static constexpr char CALIBRATING_VAR[] = "calibrating.val";
        static constexpr char CALIBRATED_VAR[] = "calTr.val";
        static constexpr char GYRO_X_VAR[] = "gyroXTr.val";
        static constexpr char GYRO_Y_VAR[] = "gyroYTr.val";
        static constexpr char GYRO_Z_VAR[] = "gyroZTr.val";
        static constexpr uint16_t DECIMAL_MULTIPLIER = 100; // 2 decimal places
    public:
        CalibrationScreen(EasyNex& nex) : GenericScreen(nex)
        {
            updatePeriodTicks = 5;
        };
        void init() override;
        void update() override;
        void disableProcessing() override;
};

#endif