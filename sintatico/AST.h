#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Variable;
class Number;
class Program;
class Assign;
class Declaration;
class SimpleExpression;
class Expression;
class Block;
class DeclarationFunction;
class Term;
class Factor;
class Type;
class Statement;
class StatementWithoutLabel;
class FormalParms;

class Variable{
    public:
        string variable;
        Variable (string variable) { this->variable = variable; };
};

class Number{
    public:
        string value;
        Number (string value) { this->value = value; };
};

class Program {
    public:
        Variable *programName; vector<Variable*> *listVariables; Block *block;
        Program(Variable *programName, vector<Variable*> *listVariables, Block *block){
            this->programName = programName;
            this->listVariables = listVariables;
            this->block = block;
        };
  
};

class Assign {
    public:
        Declaration *dec; Expression *ex;
        Assign(Declaration *dec, Expression *ex) {
            this->dec = dec;
            this->ex = ex;
        };
};

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

class Branch {
    public:
        Expression *ex; StatementWithoutLabel *st1; StatementWithoutLabel *st2;
        Branch(Expression *ex, StatementWithoutLabel *st1, StatementWithoutLabel *st2){
            this->ex = ex;
            this->st1 = st1;
            this->st2 = st2;
        };
};

class Declaration {
    public:
        Variable *v; vector<Expression*> *ex;
        Declaration(Variable *v, vector<Expression*> *ex) {
            this->v = v;
            this->ex = ex;
        };
};

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

class SimpleExpression {
    public:
        string op; Term* t; SimpleExpression* simpleExpressions;
        SimpleExpression(string op, Term *t, SimpleExpression *simpleExpressions) {
            this->op = op;
            this->t = t;
            this->simpleExpressions = simpleExpressions;
        };
};

class Expression {
    public:
        SimpleExpression *ex1; string op; SimpleExpression *ex2;
        Expression(SimpleExpression *ex1, string op, SimpleExpression *ex2) {
            this->ex1 = ex1;
            this->op = op;
            this->ex2 = ex2;
        };
};

class Factor {
    public:
        Declaration *d;
        Number *n;
        Expression *ex;
        Factor *f;
        Factor(Declaration *d, Number *n, Expression *ex, Factor *f) {
            this->d = d;
            this->n = n;
            this->ex = ex;
            this->f = f;
        };
};

class Term {
    public:
        Factor *f; string op; Term *t;
        Term(Factor *f, string op, Term *t){ 
            this->f = f;
            this->op = op;
            this->t = t;
        };
};

class FormalParms {
    public:
        string op; vector<Variable*> *variables; string type;
        FormalParms(string op, vector<Variable*> *variables, string type){
            this->op = op;
            this->variables = variables;
            this->type = type;
        };
        
};

class Loop {
    public:
        Expression *ex; StatementWithoutLabel *statement;
        Loop(Expression *ex, StatementWithoutLabel *statement){
            this->ex = ex;
            this->statement = statement;
        }
};

class Type {
    public:
        Variable *variable; vector<Number*> *numbers; Type *types;
        Type(Variable *variable, vector<Number*> *numbers, Type *types) {
            this->variable = variable;
            this->numbers = numbers;
            this->types = types;
        };
};

class StatementWithoutLabel {
    public:
        Assign *assign;
        Declaration *declaration;
        Number *goto_;
        vector<Statement*> *statements;
        Branch *branch;
        Loop *loop;
        StatementWithoutLabel (Assign *assign, Declaration *declaration, 
        Number *goto_, vector<Statement*> *statements, Branch *branch, Loop *loop) {
            this->assign = assign;
            this->declaration = declaration;
            this->goto_ = goto_;
            this->statements = statements;
            this->branch = branch;
            this->loop = loop;
        };
};

class Statement {
    public:
        Number *label; StatementWithoutLabel *st;
        Statement(Number *label, StatementWithoutLabel *st) {
            this->label = label;
            this->st = st;
        };
};




#endif