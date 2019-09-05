import socket
import select

"""=====================Head Define====================="""
DEBUG_MODE = True
UDP_RECEIVE_TIMEOUT = 1
LOOP_DELAY = 1

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
"""=====================Support functions====================="""


def init_UDP_connection():
    print("My IP is:     {0}, PORT: {1}".format(UDP_PC_IP, UDP_PC_PORT))
    print("Target IP is: {0}, PORT: {1}\n\n".format(UDP_MASTER_IP, UDP_MASTER_PORT))

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setblocking(0)
    sock.bind((UDP_PC_IP, UDP_PC_PORT))

    return sock


"""===================== MAIN ====================="""

if __name__ == '__main__':
    sock = init_UDP_connection()
    count = 0
    while True:
        ready = select.select([sock], [], [], UDP_RECEIVE_TIMEOUT)
        if ready[0]:
            data, _ = sock.recvfrom(80)  # buffer size is 1024 bytes
            print("PC: I just received message: [{0}]".format(data))
            sock.sendto(data, (UDP_MASTER_IP, UDP_MASTER_PORT))
            print("PC: I just Sent a [{0}]".format(data))
        else:
            print("{0}: Nothing Came through".format(count))
            count += 1
