all: clean build-client build-server

build-client:
	gcc tcp_client.c -o client 
build-server:
	gcc tcp_server.c -o server 
clean: 
	rm -f client server 
