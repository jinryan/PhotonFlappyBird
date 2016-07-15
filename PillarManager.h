/******************************************************************************
PillarManager.h
Particle Photon Flappy Bird Project
Ryan Jin @ iD Tech Electric Engineering with the Arduino Platform
Original Creation Date: July 9
Original Completion Date: July 13
*****************************************************************************/

#ifndef PILLARMANAGER_H
#define PILLARMANAGER_H

#include "pillar.h"

class PillarManager {

    public:
        PillarManager(int screenWidth, int screenHeight); // Constructor method
        Pillar **timeToMove(); // Call this every 20 mil sec
        void reset(); // Restart the game

        // Getter methods
        float getDelay();
        int getCurrentAmountOfPillarsOnScreen();
        int getAmountOfPillarsUserPassed();

    private:

        // ==================== Constants ======================
        static const int _MAX_AMOUNT_OF_PILLARS_ON_SCREEN = 3;
        const int _BIRD_SPACE = Pillar::BIRD_SPACE;
        const int _DEFAULT_AMOUNT_OF_PILLARS_ON_SCREEN = 0;
        const int _DEFAULT_DELAY_OF_PILLARS = 30;
        const int _PILLARS_PASSED_TO_LEVEL_UP_FACTOR = 2;
        const int _PILLARS_PASSED_TO_LEVEL_UP_STARTING_POINT = 6;
        const int _MIN_PILLAR_BETWEEN_PILLAR_SPACE = 15;
        const int _MIN_HEIGHT_OF_PILLARS = 4;
        const float _MIN_DELAY_OF_PILLARS = 12.0;
        const float _ACCELERATION_RATE = 0.005;


        // =================== Variables ======================
        int _screenWidth;
        int _screenHeight;
        int _amountOfPillarsOnScreen;
        int _pillarsPassedToLevelUp;
        unsigned int _amountOfPillarsUserPassed;
        float _currentPillarDelay;
        Pillar *_pillars[_MAX_AMOUNT_OF_PILLARS_ON_SCREEN]; // All the pillars



        // ================ Private Methods ===================
        /******************************************************
         * Steps required during each refresh
         * 1. Check to see if one pillar is off the screen or not, and if so add new pillar
         * 2. Update the amount of pillars the user passed
         * 3. Update the delay of the pillars
         * 4. Update the position of the pillar

        ******************************************************/

        void _timeToMove(); // Does the step mentioned above

        // 1. Add pillars methods
        void _addPillarsIfNeeded();
        void _recycleFirstPillar();
        void _appendExtraPillar();
        bool _lastPillarIsFarEnoughToAddNew();

        // Pillar construction methods
        int _generateRandomHeight();
        Pillar *_newPillarWithHeight(int height);

        // 2. Update amount of pillars user passed
        void _oneMorePillarPassed();
        bool _pillarPassed();

        // 3. Update delay of the pillars
        void _updateDelaySpeedOfPillars();

        // 4. Update the position of the pillars
        void _move1Px();
};


#endif
