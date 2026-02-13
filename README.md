# RATLER
Robotic All Terrain Lunar Exploration Rover

Ratler code base for testing and development

Needs avrgcc from avr8-gnu-toolchain
and make from gnumin32

Flashing on rpi4
sudo avrdude -p atmega328p -C /etc/avrdude_gpio.conf -c RATLER -v -U flash:w:MicroController.hex:i

Serial Pinout
1
2 5V
3 MOSI
4 SCK
5 GND
6 MISO
7 3.3V
8 Chip Select
9 Reset

AVR side things to add
- spi communication
    - speed
    - direction
    - position of all wheels
    - flags for onboard sensors
- motor control
    - pwm / speed control
    - direction
    - flags for over temp
- position feedback
    - positional loop so motors know where they are

AVR pcb
- sensor pins ie 3.3v, signal, gnd
    - selectable 5v 3.3v header
- spi and programming header
- led status control
- battery monitor
- sub voltage monitors
- motor control and input header
- motor position header
- analog header
- any unused port has header
- crystal
