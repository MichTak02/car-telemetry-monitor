#include "AltitudeFusion.h"

void AltitudeFusion::enable()
{
    _altitude = 0.0f;
    _enabled = true;
}

void AltitudeFusion::disable()
{
    _enabled = false;
}

void AltitudeFusion::update()
{
    if (!_enabled) {
        return;
    }
    FloatSample barometerAltitude = _barometer.getSample();

    if (!_gps.isValid()) {
        // If GPS data is not valid, use only barometer
        _altitude = barometerAltitude.value;
        return;
    }

    GPSSample gpsSample = _gps.getSample();

    // Use complementary filter to fuse GPS and barometer altitude
    float gpsAltitude = gpsSample.altitude;
    _altitude = _alpha * gpsAltitude + (1 - _alpha) * barometerAltitude.value;
}

float AltitudeFusion::getAltitude()
{
    return _altitude;
}
