"""
	This file contains a sketch for a GUI that can display changing text.

	Current idea is to have one box that updates with all responses from Cybot server
		and another text entry window that allows user to send requests.

	I have spent 0 time trying to make this look good; just focusing on functionality.

	Should be able to link up to client program and test on Saturday!

"""
import tkinter as tk
from tkinter import *


seed = 0


def inc():
	global seed
	seed += 1


class MainWindow:
	def __init__(self, master):
		self.master = master
		self.inc_btn = tk.Button(text='Manual Control',
                                 width=30,
                                 height=30,
                                 command=self.update_count)
		self.count = StringVar()
		self.count.set(hash(seed))

		self.inc_btn.grid(row=2,column=0)
		self.L1 = Label(master, text="Responses")
		self.L1.grid(row=0,column=0)
		self.L2 = Label(master, text="Requests")
		self.L2.grid(row=1,column=0)

		self.entry = Entry(textvariable=self.count,
					  state=DISABLED).grid(row=0,
										   column=1,
										   padx=10,
										   pady=10)
		self.request = Entry().grid(row=1,column=1,
							   padx=10,pady=10)
	def update_count(self):
		inc()
		self.count.set(hash(seed))



def main():
	root = tk.Tk()
	# root.geometry("600x350")
	main = MainWindow(root)
	root.mainloop()


if __name__ == '__main__':
	main()

