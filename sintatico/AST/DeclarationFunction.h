#ifndef DECLARATIONFUNCTION_H
#define DECLARATIONFUNCTION_H

#include <iostream>
#include <string>
#include <vector>
#include "Variable.h"
#include "Block.h"
#include "FormalParams.h"

using namespace std;

class DeclarationFunction{
    public:
        Variable *functionName; vector<FormalParms*> *listParams; Variable *functionType; Block *block;
        DeclarationFunction(Variable *functionName, vector<FormalParms*>  *listParams, Variable *functionType, Block *block) {
            this->functionName = functionName;
            this->listParams = listParams;
            this->functionType = functionType;
            this->block = block;
        };
};


#endif