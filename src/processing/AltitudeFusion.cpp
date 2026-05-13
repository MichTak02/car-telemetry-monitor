#include "AltitudeFusion.h"


float AltitudeFusion::pressureToAltitude(float pressureHpa)
{
    // Barometric formula to convert pressure to altitude (https://ncar.github.io/aircraft_ProcessingAlgorithms/3-the-state-of-the-aircraft.html#palt)
    return 44330.77 * (1.0 - powf(pressureHpa / SEA_LEVEL_PRESSURE, 0.1902632f));
}

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

    FloatSample barometerPressure = _barometer.getSample(UNIT_HPA);
    float barometerAltitude = pressureToAltitude(barometerPressure.value);

    if (!_gps.isValid()) {
        // If GPS data is not valid, use only barometer
        _altitude = barometerAltitude;
        return;
    }

    GPSSample gpsSample = _gps.getSample();
    
    // Use complementary filter to fuse GPS and barometer altitude
    float gpsAltitude = gpsSample.altitude;
    _altitude = _alpha * gpsAltitude + (1 - _alpha) * barometerAltitude;
}

float AltitudeFusion::getAltitude()
{
    return _altitude;
}