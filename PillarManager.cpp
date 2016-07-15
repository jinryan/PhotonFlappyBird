/******************************************************************************
PillarManager.cpp
Particle Photon Flappy Bird Project
Ryan Jin @ iD Tech Electric Engineering with the Arduino Platform
Original Creation Date: July 9
Original Completion Date: July 13
*****************************************************************************/

#include "PillarManager.h"
#include "Arduino/Arduino.h"
#include <math.h>

// ================== Public Methods ==============================

PillarManager::PillarManager(int screenWidth, int screenHeight) {
    /*****************************************
     * Initialization Plan:
     * 1. Fill in screen width and screen height variable for future uses
     * 2. Random height and create a pillar
     * 3. Initialize the following variables
     * * * _amountOfPillarsOnScreen, _amountOfPillarsUserPassed, _pillarsPassedToLevelUp,
     * * * and _currentPillarSpeed
    *****************************************/


    // 1. Fill in screen width and screen height variable for future uses
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;

    // 2. Random height and create a pillar
    int heightForTopPillar = _generateRandomHeight();
    _pillars[0] = _newPillarWithHeight(heightForTopPillar);
    // 3. Initialize properties
    _amountOfPillarsOnScreen = 1;
    _amountOfPillarsUserPassed = 0;
    _currentPillarDelay = _DEFAULT_DELAY_OF_PILLARS;
    _pillarsPassedToLevelUp = _PILLARS_PASSED_TO_LEVEL_UP_STARTING_POINT;
}

Pillar **PillarManager::timeToMove() {
    _timeToMove(); // Call private timeToMove function P.S. I couldn't think of a better name
    return _pillars;
}

void PillarManager::reset() {
    // Delete all the pillars
    for (int i = 0; i < _MAX_AMOUNT_OF_PILLARS_ON_SCREEN; i++) {
        delete _pillars[i];
    }
    // Reset variables
    // 1. Random height and create a pillar
    int heightForTopPillar = _generateRandomHeight();
    _pillars[0] = _newPillarWithHeight(heightForTopPillar);
    // 2. Reset the properties
    _amountOfPillarsOnScreen = 1;
    _amountOfPillarsUserPassed = 0;
    _currentPillarDelay = _DEFAULT_DELAY_OF_PILLARS;
    _pillarsPassedToLevelUp = _PILLARS_PASSED_TO_LEVEL_UP_STARTING_POINT;
    _amountOfPillarsUserPassed = 0;
}

// ============================ Getter methods =============================

float PillarManager::getDelay() {
    return _currentPillarDelay;
}

int PillarManager::getCurrentAmountOfPillarsOnScreen() {
    return _amountOfPillarsOnScreen;
}

int PillarManager::getAmountOfPillarsUserPassed() {
    return _amountOfPillarsUserPassed;
}

// ================== Private Methods ============================

void PillarManager::_timeToMove() {
    _addPillarsIfNeeded();
    // Record user's achievement
    if (_pillarPassed()) {
        _oneMorePillarPassed();
    }
    _updateDelaySpeedOfPillars();
    _move1Px(); // Move the pillars. The timeToMove public method will then return the updated version of _pillars, the version that had been updated by the methods above.
}

// =================== Add Pillars Methods =======================

void PillarManager::_addPillarsIfNeeded() {
    /******************************
     * Determination Plan
     * 1. If a pillar is off the screen, recycle and append
     * 2. If the user meets the _pillarsPassedToLevelUp, then add one
    *******************************/

    static bool isGoneButHasntReachedYet;

    // 1. If a pillar is off the screen, recycle and append

    // If the first pillar is gone, but when before the last pillar isn't far enough to start a new one but now it is far enough, then add a new pillar
    if (isGoneButHasntReachedYet &&  _lastPillarIsFarEnoughToAddNew()) {
        _appendExtraPillar();
        isGoneButHasntReachedYet = false;
    }
    if (_pillars[0]->isGone() &&  _lastPillarIsFarEnoughToAddNew()) {
        // If the first pillar is gone and there is enough space for another pillar, then recycle the first pillar and add another pillar
        _recycleFirstPillar();
        _appendExtraPillar();
    } else if (_pillars[0]->isGone() && !_lastPillarIsFarEnoughToAddNew()) {
        // The first pillar is gone, but the last pillar on the screen isn't far away to start a new pillar
        // So I recorded this in a variable isGoneButHasntReachedYet
        _recycleFirstPillar();
        isGoneButHasntReachedYet = true;
    }

    // 2. If the user passes the amount of pillars required to pass to level up, the max amount of pillars on screen hasn't reached yet, and there is space for another one, then add an extra pillar on screen
    if (_amountOfPillarsUserPassed > _pillarsPassedToLevelUp && _amountOfPillarsOnScreen <= _MAX_AMOUNT_OF_PILLARS_ON_SCREEN && _lastPillarIsFarEnoughToAddNew()) {
        _appendExtraPillar();
        _pillarsPassedToLevelUp *= _PILLARS_PASSED_TO_LEVEL_UP_FACTOR; // Set the benchmark for next level (one more extra pillar)
    }
}

