#ifndef ASSIGN_H
#define ASSIGN_H

#include <iostream>
#include <string>
#include <vector>
#include "Expression.h"
#include "Declaration.h"

using namespace std;

class Assign {
    public:
        Declaration *dec; Expression *ex2;
        Assign(Declaration *dec, Expression *ex2) {
            this->dec = dec;
            this->ex2 = ex2;
        };
};


#endif