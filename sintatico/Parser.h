#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <iterator>
#include <fstream>
#include "../lexico/Lexicon.h"
#include "AST.h"


using namespace std;

class Parser : public Lexicon{
  public:
    Parser(string automatonFileName, string reservedWordsFileName)
    : Lexicon(automatonFileName, reservedWordsFileName, 211, 50){};
    void parsing(string inputFileName, string outPutHashFile, string tokensFileName, string astFileName, string symbolTableFileName);

  private:
    int currentToken = 0;
    bool syntaxError = false;
    Program *ast = NULL;
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
    vector<FormalParms*> *parametrosFormais();
    FormalParms *secoesDeParametrosFormais();
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
    Read *readParser();
    Write *writeParser();
};

#endif
