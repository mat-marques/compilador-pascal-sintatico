CFLAGS = -g -std=c++11
build = /PATHTO/compilador-pascal-sintatico/build/
lexico = /PATHTO/compilador-pascal-sintatico/lexico/
#sintatico = 

main: main.o HashTable.o List.o Item.o Lexicon.o Token.o
	g++ $(CFLAGS) -o main main.o HashTable.o List.o Item.o Lexicon.o Token.o

$(build)main.o: Lexicon.h Lexicon.cpp main.cpp
	g++ $(CFLAGS) -c main.cpp -o main.o

$(build)HashTable.o: HashTable.h HashTable.cpp List.h List.cpp
	g++ $(CFLAGS) -c HashTable.cpp -o HashTable.o

$(build)List.o: List.h List.cpp Item.h Item.cpp
	gcc $(CFLAGS) -c List.cpp -o List.o

$(build)Item.o: Item.h Item.cpp
	g++ $(CFLAGS) -c Item.cpp -o Item.o

$(build)Token.o: Token.h Token.cpp
	g++ $(CFLAGS) -c Token.cpp -o Token.o

$(build)Lexicon.o: Lexicon.h Lexicon.cpp HashTable.h HashTable.cpp Token.h Token.cpp
	g++ $(CFLAGS) -c Lexicon.cpp -o Lexicon.o

.PHONY:
	clean

clean:
	rm edit *.o