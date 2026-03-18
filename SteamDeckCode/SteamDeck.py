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
    x1 = int(joystick.get_axis(0)*127)
    y1 = int(joystick.get_axis(1)*127)
    x2 = int(joystick.get_axis(4)*127)
    y2 = int(joystick.get_axis(3)*127)
    L2 = int((joystick.get_axis(2)+1)*127)
    R2 = int((joystick.get_axis(5)+1)*127)
    dpad_x, dpad_y = joystick.get_hat(0) # D-pad
    buttons = 0
    buttons = buttons | (joystick.get_button(0) << 0) # A
    buttons = buttons | (joystick.get_button(1) << 1) # B
    buttons = buttons | (joystick.get_button(2) << 2) # X
    buttons = buttons | (joystick.get_button(3) << 3) # Y
    buttons = buttons | (joystick.get_button(4) << 4) # L1
    buttons = buttons | (joystick.get_button(5) << 5) # R1

    packet = struct.pack("bbbbBBbbB", x1, y1, x2, y2, L2, R2, dpad_x, dpad_y, buttons) # Assemble packet

    # Retry sending packet up to 3 times in case of temporary name resolution failure
    max_retries = 3
    for attempt in range(max_retries):
        try:
            sock.sendto(packet, (RPi_HOST, PORT)) # Send packet to Raspberry Pi
            break  # Success, exit retry loop
        except socket.gaierror as e:
            if attempt < max_retries - 1:
                print(f"Name resolution failed (attempt {attempt + 1}/{max_retries}): {e}. Retrying...")
                time.sleep(1)  # Short delay before retry
            else:
                print(f"Failed to send packet after {max_retries} attempts: {e}")
        except Exception as e:
            print(f"Unexpected error sending packet: {e}")
            break  # Don't retry for other errors

    time.sleep(1/60)   # 60 Hz