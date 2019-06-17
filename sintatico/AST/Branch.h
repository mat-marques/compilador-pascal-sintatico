#ifndef Branch_H
#define Branch_H

#include <iostream>
#include <string>
#include <vector>
#include "Expression.h"
#include "StatementWithoutLabel.h"

using namespace std;

class Branch {
    public:
        Expression *ex; StatementWithoutLabel *st1; StatementWithoutLabel *st2;
        Branch(Expression *ex, StatementWithoutLabel *st1, StatementWithoutLabel *st2){
            this->ex = ex;
            this->st1 = st1;
            this->st2 = st2;
        };
};


#endif