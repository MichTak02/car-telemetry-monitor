#ifndef GENERIC_UTILS_G
#define GENERIC_UTILS_G

#include <Arduino.h>
#include "definitions.h"
#include "Logger.h"


class GenericUtils {
    private:
    public:
        /**
         * @brief Converts multiple floats to string and devides them by @p delim 
         * 
         * @param values array of float values
         * @param count number of floats in array
         * @param delim delimiter (one character)
         * @param buff char buffer to store string to
         * @return @p buff
         */
        static char *floatsToStr(float *values, size_t count, char delim, char *buff, uint8_t decimalPlaces = 2, bool addDelimiterToEnd = false);

        /**
         * @brief Convert float to fixed-point integer with given decimal places
         *
         * @param value float value to convert
         * @param decimalPlaces number of digits after decimal point
         * @return fixed-point integer representation
         */
        static int32_t floatToFixedPoint(float value, uint8_t decimalPlaces);

        /**
         * @brief Convert fixed-point integer to float with given decimal places
         *
         * @param value fixed-point integer value
         * @param decimalPlaces number of digits after decimal point
         * @return float representation
         */
        static float fixedPointToFloat(int32_t value, uint8_t decimalPlaces);
        
        /**
         * @brief Handle timer interrupt
         * 
         * @param triggerCount pointer to trigger counter
         * @param maxTriggerCount maximum number of unprocessed triggers
         */
        static void handleInterrupt(volatile uint16_t *triggerCount, uint32_t maxTriggerCount);

        /**
         * @brief Update interrupt pending triggers from a global tick counter
         * 
         * @param interrupt Interrupt struct to update
         * @param now Current global tick value
         */
        static void updateInterrupt(InterruptStruct& interrupt, uint32_t currentTickMs);

        /**
         * @brief Get the duration of a segment in milliseconds based on its level
         *
         * @param segmentDurationLevel The level of the segment duration
         * @return duration in milliseconds
         */
        static uint32_t getSegmentDurationMs(SegmentDurationLevel segmentDurationLevel);
};

#endif