#include "SpeedScreen.h"

void SpeedScreen::init()
{
    _speedGetter.enable();
    _accelMagnitude.enable();
    _nex.writeNum(SPEED_FIELD, -1); // Initialize the speed display to 0
    _nex.writeNum(ACCEL_FIELD, 0); // Initialize the acceleration display to 0
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

    if (speedConverted != lastSentSpeed) {
        _nex.writeNum(SPEED_FIELD, speedConverted);
    }
    
    _nex.writeNum(ACCEL_FIELD, accelConverted);
    lastSentSpeed = speedConverted;
}
