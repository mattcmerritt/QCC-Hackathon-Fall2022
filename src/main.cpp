#include <Arduino.h>
#include <M5Core2.h>

bool primed, usesound, active;
float pitch, roll, yaw;
float ppitch, proll, pyaw; // values when primed, used for comparison

void setup() {
  M5.begin();
  M5.IMU.Init();

  primed = false;
}

void loop() {
  // update (for button press)
  M5.update();

  // button behavior

  // button A - prime device
  // button B - prime device in silent mode
  // button C - deactivate device
  if(M5.BtnA.wasPressed()) {
    M5.IMU.getAhrsData(&ppitch, &proll, &pyaw);
    primed = true;
    usesound = false;
  }
  if(M5.BtnB.wasPressed()) {
    M5.IMU.getAhrsData(&ppitch, &proll, &pyaw);
    primed = true;
    usesound = true;
  }
  if(M5.BtnA.wasPressed()) {
    primed = false;
    usesound = false;
  }

  // check for significant change if primed
  if(primed) {
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);
    if(abs(pitch - ppitch) > 20 || abs(pitch - ppitch) > 20) {
      // send the message back to the receiving device

      if(usesound) {
        // play the sound
      }
    }
  }
}