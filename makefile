server: client.c serverv2.c
	gcc client.c -pthread -o client
	gcc serverv2.c -pthread -o server
s: server
	./server
c: client
	./client
