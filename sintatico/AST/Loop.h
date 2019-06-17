#ifndef LOOP_H
#define LOOP_H

#include <iostream>
#include <string>
#include <vector>
#include "Expression.h"
#include "StatementWithoutLabel.h"

using namespace std;

class Loop {
    public:
        Expression *ex; StatementWithoutLabel *statement;
        Loop(Expression *ex, StatementWithoutLabel *statement){
            this->ex = ex;
            this->statement = statement;
        }
};


#endif