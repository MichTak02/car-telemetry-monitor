#include "VibrationScreen.h"

void VibrationScreen::init()
{
    _nex.writeNum(VIBRATION_FIELD, 0);
}

void VibrationScreen::disableProcessing() {}

void VibrationScreen::update()
{
    uint32_t vibrationLevelConverted = _vibrationAnalyzer.getVibrationLevel() * DECIMAL_MULTIPLIER;
    _nex.writeNum(VIBRATION_FIELD, vibrationLevelConverted);
}
