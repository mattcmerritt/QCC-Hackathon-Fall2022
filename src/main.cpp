#include <Arduino.h>
#include <M5Core2.h>

const int MAX_X = 320;
const int MAX_Y = 240;
const int SPRITE_COLOR_DEPTH = 8;

bool primed, silent, active;

float gyroX, gyroY, gyroZ;
float pitch, roll, yaw;
float ppitch, proll, pyaw; // values when primed, used for comparison

TFT_eSprite img = TFT_eSprite(&M5.Lcd);

// SCREEN COLOR CODES:
// green = pending priming
// blue = primed, not silent
// black = primed, silent, possibly active
// red = active

void setup() {
  M5.begin();
  M5.IMU.Init();

  primed = false;

  img.setColorDepth(SPRITE_COLOR_DEPTH);
  img.createSprite(MAX_X, MAX_Y);

  img.fillScreen(GREEN);
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
    silent = false;
    active = false;
  }
  if(M5.BtnB.wasPressed()) {
    M5.IMU.getAhrsData(&ppitch, &proll, &pyaw);
    primed = true;
    silent = true;
    active = false;
  }
  if(M5.BtnC.wasPressed()) {
    primed = false;
    silent = false;
    active = false;
    img.fillScreen(GREEN);
  }

  // check for significant change if primed
  if(primed && !active) {
    // set screen color based on mode
    if(silent) {
      img.fillScreen(BLACK);
    }
    else {
      img.fillScreen(BLUE);
    }

    // get readings
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);

    // check if readings suggest motion (gyroscope) or change overall (pitch and roll)
    if(abs(gyroX) > 40 || abs(gyroY) > 40 || abs(gyroZ) > 40 || abs(pitch - ppitch) > 20 || abs(pitch - ppitch) > 20) {
      // send the message back to the receiving device
      
      active = true;
      if(!silent) {
        img.fillScreen(RED);
        // play sound
      }
    }
  }

  // draw screen
  img.pushSprite(0, 0);
}