import tkinter
from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.ttk import *


class CounterGui:
    """======================Construct the GUI in the given window======================"""

    def __init__(self, window):
        
        # Assign windows to self
        self.window = window

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
        mainmenu.add_command(label='Setting', command=self.test)
        mainmenu.add_command(label='Testing', command=self.test)
        mainmenu.add_separator()
        mainmenu.add_command(label='Exit', command=self.window.destroy)
        menubar.add_cascade(label='Menu', menu=mainmenu)

        # create "About us"
        menubar.add_command(label='About us', command=self.about_us)

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
        self.console_message = Message(self.console_frame, text = "")
        self.console_message.pack(side='left')
    
    """======================Detect the event and react======================"""

    def change(self, delta):
        """Handle clicks on one of the buttons"""
        self.count += delta
        self.count_label['text'] = str(self.count)

    def test(self):
        self.update_message("HIHI")

    def about_us(self):
        showinfo(title="About us", message="This is E19")
        
    def update_message(self, message):
        self.console_message.configure(text=message)

if __name__ == "__main__":
    window = Tk()
    CounterGui(window)
    window.mainloop()