void PillarManager::_recycleFirstPillar() {
    // Make the item in an array to be equal to the item after it
    delete _pillars[0]; // Delete first pillar
    for (int i = 0; i < _amountOfPillarsOnScreen - 1; i++) {
        _pillars[i] = _pillars[i+1];
    }
    // Make sure the last item is deleted
    _pillars[_amountOfPillarsOnScreen - 1] = NULL; // Set the last existing pillar's pointer to NULL so we don't get two pointers pointing at the same thing.

    // Update the amount of pillars on screen
    _amountOfPillarsOnScreen--; // One pillar is taken away
}

void PillarManager::_appendExtraPillar() {

    // Add the item that is after the previous existing item with a pillar
    _pillars[_amountOfPillarsOnScreen] = _newPillarWithHeight(_generateRandomHeight());
    _amountOfPillarsOnScreen++;
}

bool PillarManager::_lastPillarIsFarEnoughToAddNew() {
    // Create new 2D array for data input
    // Input the data
    // _amountOfPillarsOnScreen-1 for the last pillar on screen
    int **lastPillarInformation = _pillars[_amountOfPillarsOnScreen-1]->returnPillarInformation();

    // Determine if it passes the point or not
    int lastPillarX = lastPillarInformation[0][0]; // X point
    int lastPillarWidth = lastPillarInformation[0][2]; // Width
    int lastPillarXEnd = lastPillarX + lastPillarWidth; // The end of that pillar
    int maxSpaceItCanBe = _screenWidth - _MIN_PILLAR_BETWEEN_PILLAR_SPACE; // What is the benchmark

    // Delete the information since it was created by the new keyword which was used in the returnPillarInformation method
    for (int i = 0; i < 2; i++) {
        delete lastPillarInformation[i];
    }
    delete[] lastPillarInformation;
    return (lastPillarXEnd < maxSpaceItCanBe); // Return if the last pillar passes the benchmark
}


// ========================= Pillar Construction Methods =======================

int PillarManager::_generateRandomHeight() {
    // Random's rnage is from the min height of the pillars to the max height, which is the entire screen height minus the min space required for the bottom pillar minus the space needed for the bird
    return random(_MIN_HEIGHT_OF_PILLARS, _screenHeight - _MIN_HEIGHT_OF_PILLARS - Pillar::BIRD_SPACE);
}

Pillar *PillarManager::_newPillarWithHeight(int height) {
    Pillar *nPillar = new Pillar(_screenWidth, _screenHeight, height);
    return nPillar;
}

void PillarManager::_oneMorePillarPassed() {
    _amountOfPillarsUserPassed++;
}

bool PillarManager::_pillarPassed() {
    bool pillarPassed = false;

    for (int i = 0; i < _amountOfPillarsOnScreen; i++) {

        // Initialize this 2D array
        int **pillarInfo = _pillars[i]->returnPillarInformation();
        // Get the point in which the pillar ends at
        int xPosition = pillarInfo[0][0] + pillarInfo[0][2];

        if (xPosition == _screenWidth / 2) {
            // If the end of the pillar is smaller than the screen width, it means that it has passed it
            pillarPassed = true;

        }

        // Delete pillarInfo since it was created with a new keyword being used in returnPillarInformation method
        for(int j = 0 ; j < 2 ; j++) {
            delete pillarInfo[j];
        }
        delete[] pillarInfo;
    }
    return pillarPassed;
}

// Update Delay of the Pillars

void PillarManager::_updateDelaySpeedOfPillars() {

    if (_currentPillarDelay > _MIN_DELAY_OF_PILLARS) {
        _currentPillarDelay -= _ACCELERATION_RATE;

    }
}

// Update the position of the pillars

void PillarManager::_move1Px() {
    for (int i = 0; i < _amountOfPillarsOnScreen; i++) { // Loop through all pillars
        _pillars[i]->changePillars(1); // Shift them to the left by 1 px. Sry about the horrible name
    }
}
