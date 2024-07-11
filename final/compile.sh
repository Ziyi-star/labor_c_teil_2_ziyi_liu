#!/bin/bash
echo COMPILING...
echo avr-gcc $1 $2 $3 $4 $5 $6 $7 $8 $9 $A $B -o tmpfile -mmcu=atmega328p -Os -D F_CPU=16E6
avr-gcc $1 $2 $3 $4 $5 $6 $7 $8 $9 $A $B -o tmpfile -mmcu=atmega328p -Os -D F_CPU=16E6

echo GENERATING FIRMWARE...
echo avr-objcopy -O ihex tmpfile tmpfile.hex
avr-objcopy -O ihex tmpfile tmpfile.hex

echo FLASHING....
echo avrdude -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex:i
avrdude -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:tmpfile.hex:i

echo REMOVING TMP-FILE...
echo rm ./tmpfile ./tmpfile.hex
rm ./tmpfile ./tmpfile.hex
