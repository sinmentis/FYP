import threading
import select

import tkinter
from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.ttk import *

import Simulator_GUI
import UDP

if __name__ == "__main__":
    window = Tk()
    Simulator_GUI.Simulator_GUI(window)
    sock = UDP.init_UDP_connection()
    while True:
        ready = select.select([sock], [], [], 2)
        if not ready[0]:
            window.mainloop()
        else:
            UDP.main(sock)