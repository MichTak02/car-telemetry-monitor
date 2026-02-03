#include "GenericUtils.h"

char *GenericUtils::FloatsToStr(float *values, size_t count, char delim, char *buff)
{
    char *ptr = buff;
    for (int i = 0; i < count; i++) {
        dtostrf(values[i], 0, 2, ptr);
        ptr += strlen(ptr);
        
        // Don't add delimiter to last element
        if (i + 1 != count) {
            ptr[0] = delim;
            ptr++;
        }
    }

    return buff;
}

void GenericUtils::handleInterrupt(volatile uint32_t *triggerCount, uint32_t maxTriggerCount)
{    
    // Avoid interrupts stacking up in case of slow down
    noInterrupts();
    uint32_t currTriggerCount = *triggerCount;
    *triggerCount = min(currTriggerCount - 1, maxTriggerCount);
    interrupts();

    if (currTriggerCount > maxTriggerCount) {
        char buff[64] = {0};
        snprintf(buff, sizeof(buff), "%s %d", "Many interrupts waiting to be processed: ", currTriggerCount);
        Logger::log(LOG_WARN, buff);
    }
}
