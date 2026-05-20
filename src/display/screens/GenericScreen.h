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

        /**
         * @brief Initializes screen components on the Nextion display
         */
        virtual void init() = 0;

        /**
         * @brief Sends fresh data to the Nextion display
         */
        virtual void update() = 0;

        /**
         * @brief Stops active processing when this screen is not visible
         */
        virtual void disableProcessing() = 0;
};

#endif