#include <iostream>
#include <string>

#include "./sintatico/Parser.h"

using namespace std;

int main(int argc, char* argv[]) {

	if(argc < 6) {
		cout << "Help" << endl;
		cout << "Digite o nome dos arquivos na seguinte ordem"<< endl;
		cout << "- Nome do arquivo de Entrada" << endl;
		cout << "- Nome do arquivo de saida para a listagem dos Identificadores" << endl;
		cout << "- Nome do arquivo de saida dos Tokens" << endl;
		cout << "- Nome do arquivo de saida para a AST" << endl;
		cout << "- Nome do arquivo de saida para a Tabela de Simbolos" << endl;

	}
	else {
	   vector<string> entrada;
	   entrada.assign(argv + 1, argv + argc);

	   Parser *parser = new Parser("./configuracoes/Automaton.txt", "./configuracoes/Palavras_Reservadas.txt");

	   cout << "Compilando ..." << endl << endl;

	   parser->parsing("./testes/" + entrada[0], "./saida/" + entrada[1], "./saida/" + entrada[2], "./saida/" + entrada[3], "./saida/" + entrada[4]);

	   cout << endl << "fim." << endl;
	   delete parser;
	}

   return 0;
}

