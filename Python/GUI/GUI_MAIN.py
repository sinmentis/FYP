import tkinter
from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.ttk import *


class CounterGui:
    """======================Construct the GUI in the given window======================"""

    def __init__(self, window):

        # Assign windows to self
        self.window = window

        # gloabl variables
        self.message = []

        # Draw the GUI
        self.init_GUI()     # setting [title && geometry]
        self.add_menu()     # Adding  [main menu && About us]
        self.add_panel()    # Adding  [frame for panel && console].
        self.add_console()

        # Start running GUI
        window.mainloop()

    """======================Draw the GUI======================"""

    def init_GUI(self):
        # Set title
        self.window.title("Pilot training Emulator")

        # centralize the windows
        width_limit = (self.window.winfo_screenwidth() - 800) / 2
        height_limit = (self.window.winfo_screenheight() - 800) / 2
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


    def add_panel(self):
        self.panel_frame = tkinter.Frame(height = 600,width = 800, bg = '#C0C0C0')
        self.panel_frame.pack(fill="x", padx=5, pady=5)


    def add_console(self):

        # Adding Frame with hw:200*800
        self.console_frame = tkinter.Frame(height = 200,width = 800)
        self.console_frame.pack(fill="x", padx=5, pady=5)

        # Adding message
        self.message = []
        self.console_message = Message(self.console_frame, text = "", width=800)
        self.console_message.pack(side='left')

    """======================Detect the event and react======================"""

    def update_message(self):
        # Clear buffer limit the length to 10
        if (len(self.message) > 10):
            self.message = self.message[1:]

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