#include "AST.h"



char AST::show(const Op op) {
    const string operators = "*/-+";
    return operators[op];
}