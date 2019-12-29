/* 
 * MIT License
 * 
 * Copyright (c) 2019-present Narongsak Mala <narongsak.mala@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include "tcp.h"

int PORT = DEFAULT_TCP_PORT;

int process_msg( int sockfd ){
	
	int len;
	char buff[MAX_BUFFER];
	
	printf("process incoming data\n");	
	ssize_t nbytes = read( sockfd, &buff[0], MAX_BUFFER); 	
	printf("received(%lu): %s\n", nbytes, buff);
	if( nbytes > 0 ){
		len = sprintf(buff, "I've got a message");
		ssize_t w_byte = write( sockfd, &buff[0], len);
		printf("Sent(%d): %s\n", len, buff);
	}else{
		printf("process failed: %s\n", strerror(errno));	
	}

	close(sockfd);
	return 0;
}

int main_loop(){
	
	int n, r, len, sockfd;
	fd_set rfds, wfds;
	struct timeval tv;
	struct sockaddr_in sock_addr;

	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&sock_addr, 0, sizeof(sock_addr));

	// Filling server information
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	if ( bind(sockfd, (const struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0 ) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if( listen(sockfd, SOMAXCONN) != 0 ){
		printf("listen failed: %s", strerror(errno));	
		return -1;	
	}

	printf("TCP server listen on port %d \n", PORT);

	while(1) {

		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_SET(sockfd, &rfds);

		// Wating 5 second for incoming
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		r = select(sockfd+1, &rfds, &wfds, NULL, &tv); 

		if( r == -1) {
			break;
		}
		else if ( r ) {
		
			printf("Avialable data from %d sockets\n", r);

			// there are incoming to read
			if( FD_ISSET(sockfd ,&rfds) ) {
				
				struct sockaddr addr;
				socklen_t len;
				
				// Accecpt new connection
				int clientfd = accept( sockfd, &addr, &len);
	
				printf("Starting child process\n");

				// Process new connection
				if( fork() == 0 ){	
					process_msg(clientfd);
				}

				printf("Parent proccess unlink socket id: %d \n", clientfd);
				close(clientfd);
			}

			// there are something to send out
			if( FD_ISSET(sockfd ,&wfds) ) {
				//TODO:	
			}

		}else{
			printf("No data within five second.\n");	
		}
	}
	return 0;
}


int main(int argc, char* argv[]){

	if( argc > 1 ){
		char *str_port = argv[1];
		int port = atoi(str_port);
		PORT = (port > 1024)? port:PORT; 
		printf("UDP Server listen on port %d\n\n", PORT);
	}

	main_loop();
	
	return 0;
}
