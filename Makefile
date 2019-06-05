CFLAGS = -g -std=c++11
build = ./build/
lexico = ./lexico/
#sintatico = 

main: $(build)main.o $(build)HashTable.o $(build)List.o $(build)Item.o $(build)Lexicon.o $(build)Token.o
	g++ $(CFLAGS) -o main $(build)main.o $(build)HashTable.o $(build)List.o $(build)Item.o $(build)Lexicon.o $(build)Token.o

$(build)main.o: $(lexico)Lexicon.h $(lexico)Lexicon.cpp main.cpp
	g++ $(CFLAGS) -c main.cpp -o $(build)main.o

$(build)HashTable.o: $(lexico)HashTable.h $(lexico)HashTable.cpp $(lexico)List.h $(lexico)List.cpp
	g++ $(CFLAGS) -c $(lexico)HashTable.cpp -o $(build)HashTable.o

$(build)List.o: $(lexico)List.h $(lexico)List.cpp $(lexico)Item.h $(lexico)Item.cpp
	gcc $(CFLAGS) -c $(lexico)List.cpp -o $(build)List.o

$(build)Item.o: $(lexico)Item.h $(lexico)Item.cpp
	g++ $(CFLAGS) -c $(lexico)Item.cpp -o $(build)Item.o

$(build)Token.o: $(lexico)Token.h $(lexico)Token.cpp
	g++ $(CFLAGS) -c $(lexico)Token.cpp -o $(build)Token.o

$(build)Lexicon.o: $(lexico)Lexicon.h $(lexico)Lexicon.cpp $(lexico)HashTable.h $(lexico)HashTable.cpp $(lexico)Token.h $(lexico)Token.cpp
	g++ $(CFLAGS) -c $(lexico)Lexicon.cpp -o $(build)Lexicon.o

.PHONY:
	clean

clean:
	rm edit $(build)*.o