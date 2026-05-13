#ifndef ORIENTATION_SCREEN_H
#define ORIENTATION_SCREEN_H

#include "GenericScreen.h"
#include "processing/MotionFusion.h"

class OrientationScreen : public GenericScreen {
    private:
        MotionFusion& _motionFusion;
        static constexpr char PITCH_FIELD[] = "pitchTr.val";
        static constexpr char ROLL_FIELD[] = "rollTr.val";

        static constexpr uint16_t DECIMAL_MULTIPLIER = 100; // 2 decimal places

    public:
        OrientationScreen(EasyNex& nex, MotionFusion& motionFusion) : GenericScreen(nex), _motionFusion(motionFusion)
        {
            updatePeriodTicks = 1;
        };
        void init() override;
        void update() override;
        void disableProcessing() override;
};

#endif