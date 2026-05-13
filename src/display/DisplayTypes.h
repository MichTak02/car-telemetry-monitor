#ifndef DISPLAY_TYPES_H
#define DISPLAY_TYPES_H

#include <Arduino.h>

enum ScreenType {
    SPEED_SCREEN = 0,
    VIBRATION_SCREEN = 1,
    ALTITUDE_SCREEN = 2,
    ORIENTATION_SCREEN = 3,
    STATUS_SCREEN = 6,
    CALIBRATION_SCREEN = 10
};

#endif