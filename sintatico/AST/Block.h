#ifndef BLOCk_H
#define BLOCK_H

#include <iostream>
#include <string>
#include <vector>
#include "Number.h"
#include "Type.h"
#include "Variable.h"
#include "Statement.h"

using namespace std;

class BlockType {
    public:
        Variable *v; Type *t;
        BlockType(Variable *v, Type *t) {
            this->v = v;
            this->t = t;
        };
};

class BlockVar {
    public:
        vector<Variable*> *v; Type *t;
        BlockVar(vector<Variable*> *v, Type *t) {
            this->v = v;
            this->t = t;
        };
};

class Block {
    public:
        vector<Number*> *labels;
        vector<BlockType*> *varTypes;
        vector<BlockVar*> *varVariables;
        vector<DeclarationFunction*> *rotinas;
        vector<Statement*> *statements;
        
        Block (vector<Number*> *labels, vector<BlockType*> *varTypes, vector<BlockVar*> *varVariables, vector<DeclarationFunction*> *rotinas, vector<Statement*> *statements) { 
            this->labels = labels;
            this->varTypes = varTypes;
            this->varVariables = varVariables;
            this->rotinas = rotinas;
            this->statements = statements;
        };
};


#endif