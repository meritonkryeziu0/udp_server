# udp_server
Simple UDP Client Server communication

- Server 

1. It has variables that contain the port number and the IP Adress
2. It is able to listen to every member of the group
3. It can accept requests that are send by clients
4. It is able to read messages that are send by clients
5. It is able to grant access to the clients to read the files offered by the server 


- Client 

1. It can establish a socket connection with the server and it takes the IP Adress as an argument
2. One of the clients has write(), read(), execute().
3. The only clients only has read() permission.
4. It can establish connection with the server with the designated IP Adress and Port of the server.
5. It can read the responses send by the server.
6. It can send messages to the server in a text form.
7. It has access to the files that the server has to offer.