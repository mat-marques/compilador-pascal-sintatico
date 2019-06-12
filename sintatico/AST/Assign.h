#ifndef ASSIGN_H
#define ASSIGN_H

#include <iostream>
#include <string>
#include <vector>
#include "Expression.h"
#include "Variable.h"

using namespace std;

class Assign {
    public:
        Variable v1; vector<Expression> ex1List; Expression ex2;
        Assign(Variable v1, vector<Expression> ex1List, Expression ex2) {
            this->v1 = v1;
            this->ex1List = ex1List;
            this->ex2 = ex2;
        };
};


#endif