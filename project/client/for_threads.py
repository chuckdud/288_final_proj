"""
	This file contains a sketch for a GUI that can display changing text.

	Current idea is to have one box that updates with all responses from Cybot server
		and another text entry window that allows user to send requests.

	I have spent 0 time trying to make this look good; just focusing on functionality.

	Should be able to link up to client program and test on Saturday!

"""
import tkinter as tk
from tkinter import *
import socket   
import numpy as np
import matplotlib.pyplot as plt
import threading


options = [
	"Forward",
	"Backward",
	"Clockwise",
	"Counter-clockwise",
	"Scan",
	"Calibrate",
	"Quit"
		   ]


class MainWindow:
	def __init__(self, master, cybot):
		self.master = master

		self.cybot = cybot

		self.response = StringVar()
		self.cmd_type = StringVar()
		self.value = StringVar()

		self.cmd_lbl = Label(master, text="Command:")
		self.cmd_lbl.grid(row=0,column=0)

		self.action_lbl = Label(master, text="Action:")
		self.action_lbl.grid(row=1,column=0)

		self.cmd_drop = OptionMenu(master, self.cmd_type, *options)
		self.cmd_drop.grid(row=1,column=1)

		self.value_lbl = Label(master, text="Value:")
		self.value_lbl.grid(row=2,column=0)

		self.value_box = Entry(textvariable=self.value)
		self.value_box.grid(row=2, column=1)

		self.send_btn = tk.Button(text='Send Command', command=self._send_command)
		self.send_btn.grid(row=3,column=1)

		self.rsp_lbl = Label(master, text="Response:")
		self.rsp_lbl.grid(row=4,column=0)

		self.resp_box = Entry(textvariable=self.response, state=DISABLED)
		self.resp_box.grid(row=5, column=1)


	def _send_command(self):
		# send action
		cmd = self.cmd_type + '\0' + '\n'
		self.cybot.write(cmd.encode())
		# send numerical value (if applicable)
		if self.cmd_type == "Forward" or self.cmd_type == "Backward" or \
			self.cmd_type == "Clockwise" or self.cmd_type == "Counter-clockwise":
			
			cmd = self.value + '\0' + '\n'
			self.cybot.write(cmd.encode())


	def listen(self):
		while True:
			response = self.cybot.readline()
			self.response.set(response.decode())
		
		


def graph(ping_distance, ir_distance, angle_degrees):
        # Convert python sequence (list of strings) into a numpy array
        angle_degrees = np.array(angle_degrees) # Avoid "TypeError: can't multiply sequence by non-int of type float"
                                                # Link for more info: https://www.freecodecamp.org/news/typeerror-cant-multiply-sequence-by-non-int-of-type-float-solved-python-error/
                                                
        angle_radians = (np.pi/180) * angle_degrees # Convert degrees into radians

        # Ping
        fig, ax = plt.subplots(2, subplot_kw={'projection': 'polar'}) # One subplot of type polar
        ax[0].plot(angle_radians, ping_distance, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
        ax[0].set_rmax(50)                    # Saturate distance at 2.5 meters
        ax[0].set_rticks([0, 10, 20, 30, 40, 50])   # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
        ax[0].set_rlabel_position(-22.5)     # Adjust location of the radial labels
        ax[0].set_thetamax(180)              # Saturate angle to 180 degrees
        ax[0].set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
                                                # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
        ax[0].grid(True)                     # Show grid lines

        # Create title for plot (font size = 14pt, y & pad controls title vertical location)
        ax[0].set_title("Ping")

        # IR
        ax[1].plot(angle_radians, ir_distance, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
        ax[1].set_rmax(50)                    # Saturate distance at 2.5 meters
        ax[1].set_rticks([0, 10, 20, 30, 40, 50])   # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
        ax[1].set_rlabel_position(-22.5)     # Adjust location of the radial labels
        ax[1].set_thetamax(180)              # Saturate angle to 180 degrees
        ax[1].set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
                                                # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
        ax[1].grid(True)                     # Show grid lines

        # Create title for plot (font size = 14pt, y & pad controls title vertical location)
        ax[1].set_title("IR")

        plt.show()  # Display plot


def get_bot():
	HOST = "192.168.1.1"
	PORT = 288
	cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
	cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
						
	cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile

	return cybot

	# while True:
	# 		send_message = cmd + '\0' + '\n'   # 2) Have user enter text

	# 		cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server

	# 		print("Command sent: " + send_message) 
	# 		if cmd == "quit":
	# 				break
	# 		if cmd == "scan":
	# 				ping_dists = []
	# 				ir_dists = []
	# 				angles = []
	# 				for i in range(90):
	# 						ping_dists.append(int(cybot.readline().decode().strip('\r\n\x00')))
	# 						ir_dists.append(float(cybot.readline().decode().strip('\r\n\x00')))
	# 						angles.append(i * 2)
	# 				graph(ping_dists, ir_dists, angles)

	# 		print("Waiting for server reply...\n")
	# 		rx_message = cybot.readline()      # Wait for a message, readline expects message to end with "\n" TODO:: does readline actually expect '\n'???
	# 		print("Cybot Response: " + rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)
			
	# print("Client exiting, and closing file descriptor, and/or network socket\n")
	# time.sleep(2)
	# cybot.close()         # Close file object associated with the socket or UART





def gui(root):
	root.mainloop()

def main():
	cybot = get_bot()
	
	root = tk.Tk()
	# root.geometry("600x350")
	mw = MainWindow(root, cybot)
	t1 = threading.Thread(target=mw.listen)
	t1.start()
	root.mainloop()
	t1.join()
	# while True:
	# 	root.update()


if __name__ == '__main__':
	main()

