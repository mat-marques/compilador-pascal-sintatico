#ifndef DECLARATION_H
#define DECLARATION_H

#include <iostream>
#include <string>
#include <vector>
#include "Variable.h"
#include "Expression.h"

using namespace std;

class Declaration {
    public:
        Variable *v; vector<Expression*> *ex;
        Declaration(Variable *v, vector<Expression*> *ex) {
            this->v = v;
            this->ex = ex;
        };
};


#endif