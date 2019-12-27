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
#include <arpa/inet.h>

#define DEFAULT_PORT 8080
#define MAX_BUFFER 1000

int PORT = DEFAULT_PORT;

int main_loop(){
	int sockfd; 
	ssize_t nbytes;
    socklen_t len; 
    char buffer[MAX_BUFFER]; 
    char *clnt_msg = "I'm client!!!"; 
    struct sockaddr_in servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    sendto(sockfd, (const char *)clnt_msg, strlen(clnt_msg), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    
	printf("sent(%lu bytes): %s\n", strlen(clnt_msg), clnt_msg); 
          
    nbytes = recvfrom(sockfd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &servaddr, &len); 
    buffer[nbytes] = '\0'; 
    
	printf("received(%ld) : %s\n",nbytes,  buffer); 
    close(sockfd); 
    
	return 0; 	
}


int main(int argc, char* argv[]){
	
	main_loop();
	
	return 0;
}
