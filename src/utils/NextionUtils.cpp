#include "utils/NextionUtils.h"

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

	impactRaw = clampValue(impactRaw, (uint32_t)IMPACT_THRESHOLD_HIGH);
	segmentRaw = clampValue(segmentRaw, (uint32_t)SEGMENT_DURATION_LONG);

	impactThresholdLevel = (ImpactThresholdLevel)impactRaw;
	segmentDurationLevel = (SegmentDurationLevel)segmentRaw;

	return true;
}

bool NextionUtils::getTimeSettings(DateTime& time, bool& timeSync, uint32_t& timeZone)
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

	time = DateTime(
		(uint16_t)year,
		(uint8_t)month,
		(uint8_t)day,
		(uint8_t)hour,
		(uint8_t)minute,
		(uint8_t)second
	);

	timeSync = (_nex->readNumber(TIME_SYNC_FIELD) != 0);
	timeZone = _nex->readNumber(TIME_ZONE_FIELD);

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
