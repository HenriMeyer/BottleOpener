#include <string.h>
#include <stdlib.h>
#include "ssd1306xled.h"
#include "ssd1306xledfx.h"
#include "ssd1306xledtx.h"
#include "font6x8.h"
#include "display.h"
#include "avr_timer0.h"


void display_initialize(uint8_t vcc){
    DDRB |= (1 << vcc);             // Set VCC pin as output
    PORTB |= (1 << vcc);            // Set VCC pin high
    ms_counter = 0;                 // Reset the timer counter
    timer0_start();                 // Start timer0
    while(ms_counter < 30);         // Wait for 30 ms to ensure the display is ready
    timer0_stop();                  // Stop timer0
    ssd1306_init();
    ssd1306tx_init(font6x8, ' ');   // Initialize text with 6x8 font and base character 32
    ssd1306_clear();
}

void display_drawLines() {
    // Draw top line
    ssd1306_setpos(0, 0);
    ssd1306_start_data();
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        ssd1306_byte(0x01);
    }
    ssd1306_stop();

    // Draw bottom line
    ssd1306_setpos(0, 3);
    ssd1306_start_data();
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        ssd1306_byte(0x80);
    }
    ssd1306_stop();
}

void display_data_middle(uint16_t data){
    char buffer[6];
    itoa(data, buffer, 10);

    uint8_t len = strlen(buffer);

    uint8_t xOffset = DISPLAY_WIDTH / 2 - (len * CHAR_WIDTH) / 2;
    uint8_t yOffset = DISPLAY_HEIGHT / (BYTE_PIXELS * 2) - 1;

    ssd1306_setpos(xOffset, yOffset);   // Set cursor position
    ssd1306tx_string(buffer);           // Send the string (number)
}

void display_string_middle(char* data){
    uint8_t xOffset = DISPLAY_WIDTH / 2 - (strlen(data)* CHAR_WIDTH) / 2;
    uint8_t yOffset = DISPLAY_HEIGHT / (BYTE_PIXELS * 2) - 1;

    ssd1306_setpos(xOffset, yOffset);   // Set cursor position
    ssd1306tx_string(data);             // Send the data byte
}

void display_clear(){
    ssd1306_clear();        // Clear the display
    display_drawLines();    // Redraw the lines after clearing
}

void display_end(uint8_t vcc){
    ssd1306_clear();
    DDRB |= (1 << vcc);     // Set VCC pin as output
    PORTB &= ~(1 << vcc);   // Set VCC pin low to turn off the display
}