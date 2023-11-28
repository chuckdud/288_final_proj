# Author: Phillip Jones
# Modifications made by Charles Dudley

import serial
import time 
import socket   

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

        print("Waiting for server reply...\n")
        rx_message = cybot.readline()      # Wait for a message, readline expects message to end with "\n" TODO:: does readline actually expect '\n'???
        print("Cybot Response: " + rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)
        
print("Client exiting, and closing file descriptor, and/or network socket\n")
time.sleep(2)
cybot.close()         # Close file object associated with the socket or UART
# cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)
