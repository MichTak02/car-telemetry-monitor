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

#include "processing/SpeedGetter.h"
#include "processing/VibrationAnalyzer.h"
#include "processing/AltitudeFusion.h"
#include "processing/MotionFusion.h"
#include "DisplayTypes.h"

class DisplayCommunication {
    private:
        EasyNex& _nex;

        SpeedGetter& _speedGetter;
        AccelerationMagnitude& _accelMagnitude;
        VibrationAnalyzer& _vibrationAnalyzer;
        AltitudeFusion& _altitudeFusion;
        MotionFusion& _orientationFusion;

        SpeedScreen _speedScreen;
        VibrationScreen _vibrationScreen;
        AltitudeScreen _altitudeScreen;
        OrientationScreen _orientationScreen;
        IdleScreen _idleScreen;
        StatusScreen _statusScreen;
        CalibrationScreen _calibrationScreen;
        GenericScreen* _currentScreen = nullptr;
        uint32_t _totalTicks = 0;

        void handlePageChange();
    public:
        DisplayCommunication(EasyNex& nex, SpeedGetter& speedGetter, AccelerationMagnitude& accelMagnitude, VibrationAnalyzer& vibrationAnalyzer, AltitudeFusion& altitudeFusion, MotionFusion& orientationFusion, StatusFlags& statusFlags)
            : _nex(nex),
              _speedGetter(speedGetter),
              _accelMagnitude(accelMagnitude),
              _vibrationAnalyzer(vibrationAnalyzer),
              _altitudeFusion(altitudeFusion),
              _orientationFusion(orientationFusion),
              _speedScreen(_nex, _speedGetter, _accelMagnitude),
              _vibrationScreen(_nex, _vibrationAnalyzer),
              _altitudeScreen(_nex, _altitudeFusion),
              _orientationScreen(_nex, _orientationFusion),
              _idleScreen(_nex),
              _statusScreen(_nex, statusFlags),
              _calibrationScreen(_nex) {
        };

        void update();
};

#endif