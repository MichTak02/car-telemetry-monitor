#include "DisplayCommunication.h"

void DisplayCommunication::update()
{
    _nex.NextionListen();
    _totalTicks++;

    // Update the current screen if needed
    uint16_t periodTicks = _currentScreen->updatePeriodTicks;
    if (_currentScreen != nullptr && periodTicks != 0) {

        if ((_totalTicks % periodTicks) == 0) {
            _currentScreen->update();
        }
    }

    Serial1.print("curr page id: ");
    Serial1.println(_nex.currentPageId);


    if ((_nex.lastCurrentPageId == _nex.currentPageId) && (_currentScreen != nullptr)) {
        return;
    }

    handlePageChange();
}

void DisplayCommunication::handlePageChange()
{
    _nex.lastCurrentPageId = _nex.currentPageId;

    if (_currentScreen != nullptr) {
        _currentScreen->disableProcessing();
    }

    switch (_nex.currentPageId)
    {
        case SPEED_SCREEN: {
            _speedScreen.init();
            _currentScreen = &_speedScreen;
            break;
        }

        case VIBRATION_SCREEN: { 
            _vibrationScreen.init();
            _currentScreen = &_vibrationScreen;
            break;
        }

        case ALTITUDE_SCREEN: {
            _altitudeScreen.init();
            _currentScreen = &_altitudeScreen;
            break;
        }

        case ORIENTATION_SCREEN: {
            _orientationScreen.init();
            _currentScreen = &_orientationScreen;
            break;
        }

        case STATUS_SCREEN: {
            _statusScreen.init();
            _currentScreen = &_statusScreen;
            break;
        }

        case BLUETOOTH_SCREEN: {
            _bluetoothScreen.init();
            _currentScreen = &_bluetoothScreen;
            break;
        }

        case CALIBRATION_SCREEN: {
            _calibrationScreen.init();
            _currentScreen = &_calibrationScreen;
            break;
        }
        default: {
            _currentScreen = &_idleScreen;
            break;
        }
    }
}