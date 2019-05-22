CFLAGS = -g -std=c++11

main: main.o HashTable.o List.o Item.o Lexicon.o Token.o
	g++ $(CFLAGS) -o main main.o HashTable.o List.o Item.o Lexicon.o Token.o

main.o: Lexicon.h Lexicon.cpp main.cpp
	g++ $(CFLAGS) -c main.cpp -o main.o

HashTable.o: HashTable.h HashTable.cpp List.h List.cpp
	g++ $(CFLAGS) -c HashTable.cpp -o HashTable.o

List.o: List.h List.cpp Item.h Item.cpp
	gcc $(CFLAGS) -c List.cpp -o List.o

Item.o: Item.h Item.cpp
	g++ $(CFLAGS) -c Item.cpp -o Item.o

Token.o: Token.h Token.cpp
	g++ $(CFLAGS) -c Token.cpp -o Token.o

Lexicon.o: Lexicon.h Lexicon.cpp HashTable.h HashTable.cpp Token.h Token.cpp
	g++ $(CFLAGS) -c Lexicon.cpp -o Lexicon.o

.PHONY:
	clean

clean:
	rm edit *.o