#include "AltitudeScreen.h"

int32_t AltitudeScreen::getConvertedAltitude()
{
    float altitude = altitudeFusion.getAltitude();
    int32_t altitudeConverted = (int32_t) (altitude * DECIMAL_MULTIPLIER);
    return altitudeConverted;
}

void AltitudeScreen::disableProcessing()
{
    altitudeFusion.disable();
}

void AltitudeScreen::init()
{
    altitudeFusion.enable();
    int32_t altitudeConverted = getConvertedAltitude();
    _nex.writeNum(ALTITUDE_FIELD, altitudeConverted);
    _nex.writeNum(MIN_FIELD, altitudeConverted);
    _nex.writeNum(MAX_FIELD, altitudeConverted);
    _nex.writeNum(FIRST_VAL_FIELD, altitudeConverted);

}

void AltitudeScreen::update()
{
    int32_t altitudeConverted = getConvertedAltitude();
    _nex.writeNum(ALTITUDE_FIELD, altitudeConverted);
}
