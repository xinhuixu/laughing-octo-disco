all: server client

server: server.o networking.o func.o
	gcc -o server server.o networking.o func.o -Wall

client: client.o networking.o
	gcc -o client client.o networking.o

server.o: server.c networking.h func.h
	gcc -o server.o -c server.c

client.o: client.c networking.h 
	gcc -o client.o -c client.c

networking.o: networking.c networking.h
	gcc -o networking.o -c networking.c

func.o: func.c func.h
	gcc -o func.o -c func.c

clean:
	rm *.o
	rm server
	rm client
	rm *~
