#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <string>
#include <vector>
#include "Variable.h"
#include "Number.h"
using namespace std;


class Type {
    public:
        Variable *variable; vector<Number*> *numbers; Type *types;
        Type(Variable *variable, vector<Number*> *numbers, Type *types) {
            this->variable = variable;
            this->numbers = numbers;
            this->types = types;
        };
};

#endif