#include "StatusScreen.h"

void StatusScreen::writeStatusFlags()
{
    _nex.writeNum(GPS_STATUS_FIELD, _statusFlags.gps ? 1 : 0);
    _nex.writeNum(IMU_STATUS_FIELD, _statusFlags.imu ? 1 : 0);
    _nex.writeNum(MAG_STATUS_FIELD, _statusFlags.magnetometer ? 1 : 0);
    _nex.writeNum(BARO_STATUS_FIELD, _statusFlags.barometer ? 1 : 0);
    _nex.writeNum(SD_STATUS_FIELD, _statusFlags.sdCard ? 1 : 0);
    _nex.writeNum(BT_STATUS_FIELD, _statusFlags.bluetooth ? 1 : 0);
}

void StatusScreen::disableProcessing() {}

void StatusScreen::init()
{
    writeStatusFlags();
    _nex.writeStr(ERROR_MSG_FIELD, Logger::getLastErrorMsg());
}

void StatusScreen::update()
{
    writeStatusFlags();

    if (Logger::hasUpdatedErrorMsg()) {
        _nex.writeStr(ERROR_MSG_FIELD, Logger::getLastErrorMsg());
    }
}
