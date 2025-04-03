#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <Wire.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define BYTE_PIXELS 8

class Display {
private:
    uint8_t DisplayVCC;
public:
    void initialize(uint8_t DisplayVCC);
    void drawLines();
    void write(uint16_t data);
    void write(const char* data);
    void clear();
    void end();
};

#endif
