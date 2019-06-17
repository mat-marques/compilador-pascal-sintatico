#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <string>
#include "SimpleExpression.h"

using namespace std;

class Expression {
    public:
        SimpleExpression *ex1; string op; SimpleExpression *ex2;
        Expression(SimpleExpression *ex1, string op, SimpleExpression *ex2) {
            this->ex1 = ex1;
            this->op = op;
            this->ex2 = ex2;
        };
};


#endif