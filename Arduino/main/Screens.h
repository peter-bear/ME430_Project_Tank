#ifndef SCREENS_H
#define SCREENS_H

#include "SPI.h"
#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

#include "Utilities.h"
#include "THSensor.h"

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 32    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// SDA to pin 20; SCL to pin 21

namespace me430
{
    class Screens
    {
    public:
        Screens();
        ~Screens();

        void init();
        void onEvent(Event *e);
        void update();
        void sync();

    private:
        Adafruit_SSD1306* display;
        unsigned long lastTime;
        byte frameNumber;
    };
}

#endif