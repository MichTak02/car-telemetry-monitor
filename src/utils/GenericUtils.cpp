#include "GenericUtils.h"

char *GenericUtils::floatsToStr(float *values, size_t count, char delim, char *buff, uint8_t decimalPlaces)
{
    char *ptr = buff;
    for (int i = 0; i < count; i++) {
        dtostrf(values[i], 0, decimalPlaces, ptr);
        ptr += strlen(ptr);
        
        // Don't add delimiter to last element
        if (i + 1 != count) {
            ptr[0] = delim;
            ptr++;
        }
    }

    ptr[0] = '\0';

    return buff;
}

void GenericUtils::handleInterrupt(volatile uint16_t *pendingTriggers, uint32_t maxTriggers)
{    
    noInterrupts();
    uint16_t currPendingTriggers = *pendingTriggers;
    
    // Avoid interrupts stacking up in case of slow down
    *pendingTriggers = min((uint32_t) currPendingTriggers - 1, maxTriggers);
    interrupts();

    if (currPendingTriggers > maxTriggers) {
        char buff[64] = {0};
        snprintf(buff, sizeof(buff), "%s %d", "Too many waiting interrupts: ", currPendingTriggers);
        Logger::log(LOG_WARN, buff);
    }
}

void GenericUtils::updateInterrupt(InterruptStruct& interrupt, uint32_t currentTickMs)
{
    uint32_t elapsed = currentTickMs - interrupt.lastTick;
    if (elapsed < interrupt.periodTicks) {
        return;
    }

    uint32_t triggers = elapsed / interrupt.periodTicks;
    if (triggers == 0) {
        return;
    }

    interrupt.lastTick += triggers * interrupt.periodTicks;

    uint32_t newPending = interrupt.pendingTriggers + triggers;
    if (newPending > 0xFFFFU) {
        interrupt.pendingTriggers = 0xFFFFU;
    } else {
        interrupt.pendingTriggers = (uint16_t)newPending;
    }
}

uint32_t GenericUtils::getSegmentDurationMs(SegmentDurationLevel segmentDurationLevel)
{
    switch (segmentDurationLevel) {
        case SEGMENT_DURATION_SHORT:
            // 1 min
            return 60000;
        case SEGMENT_DURATION_MEDIUM:
            // 3 min
            return 180000;
        case SEGMENT_DURATION_LONG:
            // 5 min
            return 300000;
        default:
            Logger::log(LOG_ERROR, "Invalid segment duration level, defaulting to 3 min");
            return 180000;
    }
}

int32_t GenericUtils::floatToFixedPoint(float value, uint8_t decimalPlaces)
{
    int32_t factor = 1;
    for (uint8_t i = 0; i < decimalPlaces; i++) {
        factor *= 10;
    }

    return (int32_t)(value * factor);
}

float GenericUtils::fixedPointToFloat(int32_t value, uint8_t decimalPlaces)
{
    int32_t factor = 1;
    for (uint8_t i = 0; i < decimalPlaces; i++) {
        factor *= 10;
    }

    return value / (float)factor;
}
