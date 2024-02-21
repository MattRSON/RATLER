# Imports Libraries
import spidev


# Use Spi bus 0
bus = 0

# Chip select pin (Will be ignored for other pins)
device = 1

# Enable spi
spi = spidev.SpiDev()

# Open a connections to the device
spi.open(bus, device)

# Set the SPI speed and mode
spi.max_speed_hz = 62500 #1Mhz
spi.mode = 0

while(1):
    print('t1')
    data = input("Enter 1 or 0: ")
    print('t2')
    spi.writebytes([int(data)])
    print('t3')
            