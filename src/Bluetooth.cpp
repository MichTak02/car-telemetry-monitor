#include "Bluetooth.h"
#include "SdReader.h"

// HardwareSerial BTSerial(PA10, PA9);
#define BTSerial Serial1


Bluetooth::Bluetooth(StatusFlags& flags)
    : _timer(nullptr),
      _statusFlags(&flags),
      _lineLen(0),
      _enabled(false)
{
    _transferInfo.filename[0] = '\0';
    _transferInfo.fileSize = 0;
    _transferInfo.status = TRANSFER_IDLE;
}

void Bluetooth::init(HardwareTimer* timer)
{
    _timer = timer;
    BTSerial.begin(_baud);
    _statusFlags->bluetooth = true;
}

void Bluetooth::enable()
{
    _transferInfo.status = TRANSFER_IDLE;
    _lineLen = 0;
    _enabled = true;
}

void Bluetooth::disable()
{
    _enabled = false;
}

void Bluetooth::update()
{
    // Drain incoming bytes to prevent HC-06 buffer overflow even when disabled
    if (!_enabled) {
        while (BTSerial.available()) {
            BTSerial.read();
        }
        return;
    }

    if (_transferInfo.status == TRANSFER_IN_PROGRESS) {
        _sendFileContent(_transferInfo.filename);
    }

    while (BTSerial.available()) {
        char c = (char)BTSerial.read();

        // Prevent buffer overflow when message too long
        if (_lineLen >= sizeof(_lineBuf) - 1) {
            if (c == '\n') {
                _sendErr("OVERFLOW");
                _lineLen = 0;
            }
            continue;
        }

        if (c == '\r') {
            continue;
        }

        if (c == '\n') {
            _lineBuf[_lineLen] = '\0';
            if (_lineLen > 0) {
                _processLine(_lineBuf);
            }
            _lineLen = 0;
        } else {
            _lineBuf[_lineLen] = c;
            _lineLen += 1;
        }
    }
}

void Bluetooth::_processLine(const char* line)
{
    // LST: list all files
    if (strcmp(line, "LST") == 0) {
        _timer->pause();
        SdReader::sendFileNamesToSerial(BTSerial, false);
        _timer->resume();
    // LCK: list all locked files
    } else if (strcmp(line, "LCK") == 0) {
        _timer->pause();
        SdReader::sendFileNamesToSerial(BTSerial, true);
        _timer->resume();
    // GET: send file content
    } else if (strncmp(line, "GET ", 4) == 0 && line[4] != '\0') {
        const char* filename = line + 4;

        strncpy(_transferInfo.filename, filename, sizeof(_transferInfo.filename) - 1);
        _transferInfo.filename[sizeof(_transferInfo.filename) - 1] = '\0';
        _transferInfo.fileSize = SdReader::getFileSize(filename);
        _transferInfo.status = PENDING_TRANSFER;
    } else {
        _sendErr("CMD");
    }
}

void Bluetooth::_sendErr(const char* code)
{
    BTSerial.print("ERR ");
    BTSerial.print(code);
    BTSerial.print('\n');
}

void Bluetooth::_sendFileContent(const char *filename)
{
    _timer->pause();
    if (SdReader::sendFile(BTSerial, filename)) {
        _transferInfo.status = TRANSFER_DONE;
    } else {
        _transferInfo.status = TRANSFER_FAILED;
        _sendErr("NOFILE");
    }
    _timer->resume();
}

const TransferInfo& Bluetooth::getTransferInfo() const
{
    return _transferInfo;
}

void Bluetooth::startTransfer()
{
    _transferInfo.status = TRANSFER_IN_PROGRESS;
}
