#ifndef GENERIC_SCREEN_H
#define GENERIC_SCREEN_H

#include <Arduino.h>
#include <EasyNextionLibrary.h>

class GenericScreen {
    protected:
        EasyNex& _nex;
    public:
        uint16_t updatePeriodTicks = 1;
        GenericScreen(EasyNex& nex) : _nex(nex) {};
        virtual void init() = 0;
        virtual void update() = 0;
        virtual void disableProcessing() = 0;
};

#endif