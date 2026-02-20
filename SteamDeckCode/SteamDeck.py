import socket
import pygame
import time
import struct

# Initialize UDP client
#RPi_IP = "192.168.1.75"   # <-- change to your Pi IP
RPi_HOST = "RATLER"
PORT = 5000
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Initialize pygame and the joystick module
pygame.init()
pygame.joystick.init()

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"Detected Joystick: {joystick.get_name()}")

while True:

    pygame.event.pump()

    # Example stick values
    x1 = joystick.get_axis(0)
    y1 = joystick.get_axis(1)
    x2 = joystick.get_axis(2)
    y2 = joystick.get_axis(3)
    buttons = 0b00000001

    packet = struct.pack("bbbbB", x1, y1, x2, y2, buttons)

    sock.sendto(packet, (RPi_HOST, PORT))

    time.sleep(1/60)   # 60 Hz