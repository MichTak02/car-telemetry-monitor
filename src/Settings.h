#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "definitions.h"

struct SettingsData {
	ImpactThresholdLevel impactThresholdLevel;
	SegmentDurationLevel segmentDurationLevel;
	FloatTuple3 gyroShift;
	bool timeSync;
    int32_t timeZone;
};

class Settings {
	private:
		static SettingsData _currentSettings;

	public:
        static const SettingsData DEFAULT_SETTINGS;
		static const SettingsData& getCurrent();

		static void resetToDefault();
		static void set(const SettingsData& settings);
		static void setTimeSync(bool timeSync);
        static void setTimeZone(int32_t timeZone);
		static void setImpactAndSegment(ImpactThresholdLevel impactThresholdLevel, SegmentDurationLevel segmentDurationLevel);
		static void setGyroShift(float x, float y, float z);
		static void setGyroShift(int32_t x, int32_t y, int32_t z, uint8_t decimalPlaces);
};

#endif
