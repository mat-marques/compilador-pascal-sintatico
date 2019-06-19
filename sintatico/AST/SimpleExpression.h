#ifndef SIMPLEEXPRESSION_H
#define SIMPLEEXPRESSION_H

#include <iostream>
#include <string>
#include "Term.h"

using namespace std;

class SimpleExpression {
    public:
        string op; Term* t; SimpleExpression* simpleExpressions;
        SimpleExpression(string op, Term *t, SimpleExpression *simpleExpressions) {
            this->op = op;
            this->t = t;
            this->simpleExpressions = simpleExpressions;
        };
};


#endif