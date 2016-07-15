/******************************************************************************
FlappyBird.ino
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
* There are two animations going on. One is the bird, one is the pillars. The
* problem if we use delay is that they update at different rate, so it will
* result in one of them having animation with inconsistent rate. So we have to do
* two at them at once instead of doing them together.
*
* Since photon doesn't support multithreading, and protothreading is very hard,
* so it is better to not use functions like delay and do synchronized programming.
* Instead, set a benchmark for when should the next action be executed, and wait
* until the internal timer in the Photon reaches that mark, and then execute that
* action. The advantage of this is that the way you wait for that action to be
* executed allows you to do things in between that literally takes no time.
******************************/

// ============== Header files and constants =================
#include "flashee-eeprom/flashee-eeprom.h" // Persistent storage
#include "SparkFunMicroOLED/SparkFunMicroOLED.h"  // Include Micro OLED library

#include "PillarManager.h" // Pillar Manager manages the pillars
#include "pillar.h" // Pillars are responsible for individual pairs of pillars
#include "bird.h" // The bird class is responsible for the flappy bird on screen

using namespace Flashee;

// Set pins for the OLED for SPI connection
// ============================== Constants =================================
#define PIN_RESET D7  // Connect RST to pin 7
#define PIN_DC    D6  // Connect DC to pin 6
#define PIN_CS    A2 // Connect CS to pin A2

#define FLAPPY_SIZE 2 // The bird is a circle. This is the radius

// ================== Create Oled, Flappy, and Column Objects ===============
MicroOLED oled(MODE_SPI, PIN_RESET, PIN_DC, PIN_CS);


// This function receives the delay returned from the pillarManager and set it to the variable, and when the millis() function reaches that time, it executes the action, which is animation.
void updateBirdTime(int delay);

// This function sets all the variables to default, and restarts the game
void resetGame();

// This function handles the part where it receives the user's input and handle what to do with the bird
void getUserInput();

// Constants
const int SCREEN_WIDTH = oled.getLCDWidth();
const int SCREEN_HEIGHT = oled.getLCDHeight();
const int buttonPin = D0; // Button pin for users input

// Animated objects
Bird flappy(FLAPPY_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT); // The flappy bird on screen
PillarManager pillarManager(SCREEN_WIDTH, SCREEN_HEIGHT); // Manages all the pillars and give the needed information
FlashDevice* flash; // Manages persistent storage
Pillar **pillars; // This is an array of pillar objects, and it includes all the pillars that are supposed to be displayed on the screen

// Flappy delay and previous times
unsigned int nextTimeForFlappy; // This variable determines which mark the internal timer has to reach in able to move the flappy bird on screen by 1 px
unsigned int nextTimeForPillars; // This variables determines which mark the internal timer has to reach in able to move the pillars on the screen by 1 px

// Each time when the user STARTS pressing the button, we want to make the flappy bird go up not just by 1 px but multiple pixels so this variable allows the flappy bird to have the ability to go up despite the user's input. And each time it moves 1 px in the loop, it is automatically decreased by 1 until it reaches 0
int flapUpTime = 0;

// These two following variables work with the flapUpTime variable and determine if the user STARTED pressing the button or has already pressed the button for a long time
bool isFirstFlap = false;
bool previousFlap = false;

// This variable records the highest score
int currentHighScore = 0;


// =========== Setup and Loop ===============

void setup() {
    // Serial.begin(9600);
    flash = Devices::createDefaultStore();
    flash->read(currentHighScore, 10); // Record value for address 10 into the current high score

    pinMode(buttonPin, INPUT); // Button pin

    // Create bird circle
    oled.begin();
    oled.clear(ALL);
    oled.clear(PAGE);
    oled.circle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, FLAPPY_SIZE); // Put the bird object on screen
    oled.display();

    // Initialize the expecation times
    nextTimeForPillars = millis() + pillarManager.getDelay(); // When should the pillar by updated by 1 px
    getUserInput(); // See what the user is doing with the button and do actions with it
}



