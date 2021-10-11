#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include "wrappers.h"

#define BUFFER_SIZE 16
#define TRUE 1
#define FALSE 0

/*-------------------------------------------------------------------------
 * Program:  beangame-client - 
 *
 * Purpose:  allocate a socket, connect to a server, player plays the game
 *           and data is sent back and forth between the client and server.
 *
 * Usage:    beangame-client [ host ] [ port ]  
 *
 *		 host  - name of a computer on which server is executing
 *		 port  - protocol port number server is using
 *     
 *-------------------------------------------------------------------------
 */

int main(int argc, char* argv[]) {

	struct	sockaddr_in sad; // structure to hold an IP address	
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *ptr;

	int	sd;		               // socket descriptor			
	int port;	               // protocol port number		
	char *host;                // pointer to host name		

	memset((char *)&sad,0,sizeof(sad)); // zero out sockaddr structure	
	sad.sin_family = AF_INET;	          // set family to Internet	

	// verify usage

	if (argc < 3) {
		printf("Usage: %s [ host ] [ port ] \n", argv[0]);
		exit(-1);
	}

	host = argv[1];		
	port = atoi(argv[2]);	

	if (port > 0)	
		// test for legal value		
		sad.sin_port = htons((u_short)port);
	else {				
		// print error message and exit	
		printf("ECHOREQ: bad port number %s\n", argv[2]);
		exit(-1);
	}

	// convert host name to equivalent IP address and copy to sad 

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    getaddrinfo(host, argv[2], &hints, &res);

    for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
	    // create socket 

	    sd = Socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sd == -1) continue;

	    // connect the socket to the specified server 

	    int c = Connect(sd, ptr->ai_addr, ptr->ai_addrlen);
        if (c == -1) {
            Close(sd);
            continue;
        }

        break;
    }

    if (ptr == NULL) {
        fprintf(stderr, "Failed to connect\n");
        exit(-1);
    }

    int wins = 0;
    int losses = 0;
    char char_input;

    while (1) {
        int turn = 1;
        printf("+====================================================+\n"); 
        __fpurge(stdin);  // purge input buffer
        printf("  Play the bean game? (y/n): ");
        char_input = getc(stdin);  // player input
        
        if (char_input == 'y') {  // start the game
            int num_beans = 21;
            printf("+----------------------------------------------------+\n");
            printf("| INSTRUCTIONS                                       |\n");
            printf("|                                                    |\n");
            printf("| This game begins with 21 beans.                    |\n");
            printf("| Each player can remove 1, 2, or 3 beans at a time. |\n");
            printf("| The goal is to remove the last bean.               |\n");
            printf("+----------------------------------------------------+\n");

            while (num_beans > 0){
                char in_msg[BUFFER_SIZE];  // buffer for incoming messages
                // Player turn
                printf("  (Turn %d) How many do you want to take? ", turn);
                int player_take;
                scanf("%d", &player_take);  // player input (same function as getc)
                ++turn;

                if ((player_take == 1) || (player_take == 2) || (player_take == 3)) {
                    num_beans -= player_take;

                    // Print result of player decision
                    if (num_beans == 1) {
                        printf("  There is %d bean remaining.\n", num_beans);
                    } else if (num_beans > 1) {
                        printf("  There are %d beans remaining.\n", num_beans);
                    } else if (num_beans == 0) {
                        printf("  There are %d beans remaining.\n", num_beans);
                        printf("+----------------------------------------------------+\n");
                        printf("  You win!\n");
                        ++wins;
                        printf("  Wins: %d\n", wins);
                        printf("  Losses: %d\n", losses);
                        break;
                    } else {
                        printf("  You lose!\n");
                        ++losses;
                        printf("  Wins: %d\n", wins);
                        printf("  Losses: %d\n", losses);
                        break;
                    }

                    char num_beans_str[BUFFER_SIZE];
                    sprintf(num_beans_str, "%d", num_beans); // int to string
                    // send message to server
                    Send(sd, num_beans_str, BUFFER_SIZE, 0);

                    // receive message from server
                    Recv(sd, &in_msg, BUFFER_SIZE, 0);
                    int beans = atoi(in_msg);
                    in_msg[0] = '\0';
                    int cpu_take = num_beans - beans;
                    num_beans = beans;

                    // Print CPU decision
                    if ((num_beans > 1) && (cpu_take == 1)) {
                        printf("  CPU took %d bean. There are %d beans remaining.\n", cpu_take, num_beans);
                        printf("+----------------------------------------------------+\n");
                    } else if ((num_beans == 1) && (cpu_take == 1)) {
                        printf("  CPU took %d bean. There is %d bean remaining.\n", cpu_take, num_beans);
                        printf("+----------------------------------------------------+\n");
                    } else if ((num_beans == 1) && (cpu_take > 1)) {
                        printf("  CPU took %d beans. There is %d bean remaining.\n", cpu_take, num_beans);
                        printf("+----------------------------------------------------+\n");
                    } else if ((num_beans == 0) && (cpu_take == 1)) {
                        printf("  CPU took %d bean. There are %d beans remaining.\n", cpu_take, num_beans);
                        printf("+----------------------------------------------------+\n");
                        printf("  You lose!\n");
                        ++losses;
                        printf("  Wins: %d\n", wins);
                        printf("  Losses: %d\n", losses);
                        break;
                    } else if ((num_beans > 1) && (cpu_take > 1)) {
                        printf("  CPU took %d beans. There are %d beans remaining.\n", cpu_take, num_beans);
                        printf("+----------------------------------------------------+\n");
                    } else {
                        printf("  CPU took %d beans. There are %d beans remaining.\n", cpu_take, num_beans);
                        printf("+----------------------------------------------------+\n");
                        printf("  You lose!\n");
                        ++losses;
                        printf("  Wins: %d\n", wins);
                        printf("  Losses: %d\n", losses);
                        break;
                    }
                } else {
                    printf("+----------------------------------------------------+\n");
                    printf("  You can only take 1, 2, or 3 beans.\n");
                }
            }
        } else if (char_input == 'n') {  // end game
            printf("+----------------------------------------------------+\n");
            printf("  Final Score:\n");
            printf("    > Wins:    %d\n", wins);
            printf("    > Losses:  %d\n", losses);;
            printf("+----------------------------------------------------+\n");
            break;
        } else {  // handle error
            printf("+----------------------------------------------------+\n");
            printf("Enter y or n\n");
        }
    }

	// close the socket   
	Close(sd);

	return(0);
}
