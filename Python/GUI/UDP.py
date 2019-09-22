#!/usr/bin/python3
# -*- coding:utf-8 -*-
import socket
import select
import time

"""=====================Head Define====================="""
UDP_RECEIVE_TIMEOUT = 1
LOOP_DELAY = 1


"""=====================Class====================="""


class UDP_packet:
    def __init__(self,board_info, board_add, state):
        self.board_type = int("{0:08b}".format(board_info)[:4], 2)
        self.board_num = int("{0:08b}".format(board_info)[4:], 2)
        self.board_add = board_add
        self.state = state

    def __str__(self):
        return "Type:{}, Num:{}, Addr:{}, State:{}".format(self.board_type, self.board_num, self.board_add, self.state)

    def __repr__(self):
        return "Type:{}, Num:{}, Addr:{}, State:{}".format(self.board_type, self.board_num, self.board_add, self.state)


"""=====================Support functions====================="""


def init_UDP_connection(DEBUG_MODE=False):
    if DEBUG_MODE:
        UDP_MASTER_IP = "127.0.0.2"
        UDP_MASTER_PORT = 5005
    
        UDP_PC_IP = "127.0.0.1"
        UDP_PC_PORT = 5006
    
    else:
        UDP_MASTER_IP = "192.168.1.26"
        UDP_MASTER_PORT = 5005
    
        UDP_PC_IP = "192.168.1.25"
        UDP_PC_PORT = 5005
    print("My IP is:     {0}, PORT: {1}\nTarget IP is: {0}, PORT: {1}".format(UDP_PC_IP, UDP_PC_PORT,UDP_MASTER_IP, UDP_MASTER_PORT))
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setblocking(0)
    sock.bind((UDP_PC_IP, UDP_PC_PORT))

    return sock


"""===================== MAIN ====================="""


def main(sock):

    data = b"HELLO"
    while True:
        ready = select.select([sock], [], [], UDP_RECEIVE_TIMEOUT)
        if ready[0]:
            data, _ = sock.recvfrom(80)  # buffer size is 1024 bytes
            print("PC: I just received message: [{0}]".format(data))
            sock.sendto(data, (UDP_MASTER_IP, UDP_MASTER_PORT))
            print("PC: I just Sent a [{0}]".format(data))
        else:
            sock.sendto(data, (UDP_MASTER_IP, UDP_MASTER_PORT))
            print("PC: I just Sent a [{0}]".format(data))


if __name__ == '__main__':
    sock = init_UDP_connection()
    main(sock)