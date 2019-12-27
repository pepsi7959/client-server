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
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8080
#define MAX_BUFFER 1000

int PORT = 8080;

int process_msg( int sockfd ){

	printf("process incoming data\n");	
	
	struct sockaddr_in clnt_addr;
	char buff[MAX_BUFFER];
	socklen_t len;
	
	len = sizeof(clnt_addr);

	ssize_t nbytes = recvfrom(sockfd, (char*)buff, 100, 0, (struct sockaddr *) &clnt_addr, &len);	

	buff[nbytes] = 0;	
	printf("recieved(%ld bytes) : %s \n", nbytes, buff);

	// Response to client
	size_t n = sprintf(&buff[0], "I've got your message\n");

	nbytes = sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));

	printf("sent(%d bytes): %s \n", (int)n, buff);

	if( nbytes > 0) {
		printf("completely sent %ld bytes\n", nbytes);	
	}
	else if( nbytes < 0 ) {
		printf("sending failed : %s\n", strerror(errno));
		return -1;
	}
	else {
		printf("no message to send\n");
	}

	return 0;
}

int main_loop(){
	
	int n, r, len, sockfd;
	fd_set rfds, wfds;
	struct timeval tv;
	struct sockaddr_in sock_addr;

	if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&sock_addr, 0, sizeof(sock_addr));

	// Filling server information
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(PORT);
	sock_addr.sin_addr.s_addr = INADDR_ANY;

	if ( bind(sockfd, (const struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0 ){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

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
				process_msg(sockfd);
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
