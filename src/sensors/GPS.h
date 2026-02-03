#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include "definitions.h"
#include "Logger.h"

class GPS {
    private:
        TinyGPSPlus _gps;
        uint32_t speed; // ?
        char buff[512];

    public:
        bool init();
        void readData();
        void parseData(char *data);
        bool isFixed();

};


#endif