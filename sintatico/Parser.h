#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <sstream> 
#include "../lexico/Lexicon.h"
#include "./AST/Program.h"
#include "./AST/Number.h"
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
    void definicaoDeTipo();
    void tipo();
    void indice();
    void parteDeDeclaracoesDeVariaveis();
    void declaracoesDeVariaveis();
    void listaDeIdentificadores();
    void parteDeDeclaracoesDeSubRotinas();
    void declaracaoDeProcedimento();
    void declaracaoDeFuncao();
    void parametrosFormais();
    void secoesDeParametrosFormais();
    void comandoComposto();
    void comando();
    void comandoSemRotulo();
    void atribuicoes();
    void chamadaDeProcedimentos();
    void desvios();
    void comandoCondicional();
    void comandoRepetitivo();
    void listaDeExpressoes();
    void expressoes();
    void relacao();
    void expressaoSimples();
    void termo();
    void fator();
    void fatorAux();
    void variavel();
    void chamadaDeFuncao();

};

#endif