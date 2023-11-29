# Author: Phillip Jones
# Modifications made by Charles Dudley

import serial
import time 
import socket   
import numpy as np
import matplotlib.pyplot as plt
import os  # import function for finding absolute path to this python script


def graph(distance, angle_degrees):
        # Convert python sequence (list of strings) into a numpy array
        angle_degrees = np.array(angle_degrees) # Avoid "TypeError: can't multiply sequence by non-int of type float"
                                                # Link for more info: https://www.freecodecamp.org/news/typeerror-cant-multiply-sequence-by-non-int-of-type-float-solved-python-error/
                                                
        angle_radians = (np.pi/180) * angle_degrees # Convert degrees into radians

        # Create a polar plot
        fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}) # One subplot of type polar
        ax.plot(angle_radians, distance, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
        ax.set_xlabel('Distance (m)', fontsize = 14.0)  # Label x axis
        ax.set_ylabel('Angle (degrees)', fontsize = 14.0) # Label y axis
        ax.xaxis.set_label_coords(0.5, 0.15) # Modify location of x axis label (Typically do not need or want this)
        ax.tick_params(axis='both', which='major', labelsize=14) # set font size of tick labels
        ax.set_rmax(50)                    # Saturate distance at 2.5 meters
        ax.set_rticks([0, 10, 20, 30, 40, 50])   # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
        ax.set_rlabel_position(-22.5)     # Adjust location of the radial labels
        ax.set_thetamax(180)              # Saturate angle to 180 degrees
        ax.set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
                                                # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
        ax.grid(True)                     # Show grid lines

        # Create title for plot (font size = 14pt, y & pad controls title vertical location)
        ax.set_title("Mock-up Polar Plot of CyBot Sensor Scan from 0 to 180 Degrees", size=14, y=1.0, pad=-24) 
        plt.show()  # Display plot


HOST = "192.168.1.1"
PORT = 288
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
                      
cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile

print("Input 'drive' to control the bot.") # TODO:: change this!

while True:
        cmd = input("Enter Command: ")
        send_message = cmd + '\0' + '\n'   # 2) Have user enter text

        cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server

        print("Command sent: " + send_message) 
        if cmd == "quit":
                break

        if cmd == "scan":
                distances = []
                ir_dists = []
                angles = []
                for i in range(90):
                        distances.append(int(cybot.readline().decode().strip('\r\n\x00')))
                        ir_dists.append(float(cybot.readline().decode().strip('\r\n\x00')))
                        angles.append(i * 2)
                print(ir_dists)
                graph(distances, angles)
                graph(ir_dists, angles)


        print("Waiting for server reply...\n")
        rx_message = cybot.readline()      # Wait for a message, readline expects message to end with "\n" TODO:: does readline actually expect '\n'???
        print("Cybot Response: " + rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)
        
print("Client exiting, and closing file descriptor, and/or network socket\n")
time.sleep(2)
cybot.close()         # Close file object associated with the socket or UART
# cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)
