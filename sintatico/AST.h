#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;


class AST {
  public:
    class ASTNode {
      
    };

    /* Expression nodes */
    class Expression : public ASTNode {};

    class Id : public Expression {
      public:
        string id;
        Id (string id) { this->id = id; }
    };

    enum Op {
        MULTIPLY, DIVIDE, SUBTRACT, ADD
    };

    char show(const Op op);

    class Operator : public Expression {
      public:
        Op op;
        Expression left;
        Expression right;
        Operator (Op op, Expression left, Expression right) {
          this->op = op;
          this->left = left;
          this->right = right;
        }

    };

    class Number : public Expression {
      public:
        int n;
        Number (int n) { this->n = n; }
    };

    /* Statement nodes */
    class Statement : public ASTNode{
    };

    class Assign : public Statement {
      public:
        Id variable; Expression value;
        Assign (Id variable, Expression value) { this->variable = variable; this->value = value; }

    };

    class Block : public Statement {
      public:
        vector<Statement> statements;
        Block (Statement statements) { this->statements.push_back(statements); }
      
    };

    class Branch : public Statement {
      public:
        Expression predicate; Statement ifBranch; Statement elseBranch;
        Branch (Expression p, Statement a, Statement b) { predicate = p; ifBranch = a; elseBranch = b; }

    };

    class Loop : public Statement {
      public:
        Expression predicate; Statement body;
        Loop (Expression p, Statement body) { predicate = p; this->body = body; }

    };


};

#endif