#ifndef TERM_H
#define TERM_H

#include <iostream>
#include <string>
#include "Factor.h"

using namespace std;

class Term {
    public:
        Factor *f; string op; Term *t;
        Term(Factor *f, string op, Term *t){ 
            this->f = f;
            this->op = op;
            this->t = t;
        };
};


#endif