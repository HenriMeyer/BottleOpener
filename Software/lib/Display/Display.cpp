#include "Display.h"
#include <Tiny4kOLED.h>

void Display::initialize(const uint8_t DisplayVCC) {
    pinMode(DisplayVCC, OUTPUT);
    digitalWrite(DisplayVCC, HIGH);

    oled.begin();
    oled.clear();
    oled.on();
    oled.setFont(FONT6X8);
}

void Display::drawFrame1() {
    // oled.switchRenderFrame();  // Wechsle zum Render Frame 2
    // oled.clear();
    // oled.setCursor(0, 0);
    // oled.print(F("Frame 1"));
    oled.switchRenderFrame();  // Wechsle zum Render Frame 1
    oled.clear();
}

void Display::drawFrame2() {
    oled.switchRenderFrame();  // Wechsle zum Render Frame 2
    oled.clear();
    oled.setCursor(0, 0);
    oled.print(F("Frame 2"));
}

void Display::switchFrames() {
    oled.switchDisplayFrame();  // Umschalten zwischen Display-Frames
}

void Display::drawRectangle() {
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
        oled.sendData(0xFF);
        oled.endData();
    }
}

void Display::write(uint16_t data) {
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
}
