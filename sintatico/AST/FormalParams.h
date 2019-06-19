#ifndef FORMALPARMS_H
#define FORMALPARMS_H

#include <iostream>
#include <string>
#include <vector>
#include "Variable.h"

using namespace std;

class FormalParms {
    public:
        string op; vector<Variable*> *variables; string type;
        FormalParms(string op, vector<Variable*> *variables, string type){
            this->op = op;
            this->variables = variables;
            this->type = type;
        };
        
};


#endif