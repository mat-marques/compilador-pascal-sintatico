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
        Declaration *dec; Expression *ex;
        Assign(Declaration *dec, Expression *ex) {
            this->dec = dec;
            this->ex = ex;
        };
};


#endif