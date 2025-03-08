#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define BYTE_PIXELS 8

class Display {
public:
    void initialize(const uint8_t DisplayVCC);
    void drawFrame1();
    void drawFrame2();
    void switchFrames();
    void drawRectangle();
    void write(uint16_t data);
    void write(const char* data);
    void clear();
    void end();
};

#endif
