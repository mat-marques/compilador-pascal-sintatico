#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <string>
#include <vector>
#include "Variable.h"
#include "Block.h"
using namespace std;


class Program {
    public:
        Variable *programName; vector<Variable*> *listVariables; Block *block;
        Program(Variable *programName, vector<Variable*> *listVariables, Block *block){
            this->programName = programName;
            this->listVariables = listVariables;
            this->block = block;
        };
  
};

#endif