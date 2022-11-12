#include <Arduino.h>
#include <M5Core2.h>

#include "BluetoothSerial.h"
   
BluetoothSerial SerialBT;
int count = 0;
    
void setup()
{
  SerialBT.begin("Demo ESP32 Device");
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.clear();
}
  
// every 2 seconds, the screen flashes green and the device sends a message over bluetooth
void loop()
{
  count++;
  if (count % 20 == 0) {
    SerialBT.println("Timer finished!");
    M5.Lcd.fillScreen(GREEN);
  }
  else {
    M5.Lcd.fillScreen(RED);
  }
  delay(100);
}