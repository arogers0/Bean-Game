############################################################################
# Client side: use sockets to send data to the server, and receive the server's
# reply. The server is the CPU opponent in this game. The client sends the
# number of beans remaining to the server after the player takes beans. The
# server removes a number of beans, and sends back the remaining number of
# beans.
############################################################################

import sys
from socket import *  # portable socket interface plus constants


def main():

    wins = 0
    losses = 0

    verify_usage()

    socket_obj = socket_setup()

    run_game(losses, socket_obj, wins)

    socket_obj.close()  # close socket to send eof to server


def run_game(losses, socket_obj, wins):
    while True:
        turn = 1
        print("+====================================================+")
        play = input("  Play The Bean Game? (y/n) ")  # Start game
        play = play.lower()

        if play == 'y':
            num_beans = 21  # start with 21 beans each game
            print_instructions()

            while num_beans > 0:
                # Player turn
                player_take = input(
                    f"  (Turn {turn}) How many do you want to take? ")
                turn += 1

                if player_take == '1' or player_take == '2' or player_take == '3':

                    losses, num_beans, wins = player_decision(losses,
                                                              num_beans,
                                                              player_take,
                                                              wins)

                    beans = send_and_receive(num_beans, socket_obj)

                    losses, num_beans = cpu_decision(beans, losses, num_beans,
                                                     wins)

                elif type(player_take) != type(int):  # Error handling
                    print(
                        "+----------------------------------------------------+")
                    print("  You can only take 1, 2, or 3 beans.")
                else:
                    print(
                        "+----------------------------------------------------+")
                    print("  You can only take 1, 2, or 3 beans.")
                    continue

        elif play == 'n':  # Quit game
            print("+----------------------------------------------------+")
            print("  Final Score:")
            print(f"   > Wins:   {wins}")
            print(f"   > Losses: {losses}")
            print("+----------------------------------------------------+")
            break
        else:  # Error handling
            print("+----------------------------------------------------+")
            print("  Enter y or n")
            continue


def verify_usage():
    # verify usage
    if len(sys.argv) != 3:
        print("Usage: python " + sys.argv[
            0] + " server_name port_number (> 1024)")
        sys.exit("error in parameters.")


def socket_setup():
    server_port = int(sys.argv[2])
    server_host = sys.argv[1]
    socket_obj = socket(AF_INET, SOCK_STREAM)  # make a TCP/IP socket object
    # connect to server machine and port
    socket_obj.connect((server_host, server_port))
    return socket_obj


def send_and_receive(num_beans, socket_obj):
    socket_obj.send(
        str.encode(str(num_beans)))  # send string to server over socket
    data = socket_obj.recv(1024)  # receive line from server: up to 1k
    beans = int(bytes.decode(data))
    return beans


def cpu_decision(beans, losses, num_beans, wins):
    cpu_take = num_beans - beans  # calculate number of beans CPU took
    num_beans = beans  # update num_beans
    # Print CPU decision
    if beans > 1 and cpu_take == 1:
        print(f"  CPU took {cpu_take} bean. There are {beans}",
              "beans remaining.")
        print("+----------------------------------------------------+")
    if beans == 1 and cpu_take == 1:
        print(f"  CPU took {cpu_take} bean. There is {beans}",
              "bean remaining.")
        print("+----------------------------------------------------+")
    if beans == 1 and cpu_take > 1:
        print(f"  CPU took {cpu_take} beans. There is {beans}",
              "bean remaining.")
        print("+----------------------------------------------------+")
    if beans == 0 and cpu_take == 1:
        print(f"  CPU took {cpu_take} bean. There are {beans}",
              "beans remaining.")
        print("+----------------------------------------------------+")
        print("  You lose!")
        losses += 1
        print(f"  Wins: {wins}")
        print(f"  Losses: {losses}")
    if beans > 1 and cpu_take > 1:
        print(f"  CPU took {cpu_take} beans. There are {beans}",
              "beans remaining.")
        print("+----------------------------------------------------+")
    if beans == 0 and cpu_take > 1:
        print(f"  CPU took {cpu_take} beans. There are {beans}",
              "beans remaining.")
        print("+----------------------------------------------------+")
        print("  You lose!")
        losses += 1
        print(f"  Wins: {wins}")
        print(f"  Losses: {losses}")
    return losses, num_beans


def print_instructions():
    print("+----------------------------------------------------+")
    print("| INSTRUCTIONS                                       |")
    print("|                                                    |")
    print("| This game begins with 21 beans.                    |")
    print("| Each player can remove 1, 2, or 3 beans at a time. |")
    print("| The goal is to remove the last bean.               |")
    print("+----------------------------------------------------+")


def player_decision(losses, num_beans, player_take, wins):
    player_take = int(player_take)
    num_beans -= player_take
    # Print result of player decision
    if num_beans == 1:
        print(f"  There is {num_beans} bean remaining.")
    elif num_beans > 1:
        print(f"  There are {num_beans} beans remaining.")
    if num_beans == 0:
        print(f"  There are {num_beans} beans remaining.")
        print("+----------------------------------------------------+")
        print("  You win!")
        wins += 1
        print(f"  Wins: {wins}")
        print(f"  Losses: {losses}")
    if num_beans < 0:
        print("  You lose!")
        losses += 1
        print(f"  Wins: {wins}")
        print(f"  Losses: {losses}")
    return losses, num_beans, wins


if __name__ == '__main__':
    main()
