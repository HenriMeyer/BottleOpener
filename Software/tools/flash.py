import subprocess
import serial.tools.list_ports
import sys
import os

# --- Colours ---
RED = "\033[91m"
YELLOW = "\033[93m"
GREEN = "\033[92m"
RESET = "\033[0m"

# --- Check CLI args ---
if len(sys.argv) < 3:
    print(f"{RED}Error: You must pass the path to avrdude and .hex-file as an argument.{RESET}")
    print(f"    Example: python flash.py \"C:\avrdude\" \"C:\Privat\ProgrammierenProjekte\BottleOpener\Software\.pio\build\ATtiny85_ISP\firmware.hex\"")
    exit(1)

AVRDUDE_PATH = sys.argv[1].replace("\\", "/")
SOURCE = sys.argv[2].replace("\\", "/")

if not os.path.isdir(AVRDUDE_PATH):
    print(f"{RED}AVRDUDE path does not exist: {AVRDUDE_PATH}{RESET}")
    exit(1)
if not os.path.isfile(SOURCE) or not SOURCE.lower().endswith(".hex"):
    print(f"{RED}.hex file not found or invalid format: {SOURCE}{RESET}")
    exit(1)

# --- Detect COM Ports ---
print(f"{YELLOW}Searching for available COM ports...{RESET}", flush=True)
ports = list(serial.tools.list_ports.comports())
arduino_ports = [p.device for p in ports if 'Arduino' in p.description or 'CH340' in p.description or 'USB-SERIAL' in p.description]

if not arduino_ports:
    print(f"{RED}No Arduino found.{RESET}", flush=True)
    exit(1)

if len(arduino_ports) == 1:
    ComPort = arduino_ports[0]
    print(f"{GREEN}Arduino found at {ComPort}{RESET}")
else:
    print("Multiple ports found:")
    for i, port in enumerate(arduino_ports):
        print(f"  [{i}] {port}")
    index = input("Which port shall be used? Type in the number of the port: ")
    try:
        ComPort = arduino_ports[int(index)]
    except:
        print(f"{RED}Invalid input.{RESET}")
        exit(1)

print(f"\n{YELLOW}Using port: {ComPort}{RESET}\n")

# --- Arduino as ISP ---
print(f"{'='*40} {YELLOW}Flashing Arduino with ArduinoISP{RESET} {'='*40}", flush=True)

subprocess.run([
    f"{AVRDUDE_PATH}/avrdude.exe",
    "-C", f"{AVRDUDE_PATH}/avrdude.conf",
    "-V",
    "-p", "atmega328p",
    "-c", "arduino",
    "-P", ComPort,
    "-b", "115200",
    "-D",
    "-U", "flash:w:C:/Privat/ProgrammierenProjekte/BottleOpener/Software/tools/ArduinoISP.hex:i"
], check=True)

print(f"{GREEN}Arduino is now ready as ISP!{RESET}", flush=True)

# --- Bootloader for ATtiny85 ---
print(f"{'='*41} {YELLOW}Clear flash of ATtiny85{RESET} {'='*41}", flush=True)

subprocess.run([
    f"{AVRDUDE_PATH}/avrdude.exe",
    "-C", f"{AVRDUDE_PATH}/avrdude.conf",
    "-p", "attiny85",
    "-c", "stk500v1",
    "-P", ComPort,
    "-b", "19200",
    "-e",
    "-U", "efuse:w:0xFF:m",
    "-U", "hfuse:w:0b11010111:m",
    "-U", "lfuse:w:0x42:m",
    "-U", "flash:w:C:/Privat/ProgrammierenProjekte/BottleOpener/Software/tools/empty_all.hex:i"
], check=True)

print(f"{GREEN}Cleared flash successfully!{RESET}", flush=True)

# --- Flash firmware ---
print(f"{'='*43} {YELLOW}Flashing ATtiny85 firmware{RESET} {'='*43}", flush=True)

subprocess.run([
    f"{AVRDUDE_PATH}/avrdude.exe",
    "-C", f"{AVRDUDE_PATH}/avrdude.conf",
    "-p", "attiny85",
    "-c", "stk500v1",
    "-P", ComPort,
    "-b", "19200",
    "-U", f"flash:w:{SOURCE}:i"
], check=True)

print(f"{GREEN}Firmware flashed successfully!{RESET}")