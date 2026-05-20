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
        static constexpr uint32_t ERROR_NUMBER = 777777;

    public:
        /**
         * @brief Initializes with a reference to the Nextion display
         *
         * @param nex Nextion display instance
         */
        static void init(EasyNex& nex);

        /**
         * @brief Reads impact threshold and segment duration settings from the display
         *
         * @param impactThresholdLevel output impact threshold level
         * @param segmentDurationLevel output segment duration level
         * @return true on success
         */
        static bool getImpactAndSegment(ImpactThresholdLevel& impactThresholdLevel, SegmentDurationLevel& segmentDurationLevel);

        /**
         * @brief Reads time, sync flag, and time zone from the display
         *
         * @param time output DateTime
         * @param timeSync output time sync flag
         * @param timeZone output time zone offset
         * @return true on success
         */
        static bool getTimeSettings(DateTime& time, bool& timeSync, int8_t& timeZone);

        /**
         * @brief Reads gyroscope calibration offsets from the display
         *
         * @param gyroShift output gyro shift tuple
         * @return true on success
         */
        static bool getCalibrationSettings(FloatTuple3& gyroShift);

        /**
         * @brief Writes the current time to the Nextion display
         *
         * @param time time to write
         * @return true on success
         */
        static bool setTime(const DateTime& time);
};


#endif