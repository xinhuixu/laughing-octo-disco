all: server.exe client.exe

server.exe: server.o networking.o
	gcc -o server.exe server.o networking.o

client.exe: client.o networking.o
	gcc -o client.exe client.o networking.o

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *.exe
	rm *~
