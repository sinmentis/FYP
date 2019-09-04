import tkinter
from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.ttk import *


class CounterGui:

    def __init__(self, window):

        # Assign windows to self
        self.window = window

        # gloabl variables
        self.message = ["\n"]
        self.F =("Helvetica", 16) # font and size

        # Draw the GUI
        self.init_GUI()     # setting [title && geometry]
        self.add_menu()     # Adding  [main menu && About us]
        self.add_panel()    # Adding  [frame for panel && console].
        self.add_console()  # Adding  [Message console]

        # Start running GUI
        window.mainloop()

    """======================Draw the GUI======================"""

    def init_GUI(self):
        # Set title
        self.window.title("Emulator")

        # centralize the windows
        width_limit = (self.window.winfo_screenwidth() - 800) / 2
        height_limit = (self.window.winfo_screenheight() - 620) / 2
        self.window.geometry("800x620+{0:.0f}+{1:.0f}".format(width_limit, height_limit))

        # Fix the width and height
        self.window.resizable(False, False)

    def add_menu(self):
        # create a Top menu
        menubar = Menu(self.window)

        # create main menu
        mainmenu = Menu(menubar, tearoff=False)
        mainmenu.add_command(label='Setting', command=self.Menu_setting)
        mainmenu.add_command(label='Testing', command=self.Menu_testing)
        mainmenu.add_separator()
        mainmenu.add_command(label='Exit', command=self.window.destroy)
        menubar.add_cascade(label='Menu', menu=mainmenu)

        # create "About us"
        menubar.add_command(label='About us', command=self.Menu_about_us)

        # Start the menu
        self.window.config(menu=menubar)


    def add_console(self):

        # Adding labelFrame with hw:200*800
        self.console_frame = Labelframe(self.window, text="Message Console").place(x=10,y=405,height=200,width=780)

        # Adding message
        self.console_message = Message(self.console_frame, text = "\n\n\n\n\n\n\n\n\n",width=780)
        self.console_message.place(in_=self.console_frame,x=20, y=420)

    def add_panel(self):

        # Adding Frame with frame hw:600*800
        self.panel_frame = tkinter.Frame(height = 400,width = 800, bg = '#C0C0C0').pack(fill=BOTH, side=TOP, padx=15, pady=15)

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
        seg_label_L = Label(self.panel_frame, text="7 Seg: ").place(x=50, y= 30)
        self.seven_segs_L = Text(self.panel_frame, width=5, height=1)
        self.seven_segs_L.place(x=100, y= 30)

        poten_label_L = Label(self.panel_frame, text="Potentionmeter: ").place(x=200, y= 30)
        self.poten_L = Text(self.panel_frame, width=5, height=1)
        self.poten_L.place(x=300, y= 30)

        poten_label_R = Label(self.panel_frame, text="Potentionmeter: ").place(x=450, y= 30)
        self.poten_R = Text(self.panel_frame, width=5, height=1)
        self.poten_R.place(x=550, y= 30)

        seg_label_R = Label(self.panel_frame, text="7 Seg: ").place(x=650, y= 30)
        self.seven_segs_R = Text(self.panel_frame, width=5, height=1)
        self.seven_segs_R.place(x=700, y= 30)


    def add_dials(self):
        dial_label_L = Label(self.panel_frame, text="Dial 1", font=self.F).place(x=150, y= 100)
        dial_label_R = Label(self.panel_frame, text="Dial 2", font=self.F).place(x=550, y= 100)

        self.dial_L = Text(self.panel_frame, width=20, height=5)
        self.dial_L.place(x=100, y= 150)
        self.dial_R = Text(self.panel_frame, width=20, height=5)
        self.dial_R.place(x=500, y= 150)

    def add_leds(self):
        self.led_21_L = Label(self.panel_frame, text="2_pos L").place(x=30, y= 300)
        self.led_22_L = Label(self.panel_frame, text="2_pos R").place(x=80, y= 300)

        self.led_11_L = Label(self.panel_frame, text="1_pos").place(x=150, y= 300)

        self.led_1_L = Label(self.panel_frame, text="LED 1").place(x=220, y= 300)
        self.led_2_L = Label(self.panel_frame, text="LED 2").place(x=260, y= 300)
        self.led_3_L = Label(self.panel_frame, text="LED 3").place(x=300, y= 300)
        self.led_4_L = Label(self.panel_frame, text="LED 4").place(x=340, y= 300)

        # Symmetrical
        self.led_1_R = Label(self.panel_frame, text="LED 1").place(x=400, y= 300)
        self.led_2_R = Label(self.panel_frame, text="LED 2").place(x=440, y= 300)
        self.led_3_R = Label(self.panel_frame, text="LED 3").place(x=480, y= 300)
        self.led_4_R = Label(self.panel_frame, text="LED 4").place(x=520, y= 300)

        self.led_11_R = Label(self.panel_frame, text="1_pos").place(x=600, y= 300)

        self.led_21_R = Label(self.panel_frame, text="2_pos L").place(x=665, y= 300)
        self.led_22_R = Label(self.panel_frame, text="2_pos R").place(x=715, y= 300)

    def add_buttons(self):

        # Apply button
        self.apply = tkinter.Button(self.panel_frame, text="Send to Panel", height=1, width=12, relief=GROOVE, font=self.F, command=self.Apply)
        self.apply.place(x=310, y= 150)

        # 2 pos switch
        self.pos_L = IntVar()
        self.switch_11_L = tkinter.Radiobutton(self.panel_frame, text="Off", variable=self.pos_L, value=0)
        self.switch_11_L.place(x=60, y= 335)
        self.switch_12_L = tkinter.Radiobutton(self.panel_frame, text="On", variable=self.pos_L, value=1)
        self.switch_12_L.place(x=60, y= 360)

        # 1 pos switch
        self.switch_L_flag = 1
        self.switch_L = tkinter.Button(self.panel_frame, text="Off", height=1, width=4, relief=RAISED, command=self.Switch_L)
        self.switch_L.place(x=150, y= 335)

        # buttons
        self.button_1_L = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_1_L.place(x=220, y= 335)
        self.button_2_L = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_2_L.place(x=260, y= 335)
        self.button_3_L = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_3_L.place(x=300, y= 335)
        self.button_4_L = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_4_L.place(x=340, y= 335)

        # buttons
        self.button_1_R = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_1_R.place(x=400, y= 335)
        self.button_2_R = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_2_R.place(x=440, y= 335)
        self.button_3_R = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_3_R.place(x=480, y= 335)
        self.button_4_R = tkinter.Button(self.panel_frame, text="Off", height=1, width=4)
        self.button_4_R.place(x=520, y= 335)

        # 1 pos switch
        self.switch_R_flag = 1
        self.switch_R = tkinter.Button(self.panel_frame, text="Off", height=1, width=4, relief=RAISED, command=self.Switch_R)
        self.switch_R.place(x=600, y= 335)

        # 2 pos switch
        self.pos_R = IntVar()
        self.switch_11_R = tkinter.Radiobutton(self.panel_frame, text="Off", variable=self.pos_R, value=0)
        self.switch_11_R.place(x=690, y= 335)
        self.switch_12_R = tkinter.Radiobutton(self.panel_frame, text="On", variable=self.pos_R, value=1)
        self.switch_12_R.place(x=690, y= 360)

    """======================Detect the event and react======================"""
    """------ Buttons ------"""
    def Apply(self):
        # TODO: send UDP packet

        # Read from Text widget
        v_seg_L = str(self.seven_segs_L.get("1.0",END)).strip()
        v_seg_R = str(self.seven_segs_R.get("1.0",END)).strip()
        v_meter_L = str(self.poten_L.get("1.0",END)).strip()
        v_meter_R = str(self.poten_R.get("1.0",END)).strip()
        v_dial_L = str(self.dial_L.get("1.0",END)).strip()
        v_dial_R = str(self.dial_R.get("1.0",END)).strip()

        # Testing part: gather data and print out to console
        data_list = [v_seg_L, v_seg_R, v_meter_L, v_meter_R, v_dial_L, v_dial_R]
        data_name = ["Left 7 Seg", "Right 7 Seg", "Left meter", "Right meter", "Left Dial", "Right Dial"]
        for num in range(len(data_list)):
            self.message.append("{0:<10}: {1:<3}\n".format(data_name[num], data_list[num]))
        self.update_message()

    """------ 7 Segs && potentionmeter && dials------"""
    def Set_7seg_poten_dial(self, item, value):
        item['text'] = value

    """------ Switchs ------"""
    def Switch_L(self):
        self.switch_L_flag += 1
        if(self.switch_L_flag % 2 == 0):
            self.switch_L['relief']=SUNKEN
        else:
            self.switch_L['relief']=RAISED

    def Switch_R(self):
        self.switch_R_flag += 1
        if(self.switch_R_flag % 2 == 0):
            self.switch_R['relief']=SUNKEN
        else:
            self.switch_R['relief']=RAISED

    """------ Menus ------"""

    def Menu_testing(self):
        self.message.append("Testing\n")
        self.update_message()

    def Menu_setting(self):
        self.message.append("Setting\n")
        self.update_message()

    def Menu_about_us(self):
        showinfo(title="About us", message="This is E19")

    """------ Message Console ------"""
    def update_message(self):

        # when adding new line, remove old \n
        while("\n" in self.message):
            self.message.remove("\n")

        # Clear buffer limit the length to 10
        while(len(self.message) > 10):
            self.message = self.message[1:]

        # Adding \n if buffer is not 9
        while(len(self.message) <= 10):
            self.message += ["\n"]

        # Convert list into string
        total_message = ""
        for line in self.message:
            total_message += line

        # Update the message
        print(self.message)
        self.console_message.configure(text=total_message)


if __name__ == "__main__":
    window = Tk()
    CounterGui(window)