import tkinter
from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.ttk import *

"""Global Variables"""
F =("Helvetica", 16)

class CounterGui:
    """======================Construct the GUI in the given window======================"""

    def __init__(self, window):

        # Assign windows to self
        self.window = window

        # gloabl variables
        self.message = ["\n"]

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
        # Widget distance: {50}50{100}150{50}50{100}
        seg_label_1 = Label(self.panel_frame, text="7 Seg: ").place(x=50, y= 30)
        self.seven_segs_1 = Text(self.panel_frame, width=5, height=1).place(x=100, y= 30)

        poten_label_1 = Label(self.panel_frame, text="Potentionmeter: ").place(x=150, y= 30)
        self.poten_1 = Text(self.panel_frame, width=5, height=1).place(x=250, y= 30)

        poten_label_2 = Label(self.panel_frame, text="Potentionmeter: ").place(x=400, y= 30)
        self.poten_2 = Text(self.panel_frame, width=5, height=1).place(x=500, y= 30)

        seg_label_2 = Label(self.panel_frame, text="7 Seg: ").place(x=550, y= 30)
        self.seven_segs_2 = Text(self.panel_frame, width=5, height=1).place(x=600, y= 30)
        
        # dials
        dial_label_1 = Label(self.panel_frame, text="Dial 1", font=F).place(x=200, y= 100)
        dial_label_2 = Label(self.panel_frame, text="Dial 2", font=F).place(x=500, y= 100)

        self.dial_1 = Text(self.panel_frame, width=20, height=5).place(x=150, y= 150)
        self.dial_2 = Text(self.panel_frame, width=20, height=5).place(x=450, y= 150)

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

    """============= Menus ============="""

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