#include "SPI.h"
#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned int lastTime = 0;
unsigned int frameIndex = 0;

void setup()
{
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
}

void loop()
{
  //drawLines();

  
  
  if(millis() - lastTime > 20){

    
    if(frameIndex >= 9){
      frameIndex = 0;
    }

    display.clearDisplay();
    drawFrame(frameIndex);
    display.display();
    
    frameIndex++;
    lastTime = millis();

    
  }

 

}


void drawFrame(int frameNumber){

    int gap = 8;
    int i = -10 + frameNumber;
    int j = -10 + gap + frameNumber;

    for (; j < display.width() + 10; i += gap, j += gap)
    {
      display.drawLine(i, 0, j, (display.height() - 1) / 2, SSD1306_WHITE);
      display.drawLine(j, (display.height() - 1) / 2, i, display.height() - 1, SSD1306_WHITE);
    }
}


