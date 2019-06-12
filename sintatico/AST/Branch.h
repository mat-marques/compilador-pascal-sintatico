#ifndef Branch_H
#define Branch_H

#include <iostream>
#include <string>
#include <vector>
#include "Expression.h"
#include "Statement.h"

using namespace std;

class Branch {
    public:
        Expression ex; Statement st1; Statement st2;
        Branch(Expression ex, Statement st1, Statement st2){
            this->ex = ex;
            this->st1 = st1;
            this->st2 = st2;
        };
};


#endif