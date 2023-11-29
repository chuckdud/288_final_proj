import tkinter as tk
import serial
import time
import socket


class MainWindow:
    def __init__(self, master):
        self.master = master
        self.frame = tk.Frame(self.master)
        self.mc_btn = tk.Button(self.frame, text = 'Manual Control', width = 100, height = 100, command = self.manual_control)
        self.mc_btn.pack()
        self.frame.pack()

    def manual_control(self):
        self.newWindow = tk.Toplevel(self.master)
        self.app = ManualControl(self.newWindow)


class ManualControl:
    def __init__(self, master):
        self.master = master
        self.frame = tk.Frame(self.master)
        self.manual_btn = tk.Button(self.frame, text = 'Quit', width = 100, height = 100, command = CyBotComms.manual_cmd)
        self.manual_btn.pack()
        self.frame.pack()


class CyBotComms:
    def __init__(self):
        self.HOST = "192.168.1.1"
        self.PORT = 288
        self.cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
        self.cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
        self.cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile

    def manual_cmd(self):
        pass


    def send_msg(self, cmd):
        self.cybot.write(manual_cmd());
        

def main(): 
    # print("Input 'drive' to control the bot.") # TODO:: change this!

    root = tk.Tk()
    main = MainWindow(root)
    root.mainloop()

if __name__ == '__main__':
    main()
