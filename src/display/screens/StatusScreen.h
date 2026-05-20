#ifndef STATUS_SCREEN_H
#define STATUS_SCREEN_H

#include "GenericScreen.h"
#include "definitions.h"
#include "Logger.h"

class StatusScreen : public GenericScreen {
    private:
        static constexpr char GPS_STATUS_FIELD[] = "gpsTr.val";
        static constexpr char IMU_STATUS_FIELD[] = "imuTr.val";
        static constexpr char BARO_STATUS_FIELD[] = "barTr.val";
        static constexpr char SD_STATUS_FIELD[] = "sdTr.val";
        static constexpr char ERROR_MSG_FIELD[] = "msgTr.txt";

        StatusFlags& _statusFlags;

        void writeStatusFlags();
    public:
        StatusScreen(EasyNex& nex, StatusFlags& flags) : GenericScreen(nex), _statusFlags(flags)
        {
            updatePeriodTicks = 20;
        };
        /**
         * @brief Initializes sensor status display fields
         */
        void init() override;

        /**
         * @brief Sends sensor status flags and error messages to the display
         */
        void update() override;

        /**
         * @brief No-op - status screen requires no active processing
         */
        void disableProcessing() override;
};


#endif