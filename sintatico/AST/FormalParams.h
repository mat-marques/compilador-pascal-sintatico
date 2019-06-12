#ifndef FORMALPARMS_H
#define FORMALPARMS_H

#include <iostream>
#include <string>
#include <vector>
#include <Variable>

using namespace std;

class FormalParms {
    public:
        string wr; vector<Variable> variables; Variable v;
        FormalParms(string wr, vector<Variable> variables, Variable v){
            this->wr = wr;
            this->variables = variables;
            this->v = v;
        };
        
};


#endif