#ifndef LEXICON_H
#define LEXICON_H

#include <iostream>
#include <string>
#include <vector>

#include "HashTable.h"
#include "Token.h"

using namespace std;

class Lexicon {
    public:
        vector<int> finalStates;
        int startState, qtdStates;
        int **matrix;
        HashTable *hashReservedWords, *hashIdentifiers;
        list<Token*> *tokens;
        Lexicon(string automatonFileName, string reservedWordsFileName, int idHashSize, int rwHashSize);
        void process_lexicon(string inputFileName, string outPutHashFile, string tokensFileName);
        Token *getTokenList(int index);
        
    private:
        int **getMatrix();
        void deleteMatrix();
        void config_reserved_words(string reservedWordsFileName);
        void config_automaton(string automatonFileName);
        int getToken(int posic);
        void configureMatrix(int currentState, int nextState, string myString);
        int _nextState(int currentState, char c);
        bool _finalState(int currentState);
        bool error(int state, string error, int line, int column);
        bool checkString(string myString, int line);
        vector<string> *splitString(string input, char spliter);
};


#endif