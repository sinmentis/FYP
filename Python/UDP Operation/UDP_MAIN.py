import socket
import time

if __name__ == '__main__':
    UDP_MASTER_IP = "192.168.1.26"
    UDP_PC_IP = "192.168.1.25"
    UDP_PORT = 5005
    MESSAGE = bytes(23)
    print("UDP target IP:", UDP_MASTER_IP)
    print("UDP target port:", UDP_PORT)
    print("message:", MESSAGE)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP    
    sock.bind((UDP_PC_IP, UDP_PORT)) 

    while True:
        data, addr = sock.recvfrom(100)  # buffer size is 1024 bytes    
        print("received message: {0}".format(data))
        sock.sendto(MESSAGE, (UDP_MASTER_IP, UDP_PORT))
        time.sleep(1)
        print("Send {0}".format(23))
