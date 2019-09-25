#!/usr/bin/python3
# -*- coding:utf-8 -*-

import threading
import select
import time
import binascii

import tkinter
from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.ttk import *
from tkinter import messagebox

import UDP  # E19 UDP module

LOCAL_MODE = True
DEBUG_MODE = True
ENABLE_THREAD = False


class Simulator_GUI:

    def __init__(self, window, sock, UDP_MASTER_IP, UDP_MASTER_PORT):

        # Assign to self
        self.window = window
        self.sock = sock

        # global variables
        self.message = ["\n"]  # init message in console
        self.F = ("Helvetica", 16)  # Font of big words
        self.sF = ("Helvetica", 8)  # Font of hardware address
        self.hwbg = '#F8F8FF'  # Change color of HW text box
        self.defbg = '#C0C0C0'  # Default color
        self.ON_COLOR = "#FFD700"  # When LED is on
        self.OFF_COLOR = "#F0F0F0"  # When LED is off
        self.thread_quit_flag = False  # Easy to do clean up work

        self.UDP_MASTER_IP = UDP_MASTER_IP
        self.UDP_MASTER_PORT = UDP_MASTER_PORT

        # Deafult number is 1, due to noise from UART
        self.LED_ON = 2         # For Button and 2_state_switch and LED
        self.LED_OFF = 1        # For Button and 2_state_switch and LED
        self.STATE_L = 2        # For 3_state_switch
        self.STATE_0 = 1        # For 3_state_switch
        self.STATE_R = 3        # For 3_state_switch

        # Draw the GUI
        self.init_GUI()  # setting [title && geometry]
        self.add_menu()  # Adding  [main menu && About us]
        self.add_panel()  # Adding  [frame for panel && console].
        self.add_console()  # Adding  [Message console]
        self.add_setting()  # Adding  [Slave Configuration]

        # Read init state
        self.states = self.init_state()  # initial the state of the panel

        # Start running UDP
        if ENABLE_THREAD:
            receiver_thread = threading.Thread(target=self.receiver)
            receiver_thread.start()

        # When GUI about to quit, take over and run close_callback()
        window.protocol("WM_DELETE_WINDOW", self.close_callback)

        # Start the GUI
        window.mainloop()

    """====================== INIT ======================"""

    def init_GUI(self):

        # Set title
        self.window.title("This is a cool name : THE Emulator :D")

        # centralize the windows
        width_limit = (self.window.winfo_screenwidth() - 800) / 2
        height_limit = (self.window.winfo_screenheight() - 740) / 2
        self.window.geometry("800x740+{0:.0f}+{1:.0f}".format(width_limit, height_limit))

        # Fix the width and height
        self.window.resizable(False, False)

    """======================Draw the GUI======================"""

    def add_menu(self):

        # create a Top menu
        menubar = Menu(self.window)

        # create main menu
        mainmenu = Menu(menubar, tearoff=False)
        mainmenu.add_command(label='Testing', command=self.Menu_testing)
        mainmenu.add_separator()
        mainmenu.add_command(label='Exit', command=self.close_callback)
        menubar.add_cascade(label='Menu', menu=mainmenu)

        # create "About us"
        menubar.add_command(label='About us', command=self.Menu_about_us)

        # Start the menu
        self.window.config(menu=menubar)

    def add_console(self):

        # Adding labelFrame with hw:200*800
        self.console_frame = Labelframe(self.window, text="Message Console").place(x=10, y=400, height=210, width=800)

        # Adding message
        self.console_message = Message(self.console_frame, text="\n\n\n\n\n\n\n\n\n", width=780)
        self.console_message.place(in_=self.console_frame, x=20, y=420)

    def add_setting(self):

        # Basic grid start from x=20, y=620
        x_variable = 20
        y_variable = 620

        # Draw the label
        Label(self.panel_frame, text="Button Slave").place(x=x_variable + 100, y=y_variable)
        Label(self.panel_frame, text="LED Slave").place(x=x_variable + 200, y=y_variable)
        Label(self.panel_frame, text="Other Slave").place(x=x_variable + 300, y=y_variable)
        Label(self.panel_frame, text="Board Number").place(x=x_variable, y=y_variable + 30) # Type
        Label(self.panel_frame, text="Borad Type").place(x=x_variable, y=y_variable + 60)   # Number

        # Entrys: types
        self.but_type = Text(self.panel_frame, width=7, height=1, background=self.hwbg)
        self.but_type.insert(END, "0")
        self.but_type.place(x=x_variable + 100, y=y_variable + 30)
        self.led_type = Text(self.panel_frame, width=7, height=1, background=self.hwbg)
        self.led_type.insert(END, "0")
        self.led_type.place(x=x_variable + 200, y=y_variable + 30)
        self.oth_type = Text(self.panel_frame, width=7, height=1, background=self.hwbg)
        self.oth_type.insert(END, "0")
        self.oth_type.place(x=x_variable + 300, y=y_variable + 30)

        # Entrys: numbers
        self.but_numb = Text(self.panel_frame, width=7, height=1, background=self.hwbg)
        self.but_numb.insert(END, "0")
        self.but_numb.place(x=x_variable + 100, y=y_variable + 60)
        self.led_numb = Text(self.panel_frame, width=7, height=1, background=self.hwbg)
        self.led_numb.insert(END, "0")
        self.led_numb.place(x=x_variable + 200, y=y_variable + 60)
        self.oth_numb = Text(self.panel_frame, width=7, height=1, background=self.hwbg)
        self.oth_numb.insert(END, "0")
        self.oth_numb.place(x=x_variable + 300, y=y_variable + 60)

    def add_panel(self):

        # Adding Frame with frame hw:600*800
        self.panel_frame = tkinter.Frame(height=380, width=800, bg=self.defbg).pack(fill=BOTH, side=TOP, padx=15,
                                                                                    pady=15)

        # 7 segs and potentionmeter
        self.add_7seg_potentionmeters()

        # Dials
        self.add_dials()

        # LEDs
        self.add_leds()

        # Buttons
        self.add_buttons()

    def add_7seg_potentionmeters(self):

        # Widget distance: {50}50{100}150{50}50{100}
        y_variable = 30

        Label(self.panel_frame, text="7 Seg: ").place(x=50, y=y_variable)
        self.seven_segs_L = Text(self.panel_frame, width=5, height=1)
        self.seven_segs_L.insert(END, "0")
        self.seven_segs_L.place(x=100, y=y_variable)

        Label(self.panel_frame, text="Potentionmeter: ").place(x=200, y=y_variable)
        self.poten_L = tkinter.Label(self.panel_frame, text='0', width=5, height=1)
        self.poten_L.place(x=300, y=y_variable)

        Label(self.panel_frame, text="Potentionmeter: ").place(x=450, y=y_variable)
        self.poten_R = tkinter.Label(self.panel_frame, text='0', width=5, height=1)
        self.poten_R.place(x=550, y=y_variable)

        Label(self.panel_frame, text="7 Seg: ").place(x=650, y=y_variable)
        self.seven_segs_R = Text(self.panel_frame, width=5, height=1)
        self.seven_segs_R.insert(END, "0")
        self.seven_segs_R.place(x=700, y=y_variable)

        # Hardware address
        HW_label = Label(self.panel_frame, text="HW addr: ", background=self.defbg, font=self.sF).place(x=25,
                                                                                                        y=y_variable + 32)
        self.seven_segs_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.seven_segs_L_addr.insert(END, "0")
        self.seven_segs_L_addr.place(x=100, y=y_variable + 30)

        self.poten_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.poten_L_addr.insert(END, "0")
        self.poten_L_addr.place(x=300, y=y_variable + 30)

        self.poten_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.poten_R_addr.insert(END, "0")
        self.poten_R_addr.place(x=550, y=y_variable + 30)

        self.seven_segs_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.seven_segs_R_addr.insert(END, "0")
        self.seven_segs_R_addr.place(x=700, y=y_variable + 30)

    def add_dials(self):

        y_variable = 130

        Label(self.panel_frame, text="Dial 1", font=self.F).place(x=150, y=100)
        Label(self.panel_frame, text="Dial 2", font=self.F).place(x=550, y=100)

        self.dial_L = Text(self.panel_frame, width=20, height=3)
        self.dial_L.insert(END, "0")
        self.dial_L.place(x=100, y=y_variable)
        self.dial_R = Text(self.panel_frame, width=20, height=3)
        self.dial_R.insert(END, "0")
        self.dial_R.place(x=500, y=y_variable)

        # Apply button
        self.apply = tkinter.Button(self.panel_frame, text="Send to Panel", height=1, width=12, relief=GROOVE,
                                    font=self.F, command=self.Apply_button_handler)
        self.apply.place(x=310, y=y_variable)

        # Hardware address
        Label(self.panel_frame, text="HW addr: ", background=self.defbg, font=self.sF).place(x=25, y=y_variable + 60)
        self.dial_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.dial_L_addr.insert(END, "0")
        self.dial_L_addr.place(x=100, y=y_variable + 60)

        self.dial_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.dial_R_addr.insert(END, "0")
        self.dial_R_addr.place(x=500, y=y_variable + 60)

    def add_leds(self):

        y_variable = 230
        self.led_21_L = tkinter.Label(self.panel_frame, text="State 1")
        self.led_21_L.place(x=30, y=y_variable)
        self.led_22_L = tkinter.Label(self.panel_frame, text="State 2")
        self.led_22_L.place(x=80, y=y_variable)

        self.led_switch_L = tkinter.Label(self.panel_frame, text="1_pos")
        self.led_switch_L.place(x=150, y=y_variable)

        self.led_1_L_flag = 1
        self.led_1_L = tkinter.Label(self.panel_frame, text="LED 1")
        self.led_1_L.place(x=220, y=y_variable)
        self.led_2_L_flag = 1
        self.led_2_L = tkinter.Label(self.panel_frame, text="LED 2")
        self.led_2_L.place(x=260, y=y_variable)
        self.led_3_L_flag = 1
        self.led_3_L = tkinter.Label(self.panel_frame, text="LED 3")
        self.led_3_L.place(x=300, y=y_variable)
        self.led_4_L_flag = 1
        self.led_4_L = tkinter.Label(self.panel_frame, text="LED 4")
        self.led_4_L.place(x=340, y=y_variable)

        # Symmetrical
        self.led_1_R_flag = 1
        self.led_1_R = tkinter.Label(self.panel_frame, text="LED 1")
        self.led_1_R.place(x=400, y=y_variable)
        self.led_2_R_flag = 1
        self.led_2_R = tkinter.Label(self.panel_frame, text="LED 2")
        self.led_2_R.place(x=440, y=y_variable)
        self.led_3_R_flag = 1
        self.led_3_R = tkinter.Label(self.panel_frame, text="LED 3")
        self.led_3_R.place(x=480, y=y_variable)
        self.led_4_R_flag = 1
        self.led_4_R = tkinter.Label(self.panel_frame, text="LED 4")
        self.led_4_R.place(x=520, y=y_variable)

        self.led_switch_R = tkinter.Label(self.panel_frame, text="1_pos")
        self.led_switch_R.place(x=600, y=y_variable)

        self.led_21_R = 1
        self.led_21_R = tkinter.Label(self.panel_frame, text="State 2")
        self.led_21_R.place(x=665, y=y_variable)
        self.led_22_R = 1
        self.led_22_R = tkinter.Label(self.panel_frame, text="State 1")
        self.led_22_R.place(x=715, y=y_variable)

        # Hardware Address
        self.led_21_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_21_L_addr.insert(END, "0")
        self.led_21_L_addr.place(x=30, y=y_variable + 30)
        self.led_22_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_22_L_addr.insert(END, "0")
        self.led_22_L_addr.place(x=80, y=y_variable + 30)

        self.led_switch_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_switch_L_addr.insert(END, "0")
        self.led_switch_L_addr.place(x=150, y=y_variable + 30)

        self.led_1_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_1_L_addr.insert(END, "0")
        self.led_1_L_addr.place(x=220, y=y_variable + 30)
        self.led_2_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_2_L_addr.insert(END, "0")
        self.led_2_L_addr.place(x=260, y=y_variable + 30)
        self.led_3_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_3_L_addr.insert(END, "0")
        self.led_3_L_addr.place(x=300, y=y_variable + 30)
        self.led_4_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_4_L_addr.insert(END, "0")
        self.led_4_L_addr.place(x=340, y=y_variable + 30)

        # Symmetrical
        self.led_1_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_1_R_addr.insert(END, "0")
        self.led_1_R_addr.place(x=400, y=y_variable + 30)
        self.led_2_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_2_R_addr.insert(END, "0")
        self.led_2_R_addr.place(x=440, y=y_variable + 30)
        self.led_3_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_3_R_addr.insert(END, "0")
        self.led_3_R_addr.place(x=480, y=y_variable + 30)
        self.led_4_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_4_R_addr.insert(END, "0")
        self.led_4_R_addr.place(x=520, y=y_variable + 30)

        self.led_switch_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_switch_R_addr.insert(END, "0")
        self.led_switch_R_addr.place(x=600, y=y_variable + 30)

        self.led_21_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_21_R_addr.insert(END, "0")
        self.led_21_R_addr.place(x=665, y=y_variable + 30)
        self.led_22_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.led_22_R_addr.insert(END, "0")
        self.led_22_R_addr.place(x=715, y=y_variable + 30)

    def add_buttons(self):

        y_variable = 305

        # 2 pos switch
        self.pos_L = IntVar()
        self.pos_L.set(1)
        self.pos_L_1_led_flag = 1
        self.pos_L_2_led_flag = 1
        self.switch_1_L = tkinter.Radiobutton(self.panel_frame, text="State 1", variable=self.pos_L, value=2,
                                              command=self.Switch_2_L)
        self.switch_1_L.place(x=50, y=y_variable - 10)
        self.switch_0_L = tkinter.Radiobutton(self.panel_frame, text="State 0", variable=self.pos_L, value=1,
                                              command=self.Switch_2_L)
        self.switch_0_L.place(x=50, y=y_variable + 15)
        self.switch_2_L = tkinter.Radiobutton(self.panel_frame, text="State 2", variable=self.pos_L, value=3,
                                              command=self.Switch_2_L)
        self.switch_2_L.place(x=50, y=y_variable + 40)


        # 1 pos switch
        self.switch_L_flag = 1
        self.switch_L = tkinter.Button(self.panel_frame, text="OFF", height=1, width=4, relief=RAISED,
                                       command=self.Switch_L)
        self.switch_L.place(x=150, y=y_variable)

        # buttons
        self.button_1_L = tkinter.Button(self.panel_frame, text="1", height=1, width=4, command=self.but_1_L)
        self.button_1_L.place(x=220, y=y_variable)
        self.button_2_L = tkinter.Button(self.panel_frame, text="2", height=1, width=4, command=self.but_2_L)
        self.button_2_L.place(x=260, y=y_variable)
        self.button_3_L = tkinter.Button(self.panel_frame, text="3", height=1, width=4, command=self.but_3_L)
        self.button_3_L.place(x=300, y=y_variable)
        self.button_4_L = tkinter.Button(self.panel_frame, text="4", height=1, width=4, command=self.but_4_L)
        self.button_4_L.place(x=340, y=y_variable)

        # buttons
        self.button_1_R = tkinter.Button(self.panel_frame, text="1", height=1, width=4, command=self.but_1_R)
        self.button_1_R.place(x=400, y=y_variable)
        self.button_2_R = tkinter.Button(self.panel_frame, text="2", height=1, width=4, command=self.but_2_R)
        self.button_2_R.place(x=440, y=y_variable)
        self.button_3_R = tkinter.Button(self.panel_frame, text="3", height=1, width=4, command=self.but_3_R)
        self.button_3_R.place(x=480, y=y_variable)
        self.button_4_R = tkinter.Button(self.panel_frame, text="4", height=1, width=4, command=self.but_4_R)
        self.button_4_R.place(x=520, y=y_variable)

        # 1 pos switch
        self.switch_R_flag = 1
        self.switch_R = tkinter.Button(self.panel_frame, text="OFF", height=1, width=4, relief=RAISED,
                                       command=self.Switch_R)
        self.switch_R.place(x=600, y=y_variable)

        # 2 pos switch
        self.pos_R = IntVar()
        self.pos_R.set(1)
        self.pos_R_1_led_flag = 1
        self.pos_R_2_led_flag = 1
        self.switch_1_R = tkinter.Radiobutton(self.panel_frame, text="State 1", variable=self.pos_R, value=2,
                                              command=self.Switch_2_R)
        self.switch_1_R.place(x=680, y=y_variable - 10)
        self.switch_0_R = tkinter.Radiobutton(self.panel_frame, text="State 0", variable=self.pos_R, value=1,
                                              command=self.Switch_2_R)
        self.switch_0_R.place(x=680, y=y_variable + 15)
        self.switch_2_R = tkinter.Radiobutton(self.panel_frame, text="State 2", variable=self.pos_R, value=3,
                                              command=self.Switch_2_R)
        self.switch_2_R.place(x=680, y=y_variable + 40)


        # Hardware Address

        self.switch_1_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.switch_1_L_addr.insert(END, "0")
        self.switch_1_L_addr.place(x=50, y=y_variable + 60)

        self.switch_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.switch_L_addr.insert(END, "0")
        self.switch_L_addr.place(x=150, y=y_variable + 60)

        self.button_1_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_1_L_addr.insert(END, "0")
        self.button_1_L_addr.place(x=220, y=y_variable + 60)
        self.button_2_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_2_L_addr.insert(END, "0")
        self.button_2_L_addr.place(x=260, y=y_variable + 60)
        self.button_3_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_3_L_addr.insert(END, "0")
        self.button_3_L_addr.place(x=300, y=y_variable + 60)
        self.button_4_L_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_4_L_addr.insert(END, "0")
        self.button_4_L_addr.place(x=340, y=y_variable + 60)

        self.button_1_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_1_R_addr.insert(END, "0")
        self.button_1_R_addr.place(x=400, y=y_variable + 60)
        self.button_2_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_2_R_addr.insert(END, "0")
        self.button_2_R_addr.place(x=440, y=y_variable + 60)
        self.button_3_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_3_R_addr.insert(END, "0")
        self.button_3_R_addr.place(x=480, y=y_variable + 60)
        self.button_4_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.button_4_R_addr.insert(END, "0")
        self.button_4_R_addr.place(x=520, y=y_variable + 60)

        self.switch_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.switch_R_addr.insert(END, "0")
        self.switch_R_addr.place(x=600, y=y_variable + 60)

        self.switch_1_R_addr = Text(self.panel_frame, width=4, height=1, background=self.hwbg)
        self.switch_1_R_addr.insert(END, "0")
        self.switch_1_R_addr.place(x=680, y=y_variable + 60)

    """======================Detect the event and react======================"""

    """------ Buttons ------"""

    def Apply_button_handler(self):
        self.Update_message("Applied!\n")

        # It's time to send the UDP
        self.sender()

    def but_1_L(self):
        self.led_1_L_flag = self.LED_toggle(self.led_1_L, self.led_1_L_flag)
        self.sender()

    def but_2_L(self):
        self.led_2_L_flag = self.LED_toggle(self.led_2_L, self.led_2_L_flag)
        self.sender()

    def but_3_L(self):
        self.led_3_L_flag = self.LED_toggle(self.led_3_L, self.led_3_L_flag)
        self.sender()

    def but_4_L(self):
        self.led_4_L_flag = self.LED_toggle(self.led_4_L, self.led_4_L_flag)
        self.sender()

    def but_1_R(self):
        self.led_1_R_flag = self.LED_toggle(self.led_1_R, self.led_1_R_flag)
        self.sender()

    def but_2_R(self):
        self.led_2_R_flag = self.LED_toggle(self.led_2_R, self.led_2_R_flag)
        self.sender()

    def but_3_R(self):
        self.led_3_R_flag = self.LED_toggle(self.led_3_R, self.led_3_R_flag)
        self.sender()

    def but_4_R(self):
        self.led_4_R_flag = self.LED_toggle(self.led_4_R, self.led_4_R_flag)
        self.sender()

    """------ LEDs ------"""

    def LED_toggle(self, item, flag):

        flag -= 1
        if (flag % 2) == 0:
            item.config(bg=self.ON_COLOR)
            self.Update_message("LED ON\n")
        else:
            item.config(bg=self.OFF_COLOR)
            self.Update_message("LED OFF\n")
        flag = (1 + flag) % 2 + 1

        return flag

    """------ Switchs ------"""

    def Switch_L(self):

        flag = self.switch_L_flag
        if (flag % 2 == 1):
            self.switch_L['relief'] = SUNKEN
            self.led_switch_L['bg'] = self.ON_COLOR
            self.switch_L['text'] = "ON"
        else:
            self.switch_L['relief'] = RAISED
            self.led_switch_L['bg'] = self.OFF_COLOR
            self.switch_L['text'] = "OFF"
        self.switch_L_flag = flag % 2 + 1
        self.sender()

    def Switch_R(self):

        flag = self.switch_R_flag
        if (flag % 2 == 1):
            self.switch_R['relief'] = SUNKEN
            self.led_switch_R['bg'] = self.ON_COLOR
            self.switch_R['text'] = "ON"
        else:
            self.switch_R['relief'] = RAISED
            self.led_switch_R['bg'] = self.OFF_COLOR
            self.switch_R['text'] = "OFF"
        self.switch_R_flag = flag % 2 + 1

        self.sender()

    def Switch_2_R(self):
        flag = self.pos_R.get()
        if flag == 3:
            self.led_21_R.config(bg=self.ON_COLOR)
            self.led_22_R.config(bg=self.OFF_COLOR)
            self.pos_R_1_led_flag = 2
            self.pos_R_2_led_flag = 1
        elif flag == 2:
            self.led_21_R.config(bg=self.OFF_COLOR)
            self.led_22_R.config(bg=self.ON_COLOR)
            self.pos_R_1_led_flag = 1
            self.pos_R_2_led_flag = 2
        else:
            self.led_21_R.config(bg=self.OFF_COLOR)
            self.led_22_R.config(bg=self.OFF_COLOR)
            self.pos_R_1_led_flag = 1
            self.pos_R_2_led_flag = 1
        self.sender()

    def Switch_2_L(self):
        flag = self.pos_L.get()
        if flag == 3:
            self.led_22_L.config(bg=self.ON_COLOR)
            self.led_21_L.config(bg=self.OFF_COLOR)
            self.pos_L_1_led_flag = 2
            self.pos_L_2_led_flag = 1
        elif flag == 2:
            self.led_22_L.config(bg=self.OFF_COLOR)
            self.led_21_L.config(bg=self.ON_COLOR)
            self.pos_L_1_led_flag = 1
            self.pos_L_2_led_flag = 2
        else:
            self.led_22_L.config(bg=self.OFF_COLOR)
            self.led_21_L.config(bg=self.OFF_COLOR)
            self.pos_L_1_led_flag = 1
            self.pos_L_2_led_flag = 1
        self.sender()

    """------ Menus ------"""

    def Menu_testing(self):
        """TODO: Fill hardware address in there"""

        self.poten_L_addr.delete('1.0', END)
        self.poten_L_addr.insert(END, "51")
        self.poten_R_addr.delete('1.0', END)
        self.poten_R_addr.insert(END, "52")

        self.seven_segs_L_addr.delete('1.0', END)
        self.seven_segs_L_addr.insert(END, "50")
        self.seven_segs_R_addr.delete('1.0', END)
        self.seven_segs_R_addr.insert(END, "53")

        self.dial_L_addr.delete('1.0', END)
        self.dial_L_addr.insert(END, "54")
        self.dial_R_addr.delete('1.0', END)
        self.dial_R_addr.insert(END, "55")

        self.button_1_L_addr.delete('1.0', END)
        self.button_1_L_addr.insert(END, "11")
        self.button_2_L_addr.delete('1.0', END)
        self.button_2_L_addr.insert(END, "12")
        self.button_3_L_addr.delete('1.0', END)
        self.button_3_L_addr.insert(END, "13")
        self.button_4_L_addr.delete('1.0', END)
        self.button_4_L_addr.insert(END, "14")

        self.button_1_R_addr.delete('1.0', END)
        self.button_1_R_addr.insert(END, "15")
        self.button_2_R_addr.delete('1.0', END)
        self.button_2_R_addr.insert(END, "16")
        self.button_3_R_addr.delete('1.0', END)
        self.button_3_R_addr.insert(END, "17")
        self.button_4_R_addr.delete('1.0', END)
        self.button_4_R_addr.insert(END, "18")

        self.led_1_L_addr.delete('1.0', END)
        self.led_1_L_addr.insert(END, "21")
        self.led_2_L_addr.delete('1.0', END)
        self.led_2_L_addr.insert(END, "22")
        self.led_3_L_addr.delete('1.0', END)
        self.led_3_L_addr.insert(END, "23")
        self.led_4_L_addr.delete('1.0', END)
        self.led_4_L_addr.insert(END, "24")

        self.led_1_R_addr.delete('1.0', END)
        self.led_1_R_addr.insert(END, "25")
        self.led_2_R_addr.delete('1.0', END)
        self.led_2_R_addr.insert(END, "26")
        self.led_3_R_addr.delete('1.0', END)
        self.led_3_R_addr.insert(END, "27")
        self.led_4_R_addr.delete('1.0', END)
        self.led_4_R_addr.insert(END, "28")

        self.switch_1_L_addr.delete('1.0', END)
        self.switch_1_L_addr.insert(END, "25")

        self.Update_message("Loaded\n")

    def Menu_about_us(self):
        showinfo(title="About us", message="This is E19")

    def close_callback(self):
        self.thread_quit_flag = True  # Clean up work for UDP Thread
        tkinter.messagebox.showinfo('~~~~~Hello World~~~~~', "GUESS WHAT! I am about to quit!!")
        self.window.destroy()  # Quit GUI

    """====================== Update ======================"""

    def Update_message(self, new_message=None):

        # Adding new message into self
        if new_message:
            self.message += [new_message]

        # when adding new line, remove old \n
        while ("\n" in self.message):
            self.message.remove("\n")

        # Clear buffer limit the length to 10
        if (len(self.message) > 11):
            self.message = self.message[-11:]
        else:
            self.message += ["\n"] * (len(self.message) - 11)

        # Convert list into string
        total_message = ""
        for line in self.message:
            total_message += line

        # Update the message
        self.console_message.configure(text=total_message)

    def Update_state_from_packet(self, packet):
        """Update self.state based on packet"""
        for key, value in self.states.items():
            if int(value.board_add) == int(packet.board_add):
                if DEBUG_MODE:
                    self.Update_message("match!\n")
                self.states[key].state = packet.state  # update the state
                break
        return 0

    def Update_states_from_GUI(self):
        """Update state's structure into new dir with class"""

        states = self.init_state()
        states["seg_L"].board_add = int(self.seven_segs_L_addr.get("1.0", END).strip())
        states["seg_L"].state = int(self.seven_segs_L.get("1.0", END).strip())
        states["seg_R"].board_add = int(self.seven_segs_R_addr.get("1.0", END).strip())
        states["seg_R"].state = int(self.seven_segs_R.get("1.0", END).strip())

        states["dial_L"].board_add = int(self.dial_L_addr.get("1.0", END).strip())
        states["dial_L"].state = int(self.dial_L.get("1.0", END).strip())
        states["dial_R"].board_add = int(self.dial_R_addr.get("1.0", END).strip())
        states["dial_R"].state = int(self.dial_R.get("1.0", END).strip())

        states["pot_L"].board_add = int(self.poten_L_addr.get("1.0", END).strip())
        states["pot_L"].state = int(self.poten_L['text'])
        states["pot_R"].board_add = self.poten_R_addr.get("1.0", END).strip()
        states["pot_R"].state = int(self.poten_R['text'])

        states["led_s_L"].board_add = int(self.led_switch_L_addr.get("1.0", END).strip())
        states["led_s_L"].state = int(self.switch_L_flag)
        states["led_s_R"].board_add = int(self.led_switch_R_addr.get("1.0", END).strip())
        states["led_s_R"].state = int(self.switch_R_flag)

        states["led_21_L"].board_add = int(self.led_21_L_addr.get("1.0", END).strip())
        states["led_21_L"].state = int(self.pos_L_1_led_flag)
        states["led_21_R"].board_add = int(self.led_21_R_addr.get("1.0", END).strip())
        states["led_21_R"].state = int(self.pos_R_1_led_flag)
        states["led_22_L"].board_add = int(self.led_22_L_addr.get("1.0", END).strip())
        states["led_22_L"].state = int(self.pos_L_2_led_flag)
        states["led_22_R"].board_add = int(self.led_22_R_addr.get("1.0", END).strip())
        states["led_22_R"].state = int(self.pos_R_2_led_flag)

        states["led_1L"].board_add = int(self.led_1_L_addr.get("1.0", END).strip())
        states["led_1L"].state = int(self.led_1_L_flag)
        states["led_2L"].board_add = int(self.led_2_L_addr.get("1.0", END).strip())
        states["led_2L"].state = int(self.led_2_L_flag)
        states["led_3L"].board_add = int(self.led_3_L_addr.get("1.0", END).strip())
        states["led_3L"].state = int(self.led_3_L_flag)
        states["led_4L"].board_add = int(self.led_4_L_addr.get("1.0", END).strip())
        states["led_4L"].state = int(self.led_4_L_flag)

        states["led_1R"].board_add = int(self.led_1_R_addr.get("1.0", END).strip())
        states["led_1R"].state = int(self.led_1_R_flag)
        states["led_2R"].board_add = int(self.led_2_R_addr.get("1.0", END).strip())
        states["led_2R"].state = int(self.led_2_R_flag)
        states["led_3R"].board_add = int(self.led_3_R_addr.get("1.0", END).strip())
        states["led_3R"].state = int(self.led_3_R_flag)
        states["led_4R"].board_add = int(self.led_4_R_addr.get("1.0", END).strip())
        states["led_4R"].state = int(self.led_4_R_flag)

        for hardware in ["seg_L", "seg_R", "dial_L", "dial_R", "pot_L", "pot_R"]:
            states[hardware].board_type = int(self.oth_type.get("1.0", END).strip())
            states[hardware].board_num = int(self.oth_numb.get("1.0", END).strip())

        for hardware in ["led_1L", "led_2L", "led_3L", "led_4L", "led_1R", "led_2R", "led_3R", "led_4R", "led_21_L", "led_21_R", "led_22_L", "led_22_R", "led_s_L", "led_s_R"]:
            states[hardware].board_type = int(self.led_type.get("1.0", END).strip())
            states[hardware].board_num = int(self.led_numb.get("1.0", END).strip())

        for hardware in ["swh_2L", "swh_2R", "swh_1L", "swh_1R"]:
            states[hardware].board_type = int(self.but_type.get("1.0", END).strip())
            states[hardware].board_num = int(self.but_numb.get("1.0", END).strip())

        return states

    def Update_GUI(self):
        """Update GUI based on new self.states"""

        # 7 Segs
        self.seven_segs_L.delete('1.0', END)
        self.seven_segs_L.insert(END, self.states["seg_L"].state)
        self.seven_segs_R.delete('1.0', END)
        self.seven_segs_R.insert(END, self.states["seg_R"].state)

        # Dials
        self.dial_L.delete('1.0', END)
        self.dial_L.insert(END, self.states["dial_L"].state)
        self.dial_R.delete('1.0', END)
        self.dial_R.insert(END, self.states["dial_R"].state)

        # Pots
        self.poten_L['text'] = self.states["pot_L"].state
        self.poten_R['text'] = self.states["pot_R"].state

        # Buttons
        if self.states["but_1L"].state == self.LED_ON:
            self.but_1_L()
            self.states["but_1L"].state = self.LED_OFF

        if self.states["but_2L"].state == self.LED_ON:
            self.but_2_L()
            self.states["but_2L"].state = self.LED_OFF

        if self.states["but_3L"].state == self.LED_ON:
            self.but_3_L()
            self.states["but_3L"].state = self.LED_OFF

        if self.states["but_4L"].state == self.LED_ON:
            self.but_4_L()
            self.states["but_4L"].state = self.LED_OFF

        if self.states["but_1R"].state == self.LED_ON:
            self.but_1_R()
            self.states["but_1R"].state = self.LED_OFF

        if self.states["but_2R"].state == self.LED_ON:
            self.but_2_R()
            self.states["but_2R"].state = self.LED_OFF

        if self.states["but_3R"].state == self.LED_ON:
            self.but_3_R()
            self.states["but_3R"].state = self.LED_OFF

        if self.states["but_4R"].state == self.LED_ON:
            self.but_4_R()
            self.states["but_4R"].state = self.LED_OFF

        # 3 states switches
        if self.states["swh_2L"] == self.STATE_0:
            self.switch_0_L.invoke()
        elif self.states["swh_2L"] == self.STATE_L:
            self.switch_1_L.invoke()
        elif self.states["swh_2L"] == self.STATE_R:
            self.switch_2_L.invoke()

        if self.states["swh_2R"] == self.STATE_0:
            self.switch_0_R.invoke()
        elif self.states["swh_2R"] == self.STATE_L:
            self.switch_1_R.invoke()
        elif self.states["swh_2R"] == self.STATE_R:
            self.switch_2_R.invoke()

        # 2 states switches
        if self.states["swh_1L"] != self.switch_L_flag:
            self.switch_L.invoke()
        if self.states["swh_1R"] == self.switch_R_flag:
            self.switch_R.invoke()

    """====================== States ======================"""

    def init_state(self):
        """states = {hardware_name:(board_type, board_num, board_addr, state)}"""

        states = {"seg_L": UDP.UDP_packet(0, 0, 0), "seg_R": UDP.UDP_packet(0, 0, 0), \
                  "dial_L": UDP.UDP_packet(0, 0, 0), "dial_R": UDP.UDP_packet(0, 0, 0), \
                  "pot_L": UDP.UDP_packet(0, 0, 0), "pot_R": UDP.UDP_packet(0, 0, 0), \
                  "swh_2L": UDP.UDP_packet(0, 0, 1), "swh_2R": UDP.UDP_packet(0, 0, 1), \
                  "swh_1L": UDP.UDP_packet(0, 0, 1), "swh_1R": UDP.UDP_packet(0, 0, 1), \
                  "led_s_L": UDP.UDP_packet(0, 0, 1), "led_s_R": UDP.UDP_packet(0, 0, 1), \
                  "led_21_L": UDP.UDP_packet(0, 0, 1), "led_21_R": UDP.UDP_packet(0, 0, 1), \
                  "led_22_L": UDP.UDP_packet(0, 0, 1), "led_22_R": UDP.UDP_packet(0, 0, 1), \
                  "led_1L": UDP.UDP_packet(0, 0, 1), "led_2L": UDP.UDP_packet(0, 0, 1),
                  "led_3L": UDP.UDP_packet(0, 0, 1), "led_4L": UDP.UDP_packet(0, 0, 1), \
                  "led_1R": UDP.UDP_packet(0, 0, 1), "led_2R": UDP.UDP_packet(0, 0, 1),
                  "led_3R": UDP.UDP_packet(0, 0, 1), "led_4R": UDP.UDP_packet(0, 0, 1), \
                  "but_1L": UDP.UDP_packet(0, 0, 0), "but_2L": UDP.UDP_packet(0, 0, 0),
                  "but_3L": UDP.UDP_packet(0, 0, 0), "but_4L": UDP.UDP_packet(0, 0, 0), \
                  "but_1R": UDP.UDP_packet(0, 0, 0), "but_2R": UDP.UDP_packet(0, 0, 0),
                  "but_3R": UDP.UDP_packet(0, 0, 0), "but_4R": UDP.UDP_packet(0, 0, 0),}

        return states

    """====================== UDPLED_toggle operation ======================"""

    def receiver(self):

        while not self.thread_quit_flag:
            ready = select.select([self.sock], [], [], 0.01)
            packet_list = []
            if ready[0]:
                data, _ = self.sock.recvfrom(30)  # buffer size is 1024 bytes
                if DEBUG_MODE:
                    print("UDP:got {}, type:{}".format(data, type(data)))
                packet_list = self.decode_packet(data)  # Get packet from received data
                self.states = self.Update_states_from_GUI()
                for packet in packet_list:
                    if DEBUG_MODE:
                        self.Update_message(str(packet) + '\n')
                    #if delay_test(packet):                     # If this is not a delay test packet
                    self.Update_state_from_packet(packet)  # Update self.states based on packet
                        #self.Update_GUI()
                self.Update_GUI()  # Update GUI based on new self.states
                # TODO: Test performance of where sould Update_GUI() goes.
                
        return 0

    def sender(self):

        # Check the changging state
        udp_packet_list = self.generate_packet()

        if len(udp_packet_list) != 0:  # 
            for packet in udp_packet_list:
                try:
                    self.sock.sendto(packet, (self.UDP_MASTER_IP, self.UDP_MASTER_PORT))
                    self.Update_message("UDP Sent:{0}\n".format(packet))
                except Exception as e:
                    self.Update_message("{0}\n".format(e))
                
        if DEBUG_MODE:
            print("\nCurrent state: ")
            for key, value in self.states.items():
                print('item: {}: {}'.format(key, value))

    def generate_packet(self):
        """generate new packet based on old state and new GUI"""
        # Read new state
        new_states = self.Update_states_from_GUI()  # Read from GUI to self.states
        difference_states = {}  # Empty for whatever changed
        packet_list = []  # Store all UDP message

        # Compare with old state
        for component in self.states.keys():
            if self.states[component].state != new_states[component].state:
                difference_states[component] = new_states[component]

        if DEBUG_MODE:
            print(difference_states)

        if len(difference_states) != 0:
            for key, value in difference_states.items():
                new_packet = self.encode_packet(value)
                packet_list.append(new_packet)

            # Assign new state to old state
            self.states = new_states

        return packet_list

    def decode_packet(self, data):
        packet_list = []
        index = 1

        # decode what we got into packet.
        for index in range(len(data)):
            data_slices = data[index * 3:index * 3 + 3]
            if data_slices != b'' and data_slices != b"\x00\x00\x00":
                packet = UDP.UDP_packet(data_slices[0], data_slices[1], data_slices[2])
                packet_list.append(packet)
        return packet_list

    def encode_packet(self, value):
        """Construct the packet
        packet = board_type board_num board_add board_state"""

        info_binary_str = '{0:004b}'.format(value.board_type) + '{0:004b}'.format(value.board_num)
        info_dec_int = int(info_binary_str, 2)

        dec_list = [info_dec_int, value.board_add, value.state]
        dec_packet = bytes(dec_list)

        if DEBUG_MODE == 567:
            info_ascii = chr(info_dec_int)
            add_ascii = chr(value.board_add)
            state_ascii = chr(value.state)
            print("encode_packet info: Ascii:{}, dec:{}, binary:{}".format(info_ascii, info_dec_int, info_binary_str))
            print("encode_packet address: Ascii:{}, dec:{}".format(add_ascii, value.board_add))
            print("encode_packet state: Ascii:{}, dec:{}".format(state_ascii, value.state))
            print("encode_packet: Final Ascii:{}, len:{}".format(dec_packet, len(dec_packet)))

        return dec_packet

    def delay_test(self, packet):
        """If type and number == 64, send echo back to where it came from"""
        if packet.board_num == packet.board_type == 15 and packet.board_add == packet.state == 255:
            packet = self.encode_packet(UDP.UDP_packet(255, 255, 255))
            self.sock.sendto(packet, (self.UDP_MASTER_I,P, self.UDP_MASTER_PORT))
            return False
        else:
            return True

if __name__ == "__main__":
    sock, UDP_MASTER_IP, UDP_MASTER_PORT = UDP.init_UDP_connection(LOCAL_MODE)
    window = Tk()
    Simulator_GUI(window, sock, UDP_MASTER_IP, UDP_MASTER_PORT)
