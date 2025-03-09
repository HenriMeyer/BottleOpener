#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "Display.h"
#include <util/delay.h>

#define EEPROM_ADDRESS 0x00
#define DISPLAY_VCC 1
#define INCREASE_INTERRUPT_PIN 3  
#define DECREASE_INTERRUPT_PIN 4  

Display display;
volatile bool increaseFlag = false;
volatile bool decreaseFlag = false;
volatile bool displayFlag = false;
uint16_t count;
unsigned long timer;
uint16_t currentEEPROMAdress;
const uint16_t MAX_EEPROM_ADDRESS = E2END - sizeof(count) + 1;

ISR(PCINT0_vect) {
    _delay_ms(50);
    if (digitalRead(INCREASE_INTERRUPT_PIN) == LOW) {  
        increaseFlag = true;
    }
    if (digitalRead(DECREASE_INTERRUPT_PIN) == LOW) {  
        decreaseFlag = true;
    }
}

void goToSleep() {
    display.end();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();

    timer = millis();
    display.initialize(DISPLAY_VCC);
}

void setup() {
    cli();
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << PCINT3) | (1 << PCINT4);
    sei();
    
    pinMode(INCREASE_INTERRUPT_PIN, INPUT_PULLUP);
    pinMode(DECREASE_INTERRUPT_PIN, INPUT_PULLUP);
    
    display.initialize(DISPLAY_VCC);
    currentEEPROMAdress = EEPROM_ADDRESS;
    EEPROM.get(currentEEPROMAdress, count);
    while(count >= 25000){ // 25000 => 1/4 of max write cycles
        currentEEPROMAdress += sizeof(count);
        if(currentEEPROMAdress >= MAX_EEPROM_ADDRESS){ // Reset EEPROM
            currentEEPROMAdress = EEPROM_ADDRESS;
            for (uint16_t i = 0; i <= E2END; i++) 
                EEPROM.update(i, 0xFF);
            display.write("EEPROM IS RISKY TO USE");
            delay(5000);
            }
        EEPROM.get(currentEEPROMAdress, count);
        if(count == 0xFFFF){
            count = 0;
            EEPROM.put(currentEEPROMAdress, count);
        }
    }
    display.write(count);
    timer = millis();
}

void loop() {
    if (increaseFlag) {
        increaseFlag = false;
        displayFlag = true;
        count++;
        EEPROM.put(currentEEPROMAdress, count);
        // display.clear();
        display.write(count);
        timer = millis();
        delay(200);
    }

    if (decreaseFlag) {
        decreaseFlag = false;
        displayFlag = true;
        if (count > 0){
            count--;
            EEPROM.put(currentEEPROMAdress, count);
            // display.clear();
            display.write(count);
        }
        timer = millis();
        delay(200);
    }

    if(millis() - timer >= 8000)
        goToSleep();
}
