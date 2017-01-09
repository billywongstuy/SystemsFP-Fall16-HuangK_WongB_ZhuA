all: chinesePoker.o cards.o players.o printMethods.o
	gcc cards.o players.o printMethods.o chinesePoker.o -o chinesePoker

chinesePoker.o: chinesePoker.c cards.h players.h
	gcc -c chinesePoker.c


cards.o: cards.c cards.h
	gcc -c cards.c

players.o: players.c players.h
	gcc -c players.c

printMethods.o: printMethods.c printMethods.h
	gcc -c printMethods.c

clean:
	rm *~ *.o chinesePoker 

run: all
	./chinesePoker
