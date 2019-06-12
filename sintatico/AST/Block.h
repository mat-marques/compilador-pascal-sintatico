#ifndef BLOCk_H
#define BLOCK_H

#include <iostream>
#include <string>
#include <vector>
#include "Number.h"
#include "Procedure.h"
#include "Function.h"
#include "Type.h"
#include "Variable.h"

using namespace std;

class BlockType {
    public:
        Variable v; Type t;
        BlockType(Variable v, Type t) {
            this->v = v;
            this->t = t;
        };
};

class BlockVar {
    public:
        vector<Variable> v; Type t;
        BlockVar(vector<Variable> v, Type t) {
            this->v = v;
            this->t = t;
        };
};

class Block {
    public:
        vector<Number*> *labels;
        vector<BlockType*> *varTypes;
        vector<BlockVar*> *varVariables;
        vector<Procedure*> *procedures;
        vector<Function*> *functions;
        vector<Statement*> *statements;
        
        Block (vector<Number*> *labels, vector<BlockType*> *varTypes, vector<BlockVar*> *varVariables, vector<Procedure*> *procedures, vector<Function*> *functions, vector<Statement*> *statements) { 
            this->labels = labels;
            this->varTypes = varTypes;
            this->varVariables = varVariables;
            this->procedures = procedures;
            this->functions = functions;
            this->statements = statements;
        };
};


#endif