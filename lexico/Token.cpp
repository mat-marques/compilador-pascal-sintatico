#include <iostream>
#include <string>
#include "Token.h"

Token::Token() {}

Token::Token (string var_type, string var_value, int row, int col) {
    this->var_type = var_type;
    this->var_value = var_value;
    this->row = row;
    this->col = col;
}

void Token::show(){
    cout<< this->var_type << " = " << this->var_value << "\n";
}