#ifndef VIBRATION_SCREEN_H
#define VIBRATION_SCREEN_H

#include "GenericScreen.h"
#include "processing/VibrationMeter.h"

class VibrationScreen : public GenericScreen {
    private:
        VibrationMeter& _vibrationMeter;
        static constexpr char VIBRATION_FIELD[] = "vibTr.val";
        static constexpr uint16_t DECIMAL_MULTIPLIER = 100; // 2 decimal places
    public:
        VibrationScreen(EasyNex& nex, VibrationMeter& vibrationMeter) : GenericScreen(nex), _vibrationMeter(vibrationMeter)
        {
            updatePeriodTicks = 1;
        };
        /**
         * @brief Initializes vibration display field
         */
        void init() override;

        /**
         * @brief Sends vibration level to the display
         */
        void update() override;

        /**
         * @brief Disables vibration meter
         */
        void disableProcessing() override;
};

#endif