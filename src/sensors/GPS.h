#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include "definitions.h"
#include "Logger.h"
#include "utils/RingBuffer.h"
#include "utils/GenericUtils.h"


class GPS {
    private:
        static constexpr uint16_t _bufferSize = 512;
        static_assert(_bufferSize <= RING_BUFF_MAX_LEN, "GPS ring buffer size must be smaller than RING_BUFF_MAX_LEN");

        TinyGPSPlus _gps;
        RingBuffer<char> _buff = RingBuffer<char>(_bufferSize);
        StatusFlags& _statusFlags;

    public:
        GPS(StatusFlags& statusFlags) : _statusFlags(statusFlags) {}
        bool init();
        void readData();
        void parseData(uint16_t maxChars = 128);
        GPSSample getSample();
        bool isValid();
        bool hasUpdatedData();
        void logSample();
};


#endif