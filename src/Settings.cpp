#include "Settings.h"
#include "utils/GenericUtils.h"

const SettingsData Settings::DEFAULT_SETTINGS = {
	IMPACT_THRESHOLD_MEDIUM,
	SEGMENT_DURATION_MEDIUM,
	{0.0f, 0.0f, 0.0f},
	true,
    0
};

SettingsData Settings::_currentSettings = Settings::DEFAULT_SETTINGS;

const SettingsData& Settings::getCurrent()
{
	return _currentSettings;
}

void Settings::resetToDefault()
{
	_currentSettings = DEFAULT_SETTINGS;
}

void Settings::set(const SettingsData& settings)
{
	_currentSettings = settings;
}

void Settings::setTimeSync(bool timeSync)
{
	_currentSettings.timeSync = timeSync;
}

void Settings::setTimeZone(int32_t timeZone)
{
	_currentSettings.timeZone = timeZone;
}

void Settings::setImpactAndSegment(ImpactThresholdLevel impactThresholdLevel, SegmentDurationLevel segmentDurationLevel)
{
	_currentSettings.impactThresholdLevel = impactThresholdLevel;
	_currentSettings.segmentDurationLevel = segmentDurationLevel;
}

void Settings::setGyroShift(float x, float y, float z)
{
	_currentSettings.gyroShift = {x, y, z};
}

void Settings::setGyroShift(int32_t x, int32_t y, int32_t z, uint8_t decimalPlaces)
{
	_currentSettings.gyroShift = {
		GenericUtils::fixedPointToFloat(x, decimalPlaces),
		GenericUtils::fixedPointToFloat(y, decimalPlaces),
		GenericUtils::fixedPointToFloat(z, decimalPlaces)
	};
}
