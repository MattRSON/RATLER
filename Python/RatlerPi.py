# Imports Libraries
import spidev


# Use Spi bus 0
bus = 0

# Chip select pin
device = 0

# Enable spi
spi = spidev.SpiDev()

# Open a connections to the device
spi.open(bus, device)

# Set the SPI speed and mode
spi.max_speed_hz = 38400 #1Mhz
spi.mode = 0

while(1):
    data = input("Enter 1 or 0: ")
    print(data)
    spi.writebytes([int(data)])
    print(int(data))
            