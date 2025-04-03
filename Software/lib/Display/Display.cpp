#include "Display.h"
#include <Tiny4kOLED.h>

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

void Display::initialize(uint8_t DisplayVCC) {
    this->DisplayVCC = DisplayVCC;
    pinMode(this->DisplayVCC, OUTPUT);
    digitalWrite(this->DisplayVCC, HIGH);

    Wire.begin();
    oled.begin();
    oled.clear();
    oled.on();
    oled.setFont(FONT6X8);
    drawLines();
}

void Display::drawLines() {
    // Rectangle around the screen
    for(uint8_t i = 0; i < DISPLAY_WIDTH / BYTE_PIXELS; i++) {
        oled.setCursor(0, i);
        oled.startData();
        if(i == 0)
            oled.repeatData(0x01, DISPLAY_WIDTH - 2);
        else if(i == 1 || i == 2)
            oled.repeatData(0x00, DISPLAY_WIDTH - 2);
        else if(i == 3)
            oled.repeatData(0x80, DISPLAY_WIDTH - 2);
        oled.endData();
    }
}

void Display::write(uint16_t data) {
    uint16_t tmp = data;
    uint8_t digits = 0;
    uint8_t xOffset = DISPLAY_WIDTH/2;
    uint8_t yOffset = DISPLAY_HEIGHT/(BYTE_PIXELS * 2) - 1;
    if(data > 0){
        while(tmp > 0) {
            tmp /= 10;
            digits++;
        }
        if(digits%2 == 0)
            xOffset -= (digits/2) * CHAR_WIDTH;
        else
            xOffset -= (digits/2) * CHAR_WIDTH + CHAR_WIDTH/2;
    }
    oled.setCursor(xOffset, yOffset);
    oled.print(data);
}

void Display::write(const char* data) {
    oled.print(data);
}

void Display::clear() {
    oled.clear();
}

void Display::end() {
    oled.clear();
    oled.off();
    pinMode(this->DisplayVCC, OUTPUT);
    digitalWrite(this->DisplayVCC, LOW);
}