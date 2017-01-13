all: main network

main: chinesePoker.o cards.o players.o printMethods.o input.o turns.o rules.o
	gcc cards.o players.o printMethods.o input.o turns.o rules.o chinesePoker.o -o chinesePoker

chinesePoker.o: chinesePoker.c cards.h players.h
	gcc -c chinesePoker.c

cards.o: cards.c cards.h
	gcc -c cards.c

players.o: players.c players.h
	gcc -c players.c

printMethods.o: printMethods.c printMethods.h
	gcc -c printMethods.c

input.o: input.c input.h
	gcc -c input.c

turns.o: turns.c turns.h
	gcc -c turns.c

rules.o: rules.c rules.h
	gcc -c rules.c




network: server client


server: server.o networking.o cards.o players.o printMethods.o input.o turns.o rules.o
	gcc cards.o players.o printMethods.o input.o turns.o rules.o server.o networking.o -o server

client: client.o networking.o cards.o players.o printMethods.o input.o turns.o rules.o
	gcc cards.o players.o printMethods.o input.o turns.o rules.o client.o networking.o -o client

server.o: server.c networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

networking.o: networking.c networking.h
	gcc -c networking.c




clean:
	rm *~ *# *.o chinesePoker client server

run: main
	./chinesePoker
