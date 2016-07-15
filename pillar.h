/******************************************************************************
pillar.h
Particle Photon Flappy Bird Project
Ryan Jin @ iD Tech Electric Engineering with the Arduino Platform
Original Creation Date: July 9
Original Completion Date: July 13
*****************************************************************************/

#ifndef PILLAR_H
#define PILLAR_H

class Pillar {

    public:

        Pillar(int screenWidth, int screenHeight, int upPillarHeight);
        int** returnPillarInformation(); // This function returns a two dimensional array. They are information of the top and bottom pillar, and each of these pillars consist of four items, or a rect: x, y, width, and height
        void changePillars(int change); // Shift the x to the left by the "change" parameter
        bool isGone(); // Returns if the pillar is off the screen or not

        static const int BIRD_SPACE  = 25; // The space needed between pillars

    private:

        const int _PILLAR_WIDTH  = 10; // Constant pillar width

        int _screenHeight;
        int _screenWidth;
        int _upPillarHeight;
        int _downPillarHeight;
        unsigned int _upPillarPosition[2];
        unsigned int _downPillarPosition[2];
};


#endif
