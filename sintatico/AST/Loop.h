#ifndef LOOP_H
#define LOOP_H

#include <iostream>
#include <string>
#include <vector>
#include "Expression.h"
#include "Statement.h"

using namespace std;

class Loop {
    public:
        Expression ex; Statement statement;
        Loop(Expression ex, Statement statement){
            this->ex = ex;
            this->statement = statement;
        }
};


#endif