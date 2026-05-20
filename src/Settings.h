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

        /**
         * @brief Returns the current settings
         *
         * @return current settings
         */
        static const SettingsData& getCurrent();

        /**
         * @brief Resets settings to default values
         */
        static void resetToDefault();

        /**
         * @brief Applies new settings
         *
         * @param settings settings to apply
         */
        static void set(const SettingsData& settings);

        /**
         * @brief Sets the time sync flag
         *
         * @param timeSync true to enable GPS time sync
         */
        static void setTimeSync(bool timeSync);

        /**
         * @brief Sets the time zone offset in minutes
         *
         * @param timeZone time zone offset in minutes
         */
        static void setTimeZone(int32_t timeZone);

        /**
         * @brief Sets the impact threshold and segment duration levels
         *
         * @param impactThresholdLevel impact sensitivity level
         * @param segmentDurationLevel segment duration level
         */
        static void setImpactAndSegment(ImpactThresholdLevel impactThresholdLevel, SegmentDurationLevel segmentDurationLevel);

        /**
         * @brief Sets gyroscope bias correction offsets
         *
         * @param x X-axis offset
         * @param y Y-axis offset
         * @param z Z-axis offset
         */
        static void setGyroShift(float x, float y, float z);

        /**
         * @brief Sets gyroscope bias correction offsets from fixed-point integers
         *
         * @param x X-axis offset
         * @param y Y-axis offset
         * @param z Z-axis offset
         * @param decimalPlaces number of decimal places
         */
        static void setGyroShift(int32_t x, int32_t y, int32_t z, uint8_t decimalPlaces);
};

#endif
