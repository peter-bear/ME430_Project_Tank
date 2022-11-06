#include "Screens.h"

using namespace me430;

Screens::Screens()
{
    lastTime = 0;
}

Screens::~Screens()
{
}

void Screens::init()
{
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Clear the buffer
    display->clearDisplay();
}

void Screens::update()
{
    if (millis() - lastTime > 20)
    {

        if (frameNumber >= 9)
        {
            frameNumber = 0;
        }

        display->clearDisplay();

        int gap = 8;
        int i = -10 + frameNumber;
        int j = -10 + gap + frameNumber;

        for (; j < display->width() + 10; i += gap, j += gap)
        {
            display->drawLine(i, 0, j, (display->height() - 1) / 2, SSD1306_WHITE);
            display->drawLine(j, (display->height() - 1) / 2, i, display->height() - 1, SSD1306_WHITE);
        }

        display->display();

        frameNumber++;
        lastTime = millis();
    }
}