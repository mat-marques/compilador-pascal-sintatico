#ifndef TERM_H
#define TERM_H

#include <iostream>
#include <string>
#include "Factor.h"

using namespace std;

class Term {
    public:
        Factor *f; vector<Factor*> *factorList;
        Term(Factor *f, vector<Factor*> *factorList){ 
            this->f = f;
            this->factorList = factorList;
        };
};


#endif