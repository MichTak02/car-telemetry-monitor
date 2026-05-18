#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <HardwareTimer.h>
#include "definitions.h"

class Bluetooth {
private:
    static constexpr uint32_t _baud = 115200;

    HardwareTimer* _timer;
    StatusFlags* _statusFlags;
    char _lineBuf[80];
    uint8_t _lineLen;
    bool _enabled;
    TransferInfo _transferInfo;

    void _processLine(const char* line);
    void _sendErr(const char* code);
    void _sendFileContent(const char* filename);

public:
    Bluetooth(StatusFlags& flags);

    /**
     * @brief Begin the serial and store the timer for pause/resume
     */
    void init(HardwareTimer* timer);

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

    /**
     * @brief Starts the transfer of the pending file by setting its status to TRANSFER_IN_PROGRESS
     */
    void startTransfer();
};

#endif
