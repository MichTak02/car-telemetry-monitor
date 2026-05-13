#ifndef IDLE_SCREEN_H
#define IDLE_SCREEN_H

#include "GenericScreen.h"

class IdleScreen : public GenericScreen {
    public:
        IdleScreen(EasyNex& nex) : GenericScreen(nex) {};
        void init() override {}
        void update() override {}
        void disableProcessing() override {}
};

#endif