import socket

if __name__ == '__main__':
    UDP_IP = "192.168.1.25"

    UDP_PORT = 5005
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP

    sock.bind((UDP_IP, UDP_PORT)) 
    print("Init Done!\n")

    while True:
        data, addr = sock.recvfrom(100)  # buffer size is 1024 bytes
        print("received message: {0}".format(data))