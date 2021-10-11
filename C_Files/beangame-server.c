#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wrappers.h"

#define BUFFER_SIZE 16 // length of message buffer    
#define	QLEN 6   // length of request queue

int	num_requests = 0;  // tally of client requests

/*--------------------------------------------------------------------------
 * Program:   beangame-server - a server for the CPU's turn in the bean game
 *
 * Purpose:   repeatedly execute the following:
 *      (0) wait for a connection request from client
 *		(1) wait for a null-terminated string from client
 *		(3) computer makes a decision
 *		(2) send back a string to client 
 *		(3) close the connection
 *		(4) go back to step (0)
 *
 * Usage:    beangame-server [ port ]
 *
 *		 port  - a port number in user space
 *
 *--------------------------------------------------------------------------
 */

int main (int argc, char* argv[]) {

	struct sockaddr_in sad;  // structure to hold server's address	
	struct sockaddr_in cad;  // structure to hold client's address	
	int sd, sd2;	           // socket descriptors			
	int port;		             // protocol port number		
	socklen_t alen;	         // length of address			
	char in_msg[BUFFER_SIZE];  // buffer for incoming message
    char out_msg[BUFFER_SIZE]; // buffer for outgoing message

	// prepare address data structure

	// The memset call is ESSENTIAL!
	// if you don't do this every time you create a sockaddr struct, you will 
	// see some pretty strange behaviour
	memset((char *)&sad,0,sizeof(sad)); // zero out sockaddr structure

	sad.sin_family = AF_INET;	          // set family to Internet	        
	sad.sin_addr.s_addr = INADDR_ANY;   // set the local IP address	

	// verify usage

	if (argc > 1) {			
		port = atoi(argv[1]);	        
	}
	else {
		printf("Usage: %s [ port ]\n", argv[0]);
		exit(-1);
	}

	if (port > 0)	
		// test for illegal value	
		sad.sin_port = htons((u_short)port);
	else {				
		// print error message and exit	
		fprintf(stderr,"ECHOD: bad port number %s\n", argv[1]);
		exit(-1);
	}

	// create socket 

	sd = Socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// assign IP/port number to socket where connections come in 

	Bind(sd, (struct sockaddr *)&sad, sizeof(sad));

	// set up socket to receive incomming connections 

	Listen(sd, QLEN);

	// main server loop - accept and handle requests 

	while (1) {
		alen = sizeof(cad);

		sd2 = Accept(sd, (struct sockaddr *)&cad, &alen); 

		num_requests++;
        while (1) {
		// receive the string sent by client

        Recv(sd2, in_msg, BUFFER_SIZE, 0);
        printf("DEBUG: in_msg = %s\n", in_msg);

        int num_beans = atoi(in_msg);

        // CPU Turn
        int cpu_take = 0;
        srand(time(NULL)); // generate seed for pseudo-random number generator
        if (num_beans > 3) {
            cpu_take = (rand() % 3) + 1; // pseudo-random number in range [1, 3]
        } else if (num_beans == 3) {
            cpu_take = 3;
        } else if (num_beans == 2) {
            cpu_take = 2;
        } else {
            cpu_take = 1;
        }
        num_beans -= cpu_take;
        
		// send the remaining number of beans back to client
        sprintf(out_msg, "%d", num_beans);  // int to string
        printf("DEBUG: out_msg = %s\n", out_msg);
        Send(sd2, out_msg, BUFFER_SIZE, 0);
        out_msg[0] = '\0';  // clear buffer
        }
		Close(sd2);
    }
}
