/******************************************************************************
bird.h
Particle Photon Flappy Bird Project
Ryan Jin @ iD Tech Electric Engineering with the Arduino Platform
Original Creation Date: July 9
Original Completion Date: July 13
*****************************************************************************/

// ####### For algorithm, please read the notes at bird.cpp #######

#ifndef BIRD_H
#define BIRD_H

#include "pillar.h"

class Bird {
    public:

        Bird(int birdSize, int screenWidth, int screenHeight);
        void userInput(bool flap);

        // Getter methods
        int getDelay();
        bool getGoingUp();
        int getBirdPosition();

        // Determine if the bird crashed or not
        bool birdCrashed(Pillar **pillars, int amountOfPillars);

        // Reset
        void reset();

    private:
        // Constants that keep track of the acceleration
        const double _DEFAULT_GRAVITATIONAL_DELAY = 50; // Default delay
        const double _DEFAULT_FLAP_DELAY = 40;
        const double _DEFAULT_ACCELERATION_RATE = 0.3; // How much the delay decreases after the animation of each pixel if the user keeps pressing

        double _currentGravitationalDelay; // Free falling delay
        double _currentFlapDelay; // Flapping delay

        int _birdSize;
        int _birdPosition;
        int _screenHeight;
        int _screenWidth;
        int _currentDelay;
        bool _goingUp;

        void _freeFall();
        void _flap();


};

#endif
