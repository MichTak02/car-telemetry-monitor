#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <HardwareTimer.h>
#include "definitions.h"

#define BLUETOOTH_RX_PIN PA10
#define BLUETOOTH_TX_PIN PA9
#define BLUETOOTH_BAUD 115200

class Bluetooth {
private:
    HardwareSerial& _serial;
    HardwareTimer* _timer;
    StatusFlags* _statusFlags;
    char _lineBuf[80];
    uint8_t _lineLen;
    bool _enabled;
    TransferInfo _transferInfo;

    void _processLine(const char* line);
    void _sendErr(const char* code);

public:
    /**
     * @brief Constructor with a HardwareSerial bound to the HC-06 pins and StatusFlags ref
     */
    Bluetooth(HardwareSerial& serial, StatusFlags& flags);

    /**
     * @brief Begin the serial at given baud and store the timer for pause/resume
     */
    void init(uint32_t baud, HardwareTimer* timer);

    /**
     * @brief Allow command processing. Resets the line buffer
     */
    void enable();

    /**
     * @brief Stop processing commands, update() will still drain incoming bytes
     */
    void disable();

    /**
     * @brief Reads incoming bytes and processes them if enabled
    */
    void update();

    /**
     * @brief Returns the latest GET transfer info (filename, size, status)
     */
    const TransferInfo& getTransferInfo() const;
};

#endif
