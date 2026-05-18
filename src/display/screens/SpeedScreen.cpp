#include "SpeedScreen.h"

void SpeedScreen::init()
{
    _speedGetter.enable();
    _accelMagnitude.enable();
    lastSentSpeed = -1;
}

void SpeedScreen::disableProcessing()
{
    _speedGetter.disable();
    _accelMagnitude.disable();
}

void SpeedScreen::update()
{
    uint16_t speedConverted = (uint16_t) _speedGetter.getSpeed();
    uint16_t accelConverted = (uint16_t) (_accelMagnitude.getAcceleration() * DECIMAL_MULTIPLIER);

    _nex.writeNum(SPEED_FIELD, speedConverted);
    _nex.writeNum(ACCEL_FIELD, accelConverted);
}
