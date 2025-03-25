# 🍾 BottleOpener

A smart bottle opener with interrupt-driven bottle counting, power-saving sleep mode, and a custom PCB + 3D-printed case.

---

## 🧠 Project Overview

The **BottleOpener** is a microcontroller-based device that automatically counts opened bottles using a **digital interrupt** triggered by a mechanical contact. The current count is shown on an **OLED 0.91 display**.

When idle, the system enters a **deep sleep mode** to minimize power consumption and only wakes up when a bottle is opened.

---

## 🔧 Technical Details

- **Microcontroller:** ATtiny85 (flashed via Arduino UNO using ISP)
- **Interrupt-driven bottle detection**
- **Sleep mode:** Ultra-low power consumption when inactive
- **Display:** I²C OLED to show bottle count
- **Trigger mechanism:** Interrupt pin activated by a mechanical switch (e.g. reed contact or lever)

---

## 💡 Features

- Energy-efficient: less than 1 µA in deep sleep
- Wake-up via pin change interrupt (bottle opened)
- Persistent counter logic (reset via button optional)
- Display update on each trigger

---

## 🔩 Hardware

### 🔋 Electronics

- **PCB design** using **KiCad**
- Includes:
  - ISP header
  - I²C display connector
  - Interrupt pin (with internal pull-up enabled)
  - Power supply (e.g. CR2032)
  - LDO (HT7550)

### 🖨️ Enclosure

- Custom-designed housing created in **FreeCad**
- 3D-printable STL file included
- Designed to hold PCB, display, and battery securely

---

## 📝 Flashing the ATtiny85

Use an **Arduino UNO as ISP** and follow this wiring:

| Arduino Pin | ATtiny85 Pin | Function        |
|-------------|--------------|-----------------|
| 5V          | Pin 8        | VCC             |
| GND         | Pin 4        | GND             |
| Pin 10      | Pin 1        | RESET           |
| Pin 11      | Pin 5        | MOSI (D0)       |
| Pin 12      | Pin 6        | MISO (D1)       |
| Pin 13      | Pin 7        | SCK (D2)        |

### Optional: Add a 10 µF capacitor between RESET and GND on the Arduino to prevent auto-reset during flashing.

---

## 🚀 Getting Started

1. Flash `ArduinoISP` sketch to your Arduino UNO or use `flash.py`
2. Connect the ATtiny85 according to the table above
3. Flash the firmware using PlatformIO or `flash.py`
4. Mount everything in the printed housing
5. Start opening bottles 🍻

---

## 📜 License

This project is licensed under the **MIT License** — feel free to hack, fork, and improve

### 📦 Third-Party Libraries Used

This project uses the following third-party libraries:

- **Arduino Core** (LGPL) – https://www.arduino.cc/
- **Tiny4kOLED** (MIT) – https://github.com/datacute/Tiny4kOLED
