#ifndef SIMPLEEXPRESSION_H
#define SIMPLEEXPRESSION_H

#include <iostream>
#include <string>
#include "Term.h"

using namespace std;

class SimpleExpression {
    public:
        string op; Term*t1; SimpleExpression*simpleExpressions;
        SimpleExpression(string op, Term *t1, SimpleExpression *simpleExpressions) {
            this->op = op;
            this->t1 = t1;
            this->simpleExpressions = simpleExpressions;
        }
};


#endif