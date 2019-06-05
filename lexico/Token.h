#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

class Token {
  public:
    string var_type;
    string var_value;
    int row;
    int col;
    Token();
    Token(string var_type, string var_value, int row, int col);
    void show();
};

#endif