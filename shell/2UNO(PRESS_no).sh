#!/bin/bash
avrdude -C /home/chenty/arduino-1.0.4/hardware/tools/avrdude.conf -p atmega16U2 -c stk500v1 -P /dev/ttyACM0 -b 19200 -U flash:w:/home/chenty/arduino-1.0.4/hardware/arduino/firmwares/atmegaxxu2/Arduino-COMBINED-dfu-usbserial-atmega16u2-Uno-Rev3.hex:i -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
read
