#include <Arduino.h>
#include <M5Core2.h>
#include "BluetoothSerial.h"

const int MAX_X = 320;
const int MAX_Y = 240;
const int SPRITE_COLOR_DEPTH = 8;

bool primed, active;

float gyroX, gyroY, gyroZ;

TFT_eSprite img = TFT_eSprite(&M5.Lcd);

// SCREEN COLOR CODES:
// green = pending priming
// blue = primed, not silent
// black = primed, silent, possibly active
// red = active

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  M5.begin();
  M5.IMU.Init();

  primed = false;

  img.setColorDepth(SPRITE_COLOR_DEPTH);
  img.createSprite(MAX_X, MAX_Y);

  img.fillScreen(GREEN);

  SerialBT.begin("ESP32 Motion Detector");
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

      SerialBT.println("ALARM ACTIVE");
    }
    // if active unprime
    if(active) {
      active = false;
      primed = false;

      // set screen for unprimed
      img.fillScreen(GREEN);

      SerialBT.println("ALARM DISABLED");
    }
  }

  // check for significant change if primed
  if(primed && !active) {
    // get readings
    M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);

    // check if readings suggest motion (gyroscope) or change overall (pitch and roll)
    if(abs(gyroX) > 40 || abs(gyroY) > 40 || abs(gyroZ) > 40) {
      // send the message back to the receiving device
      SerialBT.println("BEING STOLEN");
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