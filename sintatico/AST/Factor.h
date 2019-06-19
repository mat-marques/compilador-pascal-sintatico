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
        Declaration *d;
        Number *n;
        Expression *ex;
        Factor *f;
        Factor(Declaration *d, Number *n, Expression *ex, Factor *f) {
            this->d = d;
            this->n = n;
            this->ex = ex;
            this->f = f;
        };
};


#endif