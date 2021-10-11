#The Bean Game
Alex Rogers

###Description:
This game begins with 21 beans. 
Each player can remove 1, 2, or 3 beans at a time. 
The goal is to remove the last bean. \
The file `beangame-server.py` sets up a server for the game. There is also a C version.
It opens a TCP/IP socket on a port, listen for a message from a client, and send a reply. The 
reply is the number of beans left after the server removes some. \
The file `beangame-client.py` is what the player interacts with. There is also a C version.
The player makes a decision for how many beans to remove, then the remainder is sent to
the server. The server makes a similar decision and sends the remainder back
to the client. This repeats until either the player or CPU wins.

###Run:
**Python** \
To run the game, first open two terminals. The examples below are assuming you
in the directory of the files. \
To run the server: `$python beangame-server.py 1045` \
To run the client: `$python beangame-client.py localhost 1045` \
The port 1045 can be changed to a number >1024. The host localhost can be
changed to some other host if you want to run the server and client on 
separate machines. Use localhost if you are running both on the same machine.
To stop running the client, either finish the game or terminate the process.
To stop running the server, you need to terminate the process.

**C** \
To run the C version, open two terminals. The examples below are assuming you
in the directory of the files.
To run the server: `$./beangame-server 1045` \
To run the client: `$./beangame-client localhost 1045` \
The port 1045 can be changed to a number >1024. The host localhost can be
changed to some other host if you want to run the server and client on 
separate machines. Use localhost if you are running both on the same machine.
To stop running the client, either finish the game or terminate the process.
To stop running the server, you can terminate the process or allow the
client to stop running. \
If you need to compile the programs first, there is a Makefile included. \
In the file directory, enter `$make` to compile the files needed.