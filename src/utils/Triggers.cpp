#include <Arduino.h>
#include "definitions.h"

// Time setting trigger
void trigger0() {
  eventFlags.timeChanged = true;
}

// Settings trigger
void trigger1() {
  eventFlags.settingsChanged = true;
}

// Gyroscope calibration trigger
void trigger2() {
  eventFlags.calibrationRequest = true;
}

// Factory reset trigger
void trigger3() {
  NVIC_SystemReset();
}