#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include "../lexico/Lexicon.h"

using namespace std;

class Parser : public Lexicon{
  public:
    Parser(string automatonFileName, string reservedWordsFileName)
    : Lexicon(automatonFileName, reservedWordsFileName, 211, 50){};

  private:
    int currentToken = 0;
    void eat(string symbol);
    void programa();
    void bloco();
    void parteDeDeclaracoesDeRotulos();
    void parteDeDefinicoesDeTipos();
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
    void variavel();
    void chamadaDeFuncao();
    void numero();
    void digito();
    void identificador();
    void letra();

};

#endif