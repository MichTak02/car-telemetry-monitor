#include "BluetoothScreen.h"

void BluetoothScreen::init()
{
    _bluetooth.enable();
}

void BluetoothScreen::update()
{
    const TransferInfo& transferInfo = _bluetooth.getTransferInfo();
    if (transferInfo.status == PENDING_TRANSFER) {
        _nex.writeStr(FILE_NAME_FIELD, transferInfo.filename);
        _nex.writeNum(SIZE_FIELD, transferInfo.fileSize);
        _bluetooth.startTransfer();
    }

    _nex.writeNum(STATUS_FIELD, (int32_t) transferInfo.status);
}

void BluetoothScreen::disableProcessing()
{
    _bluetooth.disable();
}
