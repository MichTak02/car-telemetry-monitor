#ifndef NEXTION_UTILS_H
#define NEXTION_UTILS_H

#include <EasyNextionLibrary.h>
#include <RTClib.h>
#include "utils/GenericUtils.h"
#include "definitions.h"

class NextionUtils {
		private:
		static EasyNex* _nex;
		static bool isReady();

		static constexpr char IMPACT_THRESHOLD_FIELD[] = "impactThresholdLevel";
		static constexpr char SEGMENT_DURATION_FIELD[] = "segmentDurationLevel";
		static constexpr char RTC0_FIELD[] = "rtc0";
		static constexpr char RTC1_FIELD[] = "rtc1";
		static constexpr char RTC2_FIELD[] = "rtc2";
		static constexpr char RTC3_FIELD[] = "rtc3";
		static constexpr char RTC4_FIELD[] = "rtc4";
		static constexpr char RTC5_FIELD[] = "rtc5";
		static constexpr char TIME_SYNC_FIELD[] = "timeSync";
		static constexpr char TIME_ZONE_FIELD[] = "timeZone";
        static constexpr char GYRO_X_FIELD[] = "gyroShiftX";
        static constexpr char GYRO_Y_FIELD[] = "gyroShiftY";
        static constexpr char GYRO_Z_FIELD[] = "gyroShiftZ";

    public:
		static void init(EasyNex& nex);
		static bool getImpactAndSegment(ImpactThresholdLevel& impactThresholdLevel, SegmentDurationLevel& segmentDurationLevel);
		static bool getTimeSettings(DateTime& time, bool& timeSync, uint32_t& timeZone);
        static bool getCalibrationSettings(FloatTuple3& gyroShift);
		static bool setTime(const DateTime& time);
};


#endif