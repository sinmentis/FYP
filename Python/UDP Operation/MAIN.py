import socket
import time
import select

if __name__ == '__main__':
    UDP_MASTER_IP = "192.168.1.26"
    UDP_PC_IP = "192.168.1.25"
    UDP_PORT = 5005
    print("UDP target IP:", UDP_MASTER_IP)
    print("UDP target port:", UDP_PORT)
    time.sleep(0.1)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP   
    sock.settimeout(1)
    sock.bind((UDP_PC_IP, UDP_PORT)) 
    print("{}GO{}".format("*"*20,"*"*20))
    
    last_data = ""
    
    while True:
        data, addr = sock.recvfrom(80)  # buffer size is 1024 bytes
        if last_data != data:
            print("PC: I just received message: [{0}]".format(data))
            sock.sendto(bytes(data), (UDP_MASTER_IP, UDP_PORT))
            print("PC: I just Sent a [{0}]".format(data))
        last_data = data
        data = ""
        time.sleep(2)