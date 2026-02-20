import socket
import time
import struct

#RPi_IP = "192.168.1.75"   # <-- change to your Pi IP
RPi_HOST = "RATLER"
PORT = 5000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    # Example motor values
    m1 = 50      # forward
    m2 = -30     # reverse
    m3 = 0
    m4 = 0
    buttons = 0b00000001

    packet = struct.pack("bbbbB", m1, m2, m3, m4, buttons)

    sock.sendto(packet, (RPi_HOST, PORT))

    time.sleep(1/60)   # 60 Hz