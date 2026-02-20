import socket

UDP_IP = "192.168.1.75"
UDP_PORT = 5000

# Create a UDP socket
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

# Bind the socket to the port
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP packets on {UDP_IP}:{UDP_PORT}")

while True:
    # Receive data and client address
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print(f"Received message from {addr}: {data.decode('utf-8')}")
    # Optional: send a response back
    sock.sendto(b"Message received", addr)
