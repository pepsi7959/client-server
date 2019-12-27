# client-server
The client-server library in order to optimise your services. 

## UDP
Helps you to create UDP servr and UDP client that use a non-blocking technique

## Example
Complies source code
```txt
$> make 
rm -f client server 
gcc udp_client.c -o client 
gcc udp_server.c -o server 
```

### UDP server 
```txt
$> ./server 
Avialable data from 1 sockets
process incoming data
recieved(13 bytes) : I'm client!!! 
sent(22 bytes): I've got your message
```

### UDP client
```txt
$> ./client 
sent(13 bytes): I'm client!!!
received(22) : I've got your message
```