void loop() {

// If it is the first flap, indicated by a positive non-zero flapUpTime
  if (flapUpTime > 0) {
    flapUpTime--;
    flappy.userInput(true); // False means flapped, and this will
    nextTimeForFlappy = millis(); // So I can trigger the action
  }

  if (millis() >= nextTimeForPillars) {
    oled.clear(PAGE); // Clear display
    // Step 1: Print out the high score
    for (int a = 0; a < 39; a++) {
        // Make space to go on the fifth line since oled.setCursor doesn't really work
        oled.print(" ");
    }
    oled.print("High");
    for (int a = 0; a < 6; a++) {
        // Set new line to line 6 with a bunch of space
        oled.print(" ");
    }
    oled.print((String)currentHighScore);

    // Do pillars stuff
    // Step 2: Update next action time at the start so the processing time in between, which isn't consistent considering it might only have to animate 1 pair of pillars or it might have to animate 3 pairs, will not affect when the next animation starts, so it keeps it at a constant rate.
    nextTimeForPillars = millis() + pillarManager.getDelay();

    pillars = pillarManager.timeToMove(); // Returns an array of pillars

    for (int i = 0; i < pillarManager.getCurrentAmountOfPillarsOnScreen(); i++) {
        Pillar *pillar = pillars[i];
        int **pillarInformation = pillar->returnPillarInformation();
        // The pillar information is a two dimensional array consisting of two arrays each with 4 items. The first array is the top pillar while the second array is the bottom pillar. The items in each of these two arrays are the x, y, width, and height
        for (int j = 0; j < 2; j++) {
          int *rectInfo = pillarInformation[j];
            oled.rect(rectInfo[0], rectInfo[1],rectInfo[2], rectInfo[3]);
            delete[] rectInfo; // Remove this from memory
        }
        // Remove the pillar information from memory since it is created
        for (int x = 0; x < 2; x++) {
          // Since it is a two dimensional array, loop through the first level and delete their arrays
          delete pillarInformation[x];
        }
        delete[] pillarInformation;
    }

    oled.setCursor(1, 1);
    oled.print((String)pillarManager.getAmountOfPillarsUserPassed()); // User's current score
    oled.display();
  }

  // Bird animation, if the time is reached
  if (millis() >= nextTimeForFlappy) {
    // Determine if game is over
    if (flappy.birdCrashed(pillars, pillarManager.getCurrentAmountOfPillarsOnScreen())) {
      resetGame();
    } else {
      // If not, display the circles
      oled.circle(SCREEN_WIDTH / 2, flappy.getBirdPosition(), FLAPPY_SIZE);
      oled.display();
      getUserInput(); // After finishing display, get user input
    }
    delay(1); // Delay 1 millisecond so the program doesn't screw up the CPU
  }
}

// ============== Functions update bird time and reset ===============

void updateBirdTime(int delay) {
  nextTimeForFlappy = millis() + delay;
}

void resetGame() {
  // Clear screen
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  // Record user score, compare that to existing high score, and display it
  int userScore = pillarManager.getAmountOfPillarsUserPassed();
  if (userScore > currentHighScore) {
      oled.print("Yay, you  beat the  high score          Your scoreis ");
      currentHighScore = userScore;
      flash->write(currentHighScore, 10);
  } else {
      oled.print("Nice job. Your score is ");
  }
  oled.print((String)userScore);
  oled.display();
  delay(600);

  // RESTARTING THE GAME
  oled.clear(ALL);
  oled.clear(PAGE);
  oled.setCursor(1, 1);
  // Display message
  oled.print("Game over.Currently restarting");
  oled.display();

  // Reset
  flappy.reset();
  pillarManager.reset();
  delay(800);

  // Redisplay the flappy bird
  oled.clear(ALL);
  oled.clear(PAGE);
  oled.circle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, FLAPPY_SIZE);
  oled.display();
  delay(200);

  // Reset variables
  nextTimeForPillars = millis() + pillarManager.getDelay();
  previousFlap = false;
  isFirstFlap = false;
  flapUpTime = 0;
  getUserInput();
}

void getUserInput() {
    bool flap = digitalRead(buttonPin); // Read user input

    flappy.userInput(flap); // True is user pressed down.
    updateBirdTime(flappy.getDelay()); // Set next delay time

    // Make the isFirstFlap variable, and if it is first, then boost the bird
    isFirstFlap = (previousFlap ==  false && flap);

    previousFlap = flap;
    if (isFirstFlap) { flapUpTime = 3; }
}
