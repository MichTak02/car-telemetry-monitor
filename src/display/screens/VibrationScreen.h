#ifndef VIBRATION_SCREEN_H
#define VIBRATION_SCREEN_H

#include "GenericScreen.h"
#include "processing/VibrationAnalyzer.h"

class VibrationScreen : public GenericScreen {
    private:
        VibrationAnalyzer& _vibrationAnalyzer;
        static constexpr char VIBRATION_FIELD[] = "vibTr.val";
        static constexpr uint16_t DECIMAL_MULTIPLIER = 100; // 2 decimal places
    public:
        VibrationScreen(EasyNex& nex, VibrationAnalyzer& vibrationAnalyzer) : GenericScreen(nex), _vibrationAnalyzer(vibrationAnalyzer)
        {
            updatePeriodTicks = 1;
        };
        void init() override;
        void update() override;
        void disableProcessing() override;
};

#endif