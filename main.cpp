#include <iostream>
#include <string>

#include "./sintatico/Parser.h"

using namespace std;

int main() {

   string input = "teste0.txt", output = "teste0.txt", tokens = "teste0Tokens.txt", ast = "ast.txt", tabelaDeSimbolos = "tabelaDeSimbolos.txt";
   Parser *parser = new Parser("./configuracoes/Automaton.txt", "./configuracoes/Palavras_Reservadas.txt");

//    cout << "Digite o nome do arquivo de Entrada: ";
//    cin >> input;
//
//    cout << "Digite o nome do arquivo de saida para a listagem dos Identificadores: ";
//    cin >> output;
//
//    cout << "Digite o nome do arquivo de saida dos Tokens: ";
//    cin >> tokens;
//
//    cout << "Digite o nome do arquivo de saida para a AST: ";
//	  cin >> ast;
//
//	  cout << "Digite o nome do arquivo de saida para a Tabela de Simbolos: ";
//	  cin >> tabelaDeSimbolos;
   
   cout << "Compilando ..." << endl << endl;

   parser->parsing("./testes/" + input, "./saida/" + output, "./saida/" + tokens, "./saida/" + ast, "./saida/" + tabelaDeSimbolos);
   
   cout << "fim." << endl;
   delete parser;

   return 0;
}

