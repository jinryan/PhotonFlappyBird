/******************************************************************************
pillar.cpp
Particle Photon Flappy Bird Project
Ryan Jin @ iD Tech Electric Engineering with the Arduino Platform
Original Creation Date: July 9
Original Completion Date: July 13
*****************************************************************************/

#include "pillar.h"

Pillar::Pillar(int screenWidth, int screenHeight, int upPillarHeight) {
    // Determine the screen width and height for future uses
    _screenHeight = screenHeight;
    _screenWidth = screenWidth;

    // Initialize the pillars position as well as height. Width is already a constant
    _upPillarHeight = upPillarHeight;
    _downPillarHeight = _screenHeight - BIRD_SPACE - _upPillarHeight;
    _upPillarPosition[0] = _screenWidth;
    _upPillarPosition[1] = 0;
    _downPillarPosition[0] = _screenWidth;
    _downPillarPosition[1] = _screenHeight - _downPillarHeight;

}

int** Pillar::returnPillarInformation() {
    // This array includes two rects. One is the top one and one is the bottom one
    // {x, y, width, height}
    // The thing about returning a two dimensional array is that it is required to use the new
    // keyword and return basically pointers. If I simply return the object without the new keyword
    // and the double astricks, then after the function is finished, the object will be gone.
    // In addition, you cannot do things like {{x, y, width, height}, {x, y, width, height}} when you
    // do a int**, so what you have to do is to first initialize it by making each array in pillarsRect
    // an int array with four items using the new keyword, and then loop through each of them, and using
    // the index of i and j to get the same item from pillarsNoPointer and assign it to that pointers array
    int pillarsNoPointer[2][4] = {
                        {
                            _upPillarPosition[0],
                            _upPillarPosition[1],
                            _PILLAR_WIDTH,
                            _upPillarHeight
                        },
                        {
                            _downPillarPosition[0],
                            _downPillarPosition[1],
                            _PILLAR_WIDTH,
                            _downPillarHeight
                        }
    };

    int** pillarsRect = new int*[2];
    for (int i = 0; i < 2; i++) {
       pillarsRect[i] = new int[4];
       for (int j = 0; j < 4; j++) {
           pillarsRect[i][j] = pillarsNoPointer[i][j];
       }
    }
    return pillarsRect;
}

void Pillar::changePillars(int change) {
    // Adjust the position according to user input
    _downPillarPosition[0] -= change;
    _upPillarPosition[0] -= change;
}

bool Pillar::isGone() {
    /* If the pillar's right most y point is off the screen,
    ** then it is gone, so there should be a memory management action */
    return (_upPillarPosition[0] <= 0);
}
