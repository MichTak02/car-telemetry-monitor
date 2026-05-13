#include "OrientationScreen.h"

void OrientationScreen::disableProcessing()
{
    _motionFusion.disable();
}

void OrientationScreen::init()
{
    _motionFusion.enable();
    _nex.writeNum(PITCH_FIELD, 0);
    _nex.writeNum(ROLL_FIELD, 0);
}

void OrientationScreen::update()
{
    FloatTuple3 orientation = _motionFusion.getOrientation();

    int32_t pitchConverted = (int32_t) (orientation.pitch * DECIMAL_MULTIPLIER);
    int32_t rollConverted = (int32_t) (orientation.roll * DECIMAL_MULTIPLIER);


    _nex.writeNum(PITCH_FIELD, pitchConverted);
    _nex.writeNum(ROLL_FIELD, rollConverted);
}
