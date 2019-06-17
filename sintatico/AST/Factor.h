#ifndef FACTOR_H
#define FACTOR_H

#include <iostream>
#include <string>
#include "Declaration.h"
#include "Number.h"
#include "Expression.h"

using namespace std;

class Factor {
    public:
        Declaration *d1;
        Number *n1;
        Expression *ex1;
        Factor *f1;
        Factor(Declaration *d1, Number *n1, Expression *ex1, Factor *f1) {
            this->d1 = d1;
            this->n1 = n1;
            this->ex1 = ex1;
            this->f1 = f1;
        }
};


#endif