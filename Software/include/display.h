#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32
#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define BYTE_PIXELS 8

void display_initialize(uint8_t vcc);
void display_drawLines();
void display_data_middle(uint16_t data);
void display_string_middle(char* data);
void display_clear();
void display_end(uint8_t vcc);

#endif // DISPLAY_H
