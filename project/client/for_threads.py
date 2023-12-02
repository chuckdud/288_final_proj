import tkinter as tk
from tkinter import *


seed = 0


def increment():
	seed += 1


root = Tk()

L1 = Label(root, text="Responses")
L1.grid(row=0,column=0)
L2 = Label(root, text="Requests")
L2.grid(row=1,column=0)

response = StringVar()
response.set('example')

entry = Entry(textvariable=response,
			  state=DISABLED).grid(row=0,
			  					   column=1,
								   padx=10,
								   pady=10)

request = Entry().grid(row=1,column=1,
					   padx=10,pady=10)

# responses_box = Text(root,height=10,width=50)
# response_lbl = Label(root,text="responses")
# 
# response = hash(seed)
# 
increase_btn = Button(root,text="increase",command=increment)
# 
# response_lbl.pack()
# responses_box.pack()
# increase_btn.pack()
# 
# responses_box.insert(tk.END, response)

mainloop()
