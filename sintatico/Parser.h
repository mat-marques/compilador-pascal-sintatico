#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <list>
#include "../lexico/HashTable.h"
#include "../lexico/Token.h"

using namespace std;

class Parser {
  public:
    Parser(list<Token*> *tokens, HashTable *hashIdentifiers);

  private:
    HashTable **hashIdentifiers;
    list<Token*> **tokens;

};

#endif