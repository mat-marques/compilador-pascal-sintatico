#ifndef BRANCH_H
#define BRANCH_H

#include <iostream>
#include <string>
#include <vector>
#include "Number.h";
#include "Assign.h"
#include "Declaration.h"
#include "Branch.h"
#include "Loop.h"

using namespace std;


class Statement {
    public:
        Number label;
        Assign assign;
        Declaration declaration;
        Number goto_;
        vector<Statement> statements;
        Branch branch;
        Loop loop;
        Statement(Number label, Assign assign, Declaration declaration, 
        Number goto_, vector<Statement> statements, Branch branch, Loop loop) {
            this->label = label;
            this->assign = assign;
            this->declaration = declaration;
            this->goto_ = goto_;
            this->statements = statements;
            this->branch = branch;
            this->loop = loop;
        };
};


#endif