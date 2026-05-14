#ifndef BLUETOOTH_SCREEN_H
#define BLUETOOTH_SCREEN_H

#include "GenericScreen.h"
#include "Bluetooth.h"

class BluetoothScreen : public GenericScreen {
    private:
        static constexpr char FILE_NAME_FIELD[] = "fileTr.txt";
        static constexpr char SIZE_FIELD[] = "sizeTr.val";
        static constexpr char STATUS_FIELD[] = "statusTr.val"; 
        Bluetooth& _bluetooth;
        bool _lastTransferInProgress = false;
    public:
        BluetoothScreen(EasyNex& nex, Bluetooth& bluetooth) : GenericScreen(nex), _bluetooth(bluetooth)
        {
            updatePeriodTicks = 10;
        };
        void init() override;
        void update() override;
        void disableProcessing() override;
};

#endif