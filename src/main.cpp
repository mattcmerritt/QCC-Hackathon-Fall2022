#include <Arduino.h>
#include <M5Core2.h>

const int MAX_X = 320;
const int MAX_Y = 240;
const int SPRITE_COLOR_DEPTH = 8;

bool primed, active;

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

  Event& e = M5.Buttons.event;
  // check for a touch
  if (e & (E_TOUCH)) {
    // if unprimed prime
    if(!primed) {
      active = false;
      primed = true;

      // set screen for primed
      img.fillScreen(BLUE);
    }
    // if active unprime
    if(active) {
      active = false;
      primed = false;

      // set screen for unprimed
      img.fillScreen(GREEN);
    }
  }

  // check for significant change if primed
  if(primed && !active) {
    // get readings
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.IMU.getAhrsData(&pitch, &roll, &yaw);

    // check if readings suggest motion (gyroscope) or change overall (pitch and roll)
    if(abs(gyroX) > 40 || abs(gyroY) > 40 || abs(gyroZ) > 40 || abs(pitch - ppitch) > 20 || abs(pitch - ppitch) > 20) {
      // send the message back to the receiving device
      active = true;
    }
  }

  if(active) {
    img.fillScreen(RED);
    // play sound
  }

  // draw screen
  img.pushSprite(0, 0);
}