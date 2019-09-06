import socket
import binascii
from package_class import package

def decdode_packet(data):
    packet = bin(int.from_bytes(str(data).encode(), 'little'))
    board_type = packet[14:]
    board_num = packet[11:14]
    board_add = packet[4:11]
    state = '0'+ packet[2:4]
    
    if board_type == '000':
        print("BOARD TYPE ----------- THREE STATE SWITCHED")    
    
    if board_type == '001':
        print("BOARD TYPE ----------- ROTARY ENCODER")    
    
    if board_type == '010':
        print("BOARD TYPE ----------- TWO STATE SWITCHED/BUTTONS")
    
    if board_type == '011':
        print("BOARD TYPE ----------- POTS")    

    if board_type == '100':
        print("BOARD TYPE ----------- LEDS")   

    if board_type == '101':
        print("BOARD TYPE ----------- 7 SEGS")   
        
    if board_type == '111':
        print("BOARD TYPE ----------- DIALS")  
    
    print("BOARD NUMBER --------- ", int(board_num, 2))
    print("HW ADDRESS ----------- ", int(board_add, 2))
    
    if board_type == '010':
        if state == '000':
            print("STATE ---------------- OFF")
            
        if state == '111':
            print("STATE ---------------- ON")

    print("------------------------------------------------------")
    print("------------------------------------------------------")

def fill_eight_digit(bit):
    """fill 0 in front of the bit to make len(bit)=8"""
    bit = list(bit)
    while len(bit) != 8:
        bit = ['0'] + bit
    return bit


def decode_packet(data):
    """Convert whatever we receved into package"""
    fir_bit = fill_eight_digit(bin(data[0])[2:])
    slave_num = int("".join(fir_bit[:4]), 2)
    board_type = int("".join(fir_bit[4:]), 2)
    address = int(data[1])
    state = int(data[2])
    UDP_package = package(slave_num, board_type, address, state)
    print("WOrking")
    print(UDP_package)



if __name__ == '__main__':
    UDP_IP = "192.168.137.25"

    UDP_PORT = 5005
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP

    sock.bind((UDP_IP, UDP_PORT))
    print("Init Done!\n")
    while True:
        data, addr = sock.recvfrom(32)  # buffer size is 1024 bytes
        decode_packet(data)
