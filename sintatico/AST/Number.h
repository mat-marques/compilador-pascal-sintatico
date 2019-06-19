#ifndef NUMBER_H
#define NUMBER_H

#include <iostream>
#include <string>
#include "Expression.h"

class Number{
    public:
        string value;
        Number (string value) { this->value = value; };
};

#endif