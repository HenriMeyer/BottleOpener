#include <stdint.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/eeprom.h>

#include "avr_timer0.h"
#include "display.h"

// === systemFlags bit definitions ===
// Bit 0: FLAG_BUTTON_INCREASE → Button to increase the counter was pressed
// Bit 1: FLAG_BUTTON_DECREASE → Button to decrease the counter was pressed
// Bit 2: FLAG_GOODBYE_DISPLAYED → Indicates if "Goodbye!" message has already been shown
#define FLAG_BUTTON_INCREASE     (1 << 0)
#define FLAG_BUTTON_DECREASE     (1 << 1)
#define FLAG_GOODBYE_DISPLAYED   (1 << 2)

// === Hardware pin definitions ===
#define DISPLAY_VCC PB1  // Pin for VCC of the display

// === Global variables ===
volatile uint8_t systemFlags = 0x00;
uint16_t EEPROM_ADDRESS = 0x00;


// === Functions ===
ISR(PCINT0_vect);
void sleep(void);
uint8_t hasMagnitudeLowered(uint16_t oldValue, uint16_t newValue);

int main(void){
    // === Deactivate peripherals ===
    power_adc_disable();
    power_usi_disable();
    power_timer1_disable();

    // === Configuration of GPIO pins ===
    DDRB |= (0x01); 	    // Set PB0 as output
    PORTB &= ~(0x01);   	// Set PB0 to Low
    DDRB &= ~(0x18);    	// Set PB3 and PB4 as input
    PORTB |= (0x18);    	// Set PB3 and PB4 to High (pull-up)

    // === Enable interupts ===
    cli();
    GIMSK |= (0x20);        // Enable Pin Change Interrupts
    PCMSK |= (0x18);        // Enable pin change interrupts for PB3 and PB4
    sei();    
    
    // === Init ===
    timer0_init();
    display_initialize(DISPLAY_VCC);
    uint16_t currentData = eeprom_read_byte((uint8_t*)EEPROM_ADDRESS);
    
    display_drawLines();
    display_string_middle("Welcome!");
    timer0_start();
    while(ms_counter < 2000);
    display_clear();
    ms_counter = 0x00;

    // === Loop ===
    while(1)
    {
        if(systemFlags & FLAG_BUTTON_INCREASE){
            systemFlags &= ~FLAG_BUTTON_INCREASE;
            currentData++;
            eeprom_write_byte((uint8_t*)EEPROM_ADDRESS, currentData);
            display_data_middle(currentData);
            ms_counter = 0;
            while(ms_counter < 100); // Delay
        } else if(systemFlags & FLAG_BUTTON_DECREASE){
            systemFlags &= ~FLAG_BUTTON_DECREASE;
            currentData--;
            eeprom_write_byte((uint8_t*)EEPROM_ADDRESS, currentData);
            display_data_middle(currentData);
            ms_counter = 0;
            while(ms_counter < 100); // Delay
        }
        if(ms_counter > 5000 && !(systemFlags & FLAG_GOODBYE_DISPLAYED)){
            systemFlags |= (FLAG_GOODBYE_DISPLAYED);
            display_clear();
            display_string_middle("Goodbye!");
        }
        if(ms_counter > 8000){
            sleep();
            ms_counter = 0;
            display_drawLines();
        }
    }

    return 0;
}

ISR(PCINT0_vect){
    uint8_t currentIncreaseFlag = !(PINB & (1 << PB3));
    uint8_t currentDecreaseFlag = !(PINB & (1 << PB4));
    if (currentDecreaseFlag != 0){
        systemFlags |= (FLAG_BUTTON_DECREASE);
        systemFlags &= ~(FLAG_BUTTON_INCREASE);
    } else if (currentIncreaseFlag != 0){
        systemFlags |= (FLAG_BUTTON_INCREASE);
        systemFlags &= ~(FLAG_BUTTON_DECREASE);
    }
    if(systemFlags & FLAG_GOODBYE_DISPLAYED){
        systemFlags &= ~(FLAG_GOODBYE_DISPLAYED);
        display_clear();
    }
}

void sleep(void){
    // Disable Display
    display_end(DISPLAY_VCC);
    timer0_stop();

    // Disable peripherals
    power_timer0_disable();

    // Set sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    sei();
    
    // Go to sleep
    sleep_cpu();
    
    // Wake up
    sleep_disable();

    // Enable peripherals again
    power_timer0_enable();
    display_initialize(DISPLAY_VCC);
    timer0_start();
}

uint8_t hasMagnitudeLowered(uint16_t oldValue, uint16_t newValue) {
    uint8_t oldDigits = 1;
    uint8_t newDigits = 1;

    while (oldValue /= 10) ++oldDigits;
    while (newValue /= 10) ++newDigits;

    return oldDigits > newDigits;
}