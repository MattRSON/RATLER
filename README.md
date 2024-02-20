# RATLER
Robotic All Terrain Lunar Exploration Rover

Ratler code base for testing and development

Needs avrgcc from avr8-gnu-toolchain
and make from gnumin32

Flashing on rpi4
sudo avrdude -p atmega328p -C /etc/avrdude_gpio.conf -c RATLER -v -U flash:w:MicroController.hex:i
