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
        flag = raw_input("What NOW!?\n")
        if flag == "R":
            data, addr = sock.recvfrom(100)  # buffer size is 1024 bytes
            message = data.rstrip('\x00')
            print("PC: I just received message: [{0}]".format(message[:2]))
        else:
            sock.sendto(bytes(flag), (UDP_MASTER_IP, UDP_PORT))
            print("PC: I just Sent a [{0}]".format(flag))
            