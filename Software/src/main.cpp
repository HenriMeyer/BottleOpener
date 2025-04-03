#include <Arduino.h>
#include <EEPROM.h>
#include "Display.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#define EEPROM_ADDRESS 0x00
#define DISPLAY_VCC 1
#define INCREASE_INTERRUPT_PIN 3  
#define DECREASE_INTERRUPT_PIN 4  

ISR(PCINT0_vect);
void goToSleep(void);
bool hasMagnitudeChanged(uint16_t oldValue, uint16_t newValue);
void disablePeripherals(void);
void enablePeripherals(void);

Display display;
volatile bool increaseFlag = false;
volatile bool decreaseFlag = false;
uint16_t count;
unsigned long timer;
uint16_t currentEEPROMAdress;
const uint16_t MAX_EEPROM_ADDRESS = E2END - sizeof(count) + 1;

void setup() {
    // Disable ADC
    ADCSRA &= ~(1 << ADEN);
    power_adc_disable();

    // Enable interrupts
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
        uint16_t oldCount = count;
        count++;
        if(hasMagnitudeChanged(oldCount, count)){
            display.clear();
            display.drawLines();
        }
        EEPROM.put(currentEEPROMAdress, count);
        display.write(count);
        timer = millis();
        delay(100);
    }

    if (decreaseFlag) {
        decreaseFlag = false;
        if (count > 0){
            uint16_t oldCount = count;
            count--;
            if(hasMagnitudeChanged(oldCount, count)){
                display.clear();
                display.drawLines();
            }
            EEPROM.put(currentEEPROMAdress, count);
            display.write(count);
            timer = millis();
            delay(100);
        }
    }

    if(millis() - timer >= 8000)
        goToSleep();
}

ISR(PCINT0_vect) {
    if (digitalRead(INCREASE_INTERRUPT_PIN) == LOW) {  
        increaseFlag = true;
    }
    if (digitalRead(DECREASE_INTERRUPT_PIN) == LOW) {  
        decreaseFlag = true;
    }
}

void goToSleep() {
    display.end();
    
    disablePeripherals();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
    enablePeripherals();

    display.initialize(DISPLAY_VCC);
    timer = millis();
}

bool hasMagnitudeChanged(uint16_t oldValue, uint16_t newValue) {
    uint8_t oldDigits = 1;
    uint8_t newDigits = 1;

    while (oldValue /= 10) ++oldDigits;
    while (newValue /= 10) ++newDigits;

    return oldDigits != newDigits;
}

void disablePeripherals() {
    power_timer0_disable();
    power_timer1_disable();
    power_usi_disable();
}

void enablePeripherals() {
    power_timer0_enable();
    power_timer1_enable();
    power_usi_enable();
}