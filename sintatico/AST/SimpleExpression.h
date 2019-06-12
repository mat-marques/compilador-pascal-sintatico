#ifndef SIMPLEEXPRESSION_H
#define SIMPLEEXPRESSION_H

#include <iostream>
#include <string>
#include "Term.h"

using namespace std;

class SimpleExpression {
    public:
        string op; vector<Term> t1;
        SimpleExpression(string op, vector<Term> t1) {
            this->op = op;
            this->t1 = t1;
        }
};


#endif