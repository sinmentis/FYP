import socket
import time

if __name__ == '__main__':
    UDP_IP = "192.168.1.25"
    UDP_PORT = 5005
    MESSAGE = bytes(23)
    print("UDP target IP:", UDP_IP)
    print("UDP target port:", UDP_PORT)
    print("message:", MESSAGE)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
    i = 0
    while True:
        sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
        time.sleep(1)
        print("Send {0}".format(i))
        i += 1