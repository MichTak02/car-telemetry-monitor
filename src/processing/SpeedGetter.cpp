#include "SpeedGetter.h"

void SpeedGetter::enable()
{
    _speed = -1.0f;
    _enabled = true;
}

void SpeedGetter::disable()
{
    _enabled = false;
}

void SpeedGetter::updateSpeed()
{
    if (!_enabled) {
        return;
    }
    if (_gps.isValid()) {
        _speed = _gps.getSample().speed;
    } else {
        _speed = -1.0f; // Indicate invalid speed
    }
}

float SpeedGetter::getSpeed()
{
    return _speed;
}
