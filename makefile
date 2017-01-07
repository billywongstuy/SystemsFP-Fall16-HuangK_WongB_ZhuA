all: chinesePoker.c cards.h players.h
	gcc chinesePoker.c -o chinesePoker
clean:
	rm *~

run: chinesePoker
	./chinesePoker
