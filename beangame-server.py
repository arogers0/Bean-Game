############################################################################
# Server side: open a TCP/IP socket on a port, listen for a message from
# a client, and send a reply. The reply is the number of beans left after the
# server removes some.
############################################################################

import sys
import random
from socket import *  # get socket constructor and constants


def main():

    # verify usage
    if len(sys.argv) != 2:
        print("usage: python " + sys.argv[0] + "port-number (> 1024)")
        sys.exit("error in parameters.")

    myHost = ''  # server machine, '' means localhost
    myPort = int(sys.argv[1])

    socket_obj = socket(AF_INET, SOCK_STREAM)  # make a TCP socket object
    socket_obj.bind((myHost, myPort))  # bind it to server port number
    socket_obj.listen(5)  # listen, allow 5 pending connects

    while True:  # listen until process killed
        connection, address = socket_obj.accept()  # wait for next client connect
        print('Server connected by', address)  # connection is a new socket
        while True:
            data = connection.recv(1024)  # read next line on client socket
            if not data: break
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

            # send number of remaining beans to the client
            connection.send(str.encode(str(num_beans)))

        # close socket
        connection.close()


if __name__ == '__main__':
    main()