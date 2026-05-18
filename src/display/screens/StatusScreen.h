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
        void init() override;
        void update() override;
        void disableProcessing() override;
};


#endif