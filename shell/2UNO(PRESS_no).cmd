
D:\arduino-1.0.1-windows\hardware\tools\avr\bin\avrdude.exe -C D:\arduino-1.0.1-windows\hardware/tools/avr/etc/avrdude.conf -p atmega328p -c stk500v1 -P \\.\COM3 -b 19200 -U flash:w:D:\arduino-1.0.1-windows\hardware\arduino\firmwares\Arduino-COMBINED-dfu-usbserial-atmega16u2-Uno-Rev3.hex:i -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m -F

pause
