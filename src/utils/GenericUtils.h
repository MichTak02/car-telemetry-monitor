#ifndef GENERIC_UTILS_G
#define GENERIC_UTILS_G

#include <Arduino.h>
#include "definitions.h"
#include "Logger.h"


class GenericUtils {
    private:
    public:
        static char *FloatsToStr(float *values, size_t count, char delim, char *buff);
        static void handleInterrupt(volatile uint32_t *triggerCount, uint32_t maxTriggerCount);
};

#endif