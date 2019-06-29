#include <iostream>
#include <string>

#include "./sintatico/Parser.h"

using namespace std;

int main() {

   string input = "teste0.txt", output = "teste0.txt", tokens = "teste0Tokens.txt";
   Parser *parser = new Parser("./configuracoes/Automaton.txt", "./configuracoes/Palavras_Reservadas.txt");

   // cout << "Digite o nome do arquivo de entrada: ";
   // cin >> input;

   // cout << "Digite o nome do arquivo de saida para a listagem dos identificadores: ";
   // cin >> output;

   // cout << "Digite o nome do arquivo de saida dos tokens: ";
   // cin >> tokens;
   
   cout << "Compilando ..." << endl << endl;

   parser->parsing("./testes/" + input, "./saida/" + output, "./saida/" + tokens);
   
   cout << "fim." << endl;
   delete parser;

   return 0;
}

