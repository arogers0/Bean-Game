############################################################################
# Server side: open a TCP/IP socket on a port, listen for a message from
# a client, and send a reply. The reply is the number of beans left after the
# server removes some.
############################################################################

import sys
import string
import random
from socket import *  # get socket constructor and constants

if len(sys.argv) != 2:
    print("usage: python " + sys.argv[0] + "port-number (> 1024)")
    sys.exit("error in parameters.")

myHost = ''  # server machine, '' means local host
myPort = int(sys.argv[1])

sockobj = socket(AF_INET, SOCK_STREAM)  # make a TCP socket object
sockobj.bind((myHost, myPort))  # bind it to server port number
sockobj.listen(5)  # listen, allow 5 pending connects

while True:  # listen until process killed
    connection, address = sockobj.accept()  # wait for next client connect
    print('Server connected by', address)  # connection is a new socket
    while True:
        data = connection.recv(1024)  # read next line on client socket
        if not data: break  # send a reply line to the client
        num_beans = int(bytes.decode(data))

        # CPU turn
        cpuTake = 0
        if num_beans > 3:
            cpuTake = random.randint(1, 3)
        if num_beans == 3:
            cpuTake = 3
        if num_beans == 2:
            cpuTake = 2
        elif num_beans == 1:
            cpuTake = 1
        num_beans -= cpuTake

        connection.send(str.encode(str(num_beans)))
        # until eof when socket closed
    connection.close()
