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
        self.window.title("Pilot training Emulator")

        # centralize the windows
        width_limit = (self.window.winfo_screenwidth() - 800) / 2
        height_limit = (self.window.winfo_screenheight() - 700) / 2
        self.window.geometry("800x800+{0:.0f}+{1:.0f}".format(width_limit, height_limit))  

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
        self.console_frame = Labelframe(self.window, text="Message Console",height = 200,width = 800)
        self.console_frame.pack(fill="x", padx=5, pady=5)

        # Adding message
        self.console_message = Message(self.console_frame, text = "\n\n\n\n\n\n\n\n\n\n", width=800)
        self.console_message.pack(side='left')


    def add_panel(self):

        # Adding Frame with frame hw:600*800
        self.panel_frame = tkinter.Frame(height = 500,width = 800, bg = '#C0C0C0')
        self.panel_frame.pack(fill=BOTH, side=TOP, padx=15, pady=15)

        # 7 segs and potentionmeter
        self.add_7seg_potentionmeters()

        # dials
        self.add_dials()

        # LEDs
        self.add_leds()


    def add_7seg_potentionmeters(self):
        # Widget distance: {50}50{100}150{50}50{100}  
        seg_label_1 = Label(self.panel_frame, text="7 Seg: ").place(x=50, y= 30)
        self.seven_segs_1 = Text(self.panel_frame, width=5, height=1).place(x=100, y= 30)

        poten_label_1 = Label(self.panel_frame, text="Potentionmeter: ").place(x=200, y= 30)
        self.poten_1 = Text(self.panel_frame, width=5, height=1).place(x=300, y= 30)

        poten_label_2 = Label(self.panel_frame, text="Potentionmeter: ").place(x=450, y= 30)
        self.poten_2 = Text(self.panel_frame, width=5, height=1).place(x=550, y= 30)

        seg_label_2 = Label(self.panel_frame, text="7 Seg: ").place(x=650, y= 30)
        self.seven_segs_2 = Text(self.panel_frame, width=5, height=1).place(x=700, y= 30)


    def add_dials(self):
        dial_label_1 = Label(self.panel_frame, text="Dial 1", font=self.F).place(x=150, y= 100)
        dial_label_2 = Label(self.panel_frame, text="Dial 2", font=self.F).place(x=550, y= 100)

        self.dial_1 = Text(self.panel_frame, width=20, height=5).place(x=100, y= 150)
        self.dial_2 = Text(self.panel_frame, width=20, height=5).place(x=500, y= 150)

    def add_leds(self):
        self.led_21_L = Label(self.panel_frame, text="2_pos L").place(x=10, y= 300)
        self.led_22_L = Label(self.panel_frame, text="2_pos R").place(x=60, y= 300)

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


        """======================Detect the event and react======================"""

    def update_message(self):

        # when adding new line, remove old \n
        while("\n" in self.message):
            self.message.remove("\n")

        # Clear buffer limit the length to 10
        if(len(self.message) > 9):
            self.message = self.message[1:]

        # Adding \n if buffer is not 9
        while(len(self.message) <= 9):
            self.message += ["\n"]

        # Convert list into string
        total_message = ""
        for line in self.message:
            total_message += line

        # Update the message
        self.console_message.configure(text=total_message)

    """------ Menus ------"""

    def Menu_testing(self):
        self.message.append("Testing\n")
        self.update_message()

    def Menu_setting(self):
        self.message.append("Setting\n")
        self.update_message()

    def Menu_about_us(self):
        showinfo(title="About us", message="This is E19")


if __name__ == "__main__":
    window = Tk()
    CounterGui(window)