/******************************************************************************
bird.cpp
Particle Photon Flappy Bird Project
Ryan Jin @ iD Tech Electric Engineering with the Arduino Platform
Original Creation Date: July 9
Original Completion Date: July 13
*****************************************************************************/

/*******************************
* * Animation concepts * *
* It is not easy to do an animation where you pass in the final position and the
* time of the animation and let the Photon do the work for yhou. Instead, you have
* to move the items pixel by pixel and do animations such as acceleration by
* changing the delay between each pixels.
*
* There are two types of forces or actions the bird can have: the gravity, and the
* flapping. And these two forces each have separate delays and accelerations.
*
* If the user continues allowing one force to happen, then the acceleration happens,
* which in this class, includes adding an acceleration rate, or actually taking away
* the delay, from the existing delay property. And if it changes and for instance, the
* user starts flapping, then the gravitational delay gets changed into the default one,
* and the flap delay starts accelerating.
******************************/
#include "bird.h"

// ================================ Public Methods ================================
Bird::Bird(int birdSize, int screenWidth, int screenHeight) {
    // Initialize properties
    _birdSize = birdSize;
    _screenHeight = screenHeight;
    _screenWidth = screenWidth;
    _birdPosition = _screenHeight / 3;

    // Set delay for gravity fall and flap rise for user input
    _currentDelay = 0;
    // These are named current, but they are not the current delay in the UI, it just
    // means if this property is called, what is its current value
    _currentGravitationalDelay = _DEFAULT_GRAVITATIONAL_DELAY;
    _currentFlapDelay = _DEFAULT_FLAP_DELAY;
}

bool Bird::birdCrashed(Pillar **pillars, int amountOfPillars) {
    // If the bottom of the circle or the top touches the top or bottom of the screen, the bird is crashed
    //
    // It first checks if it touches the pillars, and then checks if it touches the top and the bottom

    for (int i = 0; i < amountOfPillars; i++) {
        // Get pillars information
        Pillar *pillar = pillars[i];
        int **pillarInformation = pillar->returnPillarInformation();

        int xStarting;
        int xEnding;
        int yBottom;
        int yTop;
        xStarting = pillarInformation[0][0];
        xEnding = xStarting + pillarInformation[0][2];
        yBottom = pillarInformation[0][3];
        yTop = pillarInformation[1][1];


        int birdTop = _birdPosition + _birdSize - 4;
        int birdBottom = _birdPosition - _birdSize + 4;
        // -4 and + 4 because of the slow refreshing rate. It is proven that the crashing will be more accurate for some reasons
        int birdStarting = _screenWidth / 2 - _birdSize;
        int birdEnding = _screenWidth / 2 + _birdSize;

        if (birdTop <= yBottom || birdBottom >= yTop) {
            // If the bird's top is above the top pillar's bottom or if the bird's bottom is underneath the bottom pillar's top
            if (birdStarting <= xEnding && birdEnding >= xStarting) {
                // Part of bird is in the same x position as the pillars
                return true;
            }
        }

        // Delete since the returnPillarInformation methods creates a "new" object
        for (int x = 0; x < 2; x++) {
            delete pillarInformation[x];
        }
        delete[] pillarInformation;
    }
    int maxYPosition = _screenHeight - _birdSize;
    int minYPosition = _birdSize;
    return (_birdPosition >= maxYPosition || _birdPosition <= minYPosition);
}

void Bird::userInput(bool flap) {
    // Determine if it is a flap or a fall
    if (flap) {
        _flap();
    } else {
        _freeFall();
    }
}

void Bird::_freeFall() {

    // Set the current delay to gravitational. In another word, swtich mode
    _currentDelay = _currentGravitationalDelay;
    // Accelerate the gravity, or decrease the delay
    _currentGravitationalDelay -= _DEFAULT_ACCELERATION_RATE;
    _birdPosition++; // Keep track of the bird's position by moving it by 1px down, or more
    _currentFlapDelay = _DEFAULT_FLAP_DELAY; // Reset the flap delay
}

void Bird::_flap() {
    // Same thing as freeFall, the opposite way
    _currentDelay = _currentFlapDelay;
    _currentFlapDelay -= _DEFAULT_ACCELERATION_RATE;
    _birdPosition --;
    _currentGravitationalDelay = _DEFAULT_GRAVITATIONAL_DELAY;
}

// Getter methods

int Bird::getDelay() {
    return _currentDelay;
}

bool Bird::getGoingUp() {
    return (_currentDelay > 0);
}

int Bird::getBirdPosition() {
    return _birdPosition;
}

// Reset all data

void Bird::reset() {
    _birdPosition = _screenHeight / 3;
    // Set delay for gravity fall and flap rise for user input
    _currentDelay = 0;
    _currentGravitationalDelay = _DEFAULT_GRAVITATIONAL_DELAY;
    _currentFlapDelay = _DEFAULT_FLAP_DELAY;
}
