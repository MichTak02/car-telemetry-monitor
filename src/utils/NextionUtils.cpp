#include "NextionUtils.h"

EasyNex* NextionUtils::_nex = nullptr;

static uint32_t clampValue(uint32_t value, uint32_t maxValue)
{
	return (value > maxValue) ? maxValue : value;
}

void NextionUtils::init(EasyNex& nex)
{
	_nex = &nex;
}

bool NextionUtils::isReady()
{
	return _nex != nullptr;
}

bool NextionUtils::getImpactAndSegment(ImpactThresholdLevel& impactThresholdLevel, SegmentDurationLevel& segmentDurationLevel)
{
	if (!isReady()) {
		return false;
	}

	uint32_t impactRaw = _nex->readNumber(IMPACT_THRESHOLD_FIELD);
	uint32_t segmentRaw = _nex->readNumber(SEGMENT_DURATION_FIELD);

    if (impactRaw == ERROR_NUMBER || segmentRaw == ERROR_NUMBER) {
        return false;
    }

	impactRaw = clampValue((uint8_t) impactRaw, IMPACT_THRESHOLD_HIGH);
	segmentRaw = clampValue((uint8_t) segmentRaw, SEGMENT_DURATION_LONG);

	impactThresholdLevel = (ImpactThresholdLevel)impactRaw;
	segmentDurationLevel = (SegmentDurationLevel)segmentRaw;

	return true;
}

bool NextionUtils::getTimeSettings(DateTime& time, bool& timeSync, int8_t& timeZone)
{
	if (!isReady()) {
		return false;
	}

	uint32_t year = _nex->readNumber(RTC0_FIELD);
	uint32_t month = _nex->readNumber(RTC1_FIELD);
	uint32_t day = _nex->readNumber(RTC2_FIELD);
	uint32_t hour = _nex->readNumber(RTC3_FIELD);
	uint32_t minute = _nex->readNumber(RTC4_FIELD);
	uint32_t second = _nex->readNumber(RTC5_FIELD);

    uint32_t timeZoneRaw = _nex->readNumber(TIME_ZONE_FIELD);
    uint32_t timeSyncRaw = _nex->readNumber(TIME_SYNC_FIELD);

    if (year == ERROR_NUMBER || month == ERROR_NUMBER || day == ERROR_NUMBER ||
        hour == ERROR_NUMBER || minute == ERROR_NUMBER || second == ERROR_NUMBER ||
        timeZoneRaw == ERROR_NUMBER || timeSyncRaw == ERROR_NUMBER) {
        return false;
    }

	time = DateTime(
		(uint16_t)year,
		(uint8_t)month,
		(uint8_t)day,
		(uint8_t)hour,
		(uint8_t)minute,
		(uint8_t)second
	);

	timeSync = ((uint8_t) timeSyncRaw != 0);
	timeZone = (int8_t) timeZoneRaw;

	return true;
}

bool NextionUtils::getCalibrationSettings(FloatTuple3 &gyroShift)
{
    if (!isReady()) {
        return false;
    }

    int32_t gyroShiftX = _nex->readNumber(GYRO_X_FIELD);
    int32_t gyroShiftY = _nex->readNumber(GYRO_Y_FIELD);
    int32_t gyroShiftZ = _nex->readNumber(GYRO_Z_FIELD);

    gyroShift.x = GenericUtils::fixedPointToFloat(gyroShiftX, 2);
    gyroShift.y = GenericUtils::fixedPointToFloat(gyroShiftY, 2);
    gyroShift.z = GenericUtils::fixedPointToFloat(gyroShiftZ, 2);

    return true;
}

bool NextionUtils::setTime(const DateTime& time)
{
	if (!isReady()) {
		return false;
	}

	_nex->writeNum(RTC0_FIELD, (uint32_t)time.year());
	_nex->writeNum(RTC1_FIELD, (uint32_t)time.month());
	_nex->writeNum(RTC2_FIELD, (uint32_t)time.day());
	_nex->writeNum(RTC3_FIELD, (uint32_t)time.hour());
	_nex->writeNum(RTC4_FIELD, (uint32_t)time.minute());
	_nex->writeNum(RTC5_FIELD, (uint32_t)time.second());

	return true;
}
