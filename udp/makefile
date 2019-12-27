all: clean build-client build-server

build-client:
	gcc udp_client.c -o client 
build-server:
	gcc udp_server.c -o server 
clean: 
	rm -f client server 
