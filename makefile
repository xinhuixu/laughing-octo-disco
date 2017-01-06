server.exe : server.h server.c
	gcc -c server.c

client.exe: client.h client.c
	gcc -lreadline client.c -o client.exe

clean:
	rm *~

run: client.exe
	./client.exe
