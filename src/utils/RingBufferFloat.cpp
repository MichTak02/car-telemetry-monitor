#include "RingBufferFloat.h"

void RingBufferFloat::addValue(float value)
{
    sum -= buffer[index];
    buffer[index] = value;
    sum += value;

    index = (index + 1) % RING_BUFF_MAX_LEN;
    if (itemsCount < RING_BUFF_MAX_LEN)
        itemsCount += 1;
}

float RingBufferFloat::getAverage()
{
    return sum / itemsCount;
}
