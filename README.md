# 🍾 BottleOpener

## 🔧 Flashing: Arduino as ISP to ATtiny85

> ℹ️ **Note:** The following pins must be connected when flashing the ATtiny85 using an Arduino UNO:

| Arduino Pin | ATtiny85 Pin | Function        |
|-------------|--------------|-----------------|
| 5V          | Pin 8        | VCC             |
| GND         | Pin 4        | GND             |
| Pin 10      | Pin 1        | RESET           |
| Pin 11      | Pin 5        | MOSI (D0)       |
| Pin 12      | Pin 6        | MISO (D1)       |
| Pin 13      | Pin 7        | SCK (D2)        |

### 📝 Instructions

1. Flash the Arduino UNO with the ArduinoISP sketch
2. Connect the ATtiny85 to the Arduino according to the table above
3. Use PlatformIO or the `flash.py` script to program the ATtiny85
4. Fuses, bootloader, and firmware will be written automatically

### 📦 Optional: Capacitor
> A 10 µF capacitor between RESET and GND on the Arduino helps to prevent auto-reset during flashing.