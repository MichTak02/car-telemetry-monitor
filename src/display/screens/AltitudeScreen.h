#ifndef ALTITUDE_SCREEN_H
#define ALTITUDE_SCREEN_H

#include "GenericScreen.h"
#include "processing/AltitudeFusion.h"

class AltitudeScreen : public GenericScreen {
    private:
        AltitudeFusion& altitudeFusion;
        static constexpr char ALTITUDE_FIELD[] = "altTr.val";
        static constexpr char MIN_FIELD[] = "min.val";
        static constexpr char MAX_FIELD[] = "max.val";
        static constexpr char FIRST_VAL_FIELD[] = "firstTr.val";

        static constexpr uint16_t DECIMAL_MULTIPLIER = 1; // 0 decimal places
        int32_t getConvertedAltitude();
    public:
        AltitudeScreen(EasyNex& nex, AltitudeFusion& fusion) : GenericScreen(nex), altitudeFusion(fusion)
        {
            updatePeriodTicks = 10;
        };
        /**
         * @brief Initializes altitude, min, and max display fields
         */
        void init() override;

        /**
         * @brief Sends fused altitude data to the display
         */
        void update() override;

        /**
         * @brief Disables altitude fusion
         */
        void disableProcessing() override;
};

#endif