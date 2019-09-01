from tkinter import *
from tkinter.ttk import *


class CounterGui:
    """======================Construct the GUI in the given window======================"""

    def __init__(self, window):
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

    def add_menu(self):
        pass

    def add_button(self):
        pass

    def add_console(self):
        pass

    """======================Detect the event and react======================"""

    def change(self, delta):
        """Handle clicks on one of the buttons"""
        self.count += delta
        self.count_label['text'] = str(self.count)


if __name__ == "__main__":
    window = Tk()
    CounterGui(window)
    window.mainloop()
