#ifndef DISPLAY_COMMUNICATION_H
#define DISPLAY_COMMUNICATION_H

#include <Arduino.h>
#include <EasyNextionLibrary.h>
#include "screens/GenericScreen.h"
#include "screens/SpeedScreen.h"
#include "screens/AltitudeScreen.h"
#include "screens/VibrationScreen.h"
#include "screens/OrientationScreen.h"
#include "screens/IdleScreen.h"
#include "screens/StatusScreen.h"
#include "screens/CalibrationScreen.h"
#include "screens/BluetoothScreen.h"

#include "Bluetooth.h"
#include "processing/SpeedGetter.h"
#include "processing/VibrationMeter.h"
#include "processing/AltitudeFusion.h"
#include "processing/MotionFusion.h"
#include "DisplayTypes.h"

class DisplayCommunication {
    private:
        EasyNex& _nex;

        SpeedGetter& _speedGetter;
        AccelerationMagnitude& _accelMagnitude;
        VibrationMeter& _vibrationMeter;
        AltitudeFusion& _altitudeFusion;
        MotionFusion& _orientationFusion;

        SpeedScreen _speedScreen;
        VibrationScreen _vibrationScreen;
        AltitudeScreen _altitudeScreen;
        OrientationScreen _orientationScreen;
        IdleScreen _idleScreen;
        StatusScreen _statusScreen;
        CalibrationScreen _calibrationScreen;
        BluetoothScreen _bluetoothScreen;
        GenericScreen* _currentScreen = nullptr;
        uint32_t _totalTicks = 0;

        void handlePageChange();
    public:
        DisplayCommunication(EasyNex& nex, SpeedGetter& speedGetter, AccelerationMagnitude& accelMagnitude, VibrationMeter& vibrationMeter, AltitudeFusion& altitudeFusion, MotionFusion& orientationFusion, StatusFlags& statusFlags, Bluetooth& bluetooth)
            : _nex(nex),
              _speedGetter(speedGetter),
              _accelMagnitude(accelMagnitude),
              _vibrationMeter(vibrationMeter),
              _altitudeFusion(altitudeFusion),
              _orientationFusion(orientationFusion),
              _speedScreen(_nex, _speedGetter, _accelMagnitude),
              _vibrationScreen(_nex, _vibrationMeter),
              _altitudeScreen(_nex, _altitudeFusion),
              _orientationScreen(_nex, _orientationFusion),
              _idleScreen(_nex),
              _statusScreen(_nex, statusFlags),
              _calibrationScreen(_nex),
              _bluetoothScreen(_nex, bluetooth) {
        };

        void update();
};

#endif