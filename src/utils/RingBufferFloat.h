#ifndef RING_BUFFER_FLOAT_H
#define RING_BUFFER_FLOAT_H

#include <Arduino.h>
#include "definitions.h"

class RingBufferFloat {
    private:
        float buffer[RING_BUFF_MAX_LEN] = {0};
        uint8_t index = 0;
        double sum = 0;
        uint16_t itemsCount = 0;

    public:
        void addValue(float value);
        float getAverage();
};

#endif