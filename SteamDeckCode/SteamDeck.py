import socket

RPi_IP = "192.168.1.75"   # <-- put your Pi IP here
PORT = 5000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

message = "Hello from Steam Deck"
sock.sendto(message.encode(), (RPi_IP, PORT))

print("Message sent!")