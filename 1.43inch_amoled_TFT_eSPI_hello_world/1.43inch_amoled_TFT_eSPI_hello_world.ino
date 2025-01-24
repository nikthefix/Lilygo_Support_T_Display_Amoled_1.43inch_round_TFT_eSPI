/*
nikthefix - 24.01.25
For Lilygo T-Display-S3-Amoled-1.75"
Test sketch using TFT_eSPI in 'sprite only mode' 



Dependencies:

ESP32_Arduino boards package version 3.1.1
TFT_eSPI 2.5.34 
  


This build uses a lean SH8601 display driver rather than the Arduino_GFX framework used in the Lilygo supplied examples.
The driver files are supplied in the sketch directory so no installation is required.
The touch driver is also included in the sketch directory so no installation is required.
I recommend a fresh install of TFT_eSPI leaving all user settings at default.

Set display brightness in setup() ---> lcd_brightness(200); // 0-255



Build options:

Select board ESP32S3 Dev Module
Select USB CDC On Boot "Enabled"
Select Flash Size 16M
Select Partition Scheme "custom" - partitions.csv in sketch folder will be used
Select PSRAM "OPI PSRAM"



Troubleshooting:

In case of build failure you can try deleting the cached sketch object files located here -
"C:\Users\your_user_name\AppData\Local\arduino\sketches"

*/


#include <Arduino.h>
#include <TFT_eSPI.h>
#include "pins_config.h"
#include "src/display/sh8601.h"
#include "src/touch/FT3168.h"

// For bitmap encoding: use Image2lcd, 16bit true colour, MSB First, RGB565, don't include head data, be sure to set max image size, save as .h file.

bool  isPressed = false;
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);



void draw_first()
{
 sprite.fillSprite(TFT_BLACK);
 sprite.drawString("Hello World",168,120,4);
 sprite.fillRect(130,180,60,60,TFT_RED);
 sprite.fillRect(200,180,60,60,TFT_GREEN);
 sprite.fillRect(270,180,60,60,TFT_BLUE); 
 sprite.drawString("Touch:",190,280,4);
 lcd_PushColors(0, 0, 466, 466, (uint16_t*)sprite.getPointer());
}



static void read_touch()
{
  if (isPressed)
  {
     isPressed = false;  
     uint16_t tp_x,tp_y;
     uint8_t touchpad = getTouch(&tp_x, &tp_y);
       if (touchpad > 0)
       {
          //Serial.printf("X: %d   Y: %d\n", tp_x, tp_y); //for testing
          sprite.setCursor(150,320,4);
          sprite.printf("X: %03d   Y: %03d", tp_x, tp_y);
          lcd_PushColors(0, 0, 466, 466, (uint16_t*)sprite.getPointer());
       }
  }
}

    

void setup() 
{
    Serial.begin(115200);
    Serial.printf("psram size : %d MB\r\nflash size : %d MB\r\n", ESP.getPsramSize() / 1024 / 1024, ESP.getFlashChipSize() / 1024 / 1024);
    pinMode(LCD_VCI_EN, OUTPUT);
    digitalWrite(LCD_VCI_EN, HIGH);
    attachInterrupt(TOUCH_INT, []() {isPressed = true;}, FALLING); //Attach interrupt
    Touch_Init(); 
    sh8601_init();
    //lcd_setRotation(0); //map touch coordinates for different rotations
    lcd_brightness(150); // 0-255    
    sprite.createSprite(466, 466);    // full screen landscape sprite in psram
    sprite.setSwapBytes(1);
    draw_first();   
}


void loop() 
{     
    read_touch();      
}




void check_for_memory_leaks() {

    Serial.print(F("Total heap  ")); Serial.println(ESP.getHeapSize());
    Serial.print(F("Free heap   ")); Serial.println(ESP.getFreeHeap());
    Serial.print(F("Total psram ")); Serial.println(ESP.getPsramSize());
    Serial.print(F("Free psram  ")); Serial.println(ESP.getFreePsram());
}
