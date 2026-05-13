#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <Arduino.h>
#include <definitions.h>

template<typename T>
class RingBuffer {
private:
    const uint16_t _size;                 
    T _buffer[RING_BUFF_MAX_LEN];
    uint16_t _writeIndex = 0;
    uint16_t _readIndex = 0;
    uint16_t _itemsCount = 0;

    void incrementIndex(uint16_t *index)
    {
        // Do not use modulo due to performance
        *index += 1;
        if (*index >= _size) {
            *index = 0;
        }
    }

public:
    explicit RingBuffer(uint16_t size) : _size(size)
    {
        // Size is expected to be <= RING_BUFF_MAX_LEN; no dynamic allocation.
    }

    bool addValue(const T *value)
    {
        _buffer[_writeIndex] = *value;

        if (_itemsCount < _size) {
            _itemsCount++;
        } else {
            // Buffer full - overwrite oldest element
            incrementIndex(&_readIndex);
        }

        incrementIndex(&_writeIndex);
        return true;
    }

    bool readValue(T *value)
    {
        if (_itemsCount == 0) {
            return false;
        }

        *value = _buffer[_readIndex];
        incrementIndex(&_readIndex);
        _itemsCount--;
        return true;
    }

    size_t unreadCount() const
    {
        return _itemsCount;
    }
};

#endif
