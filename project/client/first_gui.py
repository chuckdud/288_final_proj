import tkinter as tk

window = tk.Tk()
window.title("Mission Control")
window.geometry("1000x800+20+20")
manual = tk.Button(
    window,
    text="Click me!",
    width=25,
    height=5,
    bg="blue",
    fg="yellow",
)
manual.place(x=80, y=100)
window.mainloop()

