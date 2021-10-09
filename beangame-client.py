############################################################################
# Client side: use sockets to send data to the server, and receive the server's
# reply. The server is the CPU opponent in this game. The client sends the
# number of beans remaining to the server after the player takes beans. The
# server removes a number of beans, and sends back the remaining number of
# beans.
############################################################################

import sys
import string
from socket import *  # portable socket interface plus constants

message = 'Hello World!'  # default text to send to server
wins = 0
losses = 0

if len(sys.argv) != 3:
    print("Usage: python " + sys.argv[0] + " server_name port_number (> 1024)")
    sys.exit("error in parameters.")

serverPort = int(sys.argv[2])
serverHost = sys.argv[1]

sockobj = socket(AF_INET, SOCK_STREAM)  # make a TCP/IP socket object
sockobj.connect((serverHost, serverPort))  # connect to server machine and port


while True:
    turn = 1
    print("+----------------------------+")
    play = input(" Play The Bean Game? (y/n) ")          # Start game
    play = play.lower()

    if play == 'y':
        num_beans = 21            # start with 21 beans each game
        print("+----------------------------------------------------+")
        print("| INSTRUCTIONS                                       |")
        print("|                                                    |")
        print("| This game begins with 21 beans.                    |")
        print("| Each player can remove 1, 2, or 3 beans at a time. |")
        print("| The goal is to remove the last bean.               |")
        print("+----------------------------------------------------+")

        while num_beans > 0:
            # Player turn
            playerTake = input(f"(Turn {turn}) How many do you want to take? ")
            turn += 1

            if playerTake == '1' or playerTake == '2' or playerTake == '3':

                playerTake = int(playerTake)
                num_beans -= playerTake

                # Print result of player decision
                if num_beans == 1:
                    print(f"There is {num_beans} bean remaining.")
                elif num_beans > 1:
                    print(f"There are {num_beans} beans remaining.")
                if num_beans == 0:
                    print(f"There are {num_beans} beans remaining.")
                    print("-------")
                    print("You win!")
                    wins += 1
                    print(f"Wins: {wins}")
                    print(f"Losses: {losses}")
                if num_beans < 0:
                    print("You lose!")
                    losses += 1
                    print(f"Wins: {wins}")
                    print(f"Losses: {losses}")

                sockobj.send(str.encode(str(num_beans)))  # send line to server over socket
                data = sockobj.recv(1024)  # receive line from server: up to 1k
                beans = int(bytes.decode(data))

                cpuTake = num_beans - beans
                num_beans = beans

                # Print CPU decision
                if beans > 1 and cpuTake == 1:
                    print(f"CPU took {cpuTake} bean. There are {beans}",
                          "beans remaining.")
                    print("-------")
                if beans == 1 and cpuTake == 1:
                    print(f"CPU took {cpuTake} bean. There is {beans}",
                          "bean remaining.")
                    print("-------")
                if beans == 1 and cpuTake > 1:
                    print(f"CPU took {cpuTake} beans. There is {beans}",
                          "bean remaining.")
                    print("-------")
                if beans == 0 and cpuTake == 1:
                    print(f"CPU took {cpuTake} bean. There are {beans}",
                          "beans remaining.")
                    print("-------")
                    print("You lose!")
                    losses += 1
                    print(f"Wins: {wins}")
                    print(f"Losses: {losses}")
                if beans > 1 and cpuTake > 1:
                    print(f"CPU took {cpuTake} beans. There are {beans}",
                          "beans remaining.")
                    print("-------")
                if beans == 0 and cpuTake > 1:
                    print(f"CPU took {cpuTake} beans. There are {beans}",
                          "beans remaining.")
                    print("-------")
                    print("You lose!")
                    losses += 1
                    print(f"Wins: {wins}")
                    print(f"Losses: {losses}")

            elif type(playerTake) != type(int):
                print("-------")                             # Error handling
                print("You can only take 1, 2, or 3 beans.")
            else:
                print("-------")
                print("You can only take 1, 2, or 3 beans.")
                continue

    elif play == 'n':  # Quit game
        print("+---------------+")
        print(" Final Score:")
        print(f"  > Wins:   {wins}")
        print(f"  > Losses: {losses}")
        print("+---------------+")
        break
    else:
        print("-------")  # Error handling
        print("Enter y or n")
        continue

sockobj.close()  # close socket to send eof to server
