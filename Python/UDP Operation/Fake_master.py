import socket
import select

"""=====================Head Define====================="""
UDP_RECEIVE_TIMEOUT = 1
LOOP_DELAY = 1
UDP_MASTER_IP = "127.0.0.2"
UDP_MASTER_PORT = 5005

UDP_PC_IP = "127.0.0.1"
UDP_PC_PORT = 5006

"""=====================Support functions====================="""


def init_UDP_connection():
    print("My IP is:     {0}, PORT: {1}".format(UDP_MASTER_IP, UDP_MASTER_PORT))
    print("Target IP is: {0}, PORT: {1}\n\n".format(UDP_PC_IP, UDP_PC_PORT))

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setblocking(0)
    sock.bind((UDP_MASTER_IP, UDP_MASTER_PORT))
    return sock


"""===================== MAIN ====================="""


if __name__ == '__main__':
    sock = init_UDP_connection()
    while True:
        command = input("Command (R for receive): ")
        if command == "R":
            data, _ = sock.recvfrom(80)  # buffer size is 1024 bytes
            print("Fake Master: I just received message: [{0}]".format(data))
        else:
            sock.sendto(str.encode(command), (UDP_PC_IP, UDP_PC_PORT))
            print("Fake Master: I just Sent a [{0}]".format(command))
