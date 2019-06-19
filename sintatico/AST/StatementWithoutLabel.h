#ifndef STATEMENTWITHOUTLABEL_H
#define STATEMENTWITHOUTLABEL_H

#include <iostream>
#include <string>
#include <vector>
#include "Number.h"
#include "Assign.h"
#include "Declaration.h"
#include "Branch.h"
#include "Loop.h"

using namespace std;


class StatementWithoutLabel {
    public:
        Assign *assign;
        Declaration *declaration;
        Number *goto_;
        vector<Statement*> *statements;
        Branch *branch;
        Loop *loop;
        StatementWithoutLabel (Assign *assign, Declaration *declaration, 
        Number *goto_, vector<Statement*> *statements, Branch *branch, Loop *loop) {
            this->assign = assign;
            this->declaration = declaration;
            this->goto_ = goto_;
            this->statements = statements;
            this->branch = branch;
            this->loop = loop;
        };
};


#endif