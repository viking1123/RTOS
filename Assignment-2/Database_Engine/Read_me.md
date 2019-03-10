Instructions to run:

Server:
1. gcc -o server server.c -lpthread
2. ./server

Client:
1. gcc -o c1 c1.c             // Similarly create executables for other clients
2. ./c1 127.0.0.1             // run other executables too with IP address of the server system (single system used in case)
