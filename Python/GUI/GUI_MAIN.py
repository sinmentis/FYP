import tkinter
from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.ttk import *


class CounterGui:
    """======================Construct the GUI in the given window======================"""

    def __init__(self, window):
        self.window = window

        self.init_GUI()  # setting title and geometry
        self.add_menu()

        self.count = 1
        self.name_label = Label(window, text="Count:")
        self.name_label.grid(row=0, column=0, sticky='E', padx=5, pady=5)

        self.count_label = Label(window, text=str(self.count))
        self.count_label.grid(row=0, column=1, sticky='W')

        self.up_button = Button(window, text='up', command=lambda: self.change(+1))
        self.up_button.grid(row=1, column=0, padx=5, pady=5)

        self.down_button = Button(window, text='down', command=lambda: self.change(-1))
        self.down_button.grid(row=1, column=1, padx=5, pady=5)

        window.mainloop()

    """======================Draw the GUI======================"""

    def init_GUI(self):
        self.window.title("Pilot training Emulator")
        width_limit = (self.window.winfo_screenwidth() - 800) / 2
        height_limit = (self.window.winfo_screenheight() - 800) / 2
        self.window.geometry("800x800+{0:.0f}+{1:.0f}".format(width_limit, height_limit))  # centralize the windows
        self.window.resizable(False, False)
        # self.window.config(bg='#535352')

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
        panel_frame = Frame(self.window)

    def add_console(self):
        pass

    """======================Detect the event and react======================"""

    def change(self, delta):
        """Handle clicks on one of the buttons"""
        self.count += delta
        self.count_label['text'] = str(self.count)

    def test(self):
        print("test")

    def about_us(self):
        showinfo(title="About us", message="This is E19")


if __name__ == "__main__":
    window = Tk()
    CounterGui(window)
    window.mainloop()
