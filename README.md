# PhotonFlappyBird
This project uses the Particle Photon and an MicroOLED display with a button to play Flappy Bird. Yay!

Hello world, this project allows you to play a flappy-bird like game on an MicroOLED display with the Particle Photon. It uses a button to read the user's input considering the MicroOLED display is not touch screen. The game gets harder with more pillars presented on the screen as well as more speed, which is constantly accelerating until it hits a mark.

In addition to that, it also uses the EEPROM to store the highest score the player has achieved. This means that even if your Photon gets disconnected with the power or you flash a new firmware on there, the score will still be there and you can still access it.

How the animation and seemingly multithreading concept is in the comments on these files, and feel free to check them out as well as comment some suggestions.
