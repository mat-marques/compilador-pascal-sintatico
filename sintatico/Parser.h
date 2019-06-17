#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <iterator>
#include "../lexico/Lexicon.h"
#include "./AST/Program.h"
#include "./AST/Number.h"
#include "./AST/DeclarationFunction.h"
#include "./AST/Statement.h"
#include "./AST/Branch.h"
#include "./AST/Loop.h"
#include "./AST/Term.h"
#include "./AST/Factor.h"
#include "./AST/Expression.h"
#include "./AST/Assign.h"
#include "./AST/Branch.h"
#include "./AST/StatementWithoutLabel.h"

using namespace std;

class Parser : public Lexicon{
  public:
    Program *ast;
    Parser(string automatonFileName, string reservedWordsFileName)
    : Lexicon(automatonFileName, reservedWordsFileName, 211, 50){};

  private:
    int currentToken = 0;
    void eat(string symbol);

    void programa();
    Block *bloco();
    vector<Number*> *parteDeDeclaracoesDeRotulos();
    vector<BlockType*> *parteDeDefinicoesDeTipos();
    BlockType *definicaoDeTipo();
    Type *tipo();
    vector<Number*> *indice();

    vector<BlockVar*> *parteDeDeclaracoesDeVariaveis();
    BlockVar *declaracoesDeVariaveis();
    vector<Variable*> *listaDeIdentificadores();
    vector<DeclarationFunction*> *parteDeDeclaracoesDeSubRotinas();
    DeclarationFunction *declaracaoDeProcedimento();
    DeclarationFunction *declaracaoDeFuncao();
    vector<Variable*> *parametrosFormais();
    vector<Variable*> *secoesDeParametrosFormais();
    vector<Statement*> *comandoComposto();
    Statement *comando();
    StatementWithoutLabel *comandoSemRotulo();
    Assign *atribuicoes();
    Declaration *chamadaDeProcedimentos();
    Number *desvios();
    Branch *comandoCondicional();
    Loop *comandoRepetitivo();
    vector<Expression*> *listaDeExpressoes();
    Expression *expressoes();
    string relacao();
    SimpleExpression *expressaoSimples();
    Term *termo();
    Factor *fator();
    vector<Expression*> *fatorAux();
    Declaration *variavel();
    Declaration *chamadaDeFuncao();

};

#endif