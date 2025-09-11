avr-gcc -mmcu=atmega2560 -DF_CPU=16000000UL -Os -nostartfiles -nostdlib -W -Tblink.ld -o blink.elf blink.c 
avr-objcopy -O ihex -R .eeprom blink.elf blink.hex
avrdude -v -p atmega2560 -c wiring -P /dev/ttyACM0 -b 115200 -D \
  -U flash:w:blink.hex:i
