#!/bin/bash
avrdude -C /home/chenty/arduino-1.0.4/hardware/tools/avrdude.conf -p atmega16U2 -c stk500v1 -P /dev/ttyACM0 -b 19200 -U flash:w:"/home/chenty/桌面/W_Project/HIDUINO/ddiakopoulos-hiduino-fe3c0c7/Compiled Firmwares/HIDUINO_MIDI.hex":i -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m
read
