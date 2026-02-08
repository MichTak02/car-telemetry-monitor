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
        static char *FloatsToStr(float *values, size_t count, char delim, char *buff, uint8_t decimalPlaces = 2);
        
        /**
         * @brief Handle timer interrupt
         * 
         * @param triggerCount pointer to trigger counter
         * @param maxTriggerCount maximum number of unprocessed triggers
         */
        static void handleInterrupt(volatile uint32_t *triggerCount, uint32_t maxTriggerCount);
};

#endif