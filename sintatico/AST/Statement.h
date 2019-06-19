#ifndef STATEMENT_H
#define STATEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include "Number.h"
#include "StatementWithoutLabel.h"

using namespace std;


class Statement {
    public:
        Number *label; StatementWithoutLabel *st;
        Statement(Number *label, StatementWithoutLabel *st) {
            this->label = label;
            this->st = st;
        };
};


#endif