#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>


using namespace std;

#include "Lexicon.h"

/*
	Construtor da classe.
*/
Lexicon::Lexicon(string automatonFileName, string reservedWordsFileName, int idHashSize, int rwHashSize){
	this->hashReservedWords = new HashTable(rwHashSize);
	this->hashIdentifiers = new HashTable(idHashSize);
	this->config_reserved_words(reservedWordsFileName);
	this->tokens = new list<Token *>();
	this->config_automaton(automatonFileName);
}

/*
	Faz o slip de uma string atraves do delimitador spliter.
*/
vector<string> *Lexicon::splitString(string input, char spliter) {
	vector<string> *l = new vector<string>();
	int start = 0, size = 0, end = 0;
	for(int i = 0; i < input.length(); i++){
		if(input[i] == spliter){
			size = start - i;
			if(size < 0) 
				size = size * (-1);
			if(size > 0) 
				l->push_back(input.substr(start, size));
			start = i + 1;
		}
		if(input[i] == '\n' || input[i] == '\r') {
			end = i;
			break;
		}
	}

	size = start - end;

	if(size < 0) 
		size = size * (-1);
	if(size > 0) 
		l->push_back(input.substr(start, size));

	return l;
}

/*
	Faz o processo de configurar a hash com as palavras reservadas.
*/
void Lexicon::config_reserved_words(string reservedWordsFileName){
	ifstream wordsFile;
	string word;
	wordsFile.open (reservedWordsFileName, ios_base::in);

	if(!wordsFile.is_open( ))
	{
		cout<<"Não foi possível abrir arquivo! Programa será terminado!\n";
		wordsFile.clear( ); //reseta o objeto leitura, para limpar memória do sistema
	}

	//Configuração da matrix e definição das variáveis de inicio
	while(!wordsFile.eof())
	{
		getline(wordsFile, word);
		this->hashReservedWords->insertItem(Item(word));
	}

	wordsFile.close();
}

/*
	Faz o processo de configurar o léxico.
*/
void Lexicon::config_automaton(string automatonFileName){
	ifstream automatonFile;
	string myString, s1, s2, s3;
	char op;
	vector<string> *l;
	automatonFile.open (automatonFileName, ios_base::in);

	if(!automatonFile.is_open( ))
	{
		cout<<"Não foi possível abrir arquivo! Programa será terminado!\n";
		automatonFile.clear( ); //reseta o objeto leitura, para limpar memória do sistema
		exit(0);
	}

	//Configuração da matrix e definição das variáveis de inicio
	while(!automatonFile.eof())
	{
		getline(automatonFile, myString);
		op = myString[0];
		switch(op)
		{
			case 'Q': //Q = Quantidade de estados
				s1 = myString.substr(2, myString.size());
				this->qtdStates = std::stoi(s1);
				matrix = this->getMatrix();
				break;
			case 'T': //T = Transicoes
				l = this->splitString(myString, ' ');
				this->configureMatrix(std::stoi((*l)[1]), std::stoi((*l)[2]), (*l)[3]);
				delete l;
				break;
			case 'I': //I = Estado Inicial
				s2 = myString.substr(2, myString.size());
				this->startState = std::stoi(s2);
				break;
			case 'F': //F = Estados Finais
				l = this->splitString(myString, ' ');
				for(int i = 1; i<l->size(); i++)
				{
					this->finalStates.push_back(std::stoi((*l)[i]));
				}
				delete l;
				break;
		}
			//cout<<myString<<"\n";
	}
	automatonFile.close();
}

/*
	Faz o processo de análise das cadeias de entrada.
*/
void Lexicon::process_lexicon(string inputFileName, string outPutHashFile, string tokensFileName){
	int line = 0;
	ifstream inputFile;
	string myString;
	bool verify;
	inputFile.open (inputFileName, ios_base::in); //Arquivo de entrada

	if(!inputFile.is_open())
	{
		cout<<"Não foi possível abrir arquivo! Programa será terminado!\n";
		inputFile.clear(); //reseta o objeto leitura, para limpar memória do sistema
		exit(0);
	}

	while(!inputFile.eof())
	{
		getline(inputFile, myString);
		//cout << myString << "\n";
		verify = this->checkString(myString, line);
		if (!verify)
			break;
		line++;
	}

	this->hashIdentifiers->show(outPutHashFile);

	this->deleteMatrix();
	this->hashIdentifiers->removeHashTable();
	this->hashReservedWords->removeHashTable();

	delete this->hashReservedWords;
	inputFile.close();
}

/*
	Cria a matrix de transições
*/
int ** Lexicon::getMatrix(){
	int **new_matrix = NULL;
	if (this->qtdStates > 0)
	{
		new_matrix = new int*[this->qtdStates];
		for (int i = 0; i < this->qtdStates; ++i)
		{
			new_matrix[i] = new int[256];
		}

		for(int i = 0; i < this->qtdStates; ++i)
		{
			for(int j = 0; j < 256; ++j)
			{
				new_matrix[i][j] = 0;
			}
		}
	}
	return new_matrix;
}

/*
	Deleta uma matrix.
*/
void Lexicon::deleteMatrix(){
	for (int i = 0; i < this->qtdStates; ++i)
	{
		delete[] this->matrix[i];
	}
	delete[] this->matrix;
}

/*
	Configura a matrix de transições.
*/
void Lexicon::configureMatrix(int currentState, int nextState, string myString){
	if(myString == "QC") {
		for(int i = 0; i < 256; i++) //Todos os caracteres
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else if(myString == "a-z"){
		for(int i = 'a'; i <= 'z'; i++) //a-z
		{
			this->matrix[currentState-1][i] = nextState;
		}
		for(int i = 'A'; i <= 'Z'; i++) //A-Z
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else if (std::regex_match (myString, std::regex("[a-z]-[a-z]"))) {
		for(int i = myString[0]; i<=myString[2]; i++)
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else if (myString == "0-9") {
		for(int i = myString[0]; i<=myString[2]; i++) //0-9
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else if (std::regex_match (myString, std::regex("[0-9]-[0-9]"))) {
		for(int i = myString[0]; i<=myString[2]; i++)
		{
			this->matrix[currentState-1][i] = nextState;
		}
	}
	else 
	{
		this->matrix[currentState-1][myString[0]] = nextState;
	}
}

/*
	Retorna o proximo estado ao ler o caracter c do estado currentState.
*/
int Lexicon::_nextState(int currentState, char c){
	int next, i;
	i = c;
	next = this->matrix[currentState-1][i];
	return next;
}

/*
	Verifica se o estado atual está contido no conjunto de estados finais.
*/
bool Lexicon::_finalState(int currentState){
	for(int i = 0; i<this->finalStates.size(); i++)
	{
		if(this->finalStates[i] == currentState)
		{
			return true;
		}
	}
	return false;
}

/*
	Retorna o token mediante o estado final.
*/
int Lexicon::getToken(int posic){
	if(posic == 2) {
		return 1;
	}
	else if (posic == 3 || posic == 5) {
		//cout << "NÚMERO" << "\n";
		return 2;
	}
	else if (posic == 6 || posic == 8 || posic == 10 || posic == 12 || posic == 14 || posic == 16 ||posic == 18 || posic == 20) {
		//cout << "SÍMBOLO ESPECIAL" << "\n";
		return 3;
	}
	else if (posic == 7 || posic == 9 || posic == 11 || posic == 13 || posic == 15 || posic == 17 || posic == 19) {
		//cout << "SÍMBOLO ESPECIAL COMPOSTO" << "\n";
		return 4;
	}
	else {
		return 0;
	} 
}

/*
	Retorna o erro léxico mostrando o erro a linha e a coluna.
*/
bool Lexicon::error(int state, string error, int line, int column) {
	//cout << "-> " << error << endl;
	if(error != " " && error != "\n" && error != "\r"  && error != "\0"){
		if(state == 21) {
			cout << "erro léxico:linha: "<< line << " :coluna: "<< column << " :símbolo não reconhecido: "<< error << "\n";
			return true;
		}
		if(state == 22) {
			cout << "erro léxico:linha: "<< line << " :coluna: "<< (column - error.size()) << " :erro de identificador: " << error << "\n";
			return true;
		}
	}
	return false;
}

/*
	Verifica se uma cadeia de caracteres é aceita pelo léxico.
*/
bool Lexicon::checkString(string myString, int line){
	int current = this->startState, finalState = 0, last_terminal = 0, index = 0, start = 0, i = 0, token = 0;
	int aux;
	bool tf = false, wt = false;
	while(true)
	{
		//Busca o próximo estado da transição do autômato
		//cout<< myString[index];
		current = _nextState(current, myString[index]);
		//cout << " " << current << endl;

		if(_finalState(current)) //Verifica se a transição ocorreu
		{
			last_terminal = index;
			finalState = current;
		}

		if(current == 0){
			//Calcula o tamanho da string
			aux = start - index;
			if(aux < 0)
				aux = aux * (-1);
		
			//verifica se não é um erro
			if(finalState != 0 && finalState != 21 && finalState != 22){
				token = getToken(finalState);
				if(token >= 1 && token <= 4){
					switch(token){
						case 1:
							//Verifica se é palavra reservada
							//cout << myString.substr(start, aux) << "\n";
							if(this->hashReservedWords->searchItem2(myString.substr(start, aux))){
								this->tokens->push_back(new Token("PALAVRA RESERVADA", myString.substr(start, aux), line, start));
								//cout << "PALAVRA RESERVADA" << endl;
							} else {
								// Verifica se o id já existe na hashTable
								if(!this->hashIdentifiers->searchItem2(myString.substr(start, aux))){
									this->hashIdentifiers->insertItem(Item(myString.substr(start, aux)));
									//cout << "IDENTIFICADOR" << endl;
								}
								this->tokens->push_back(new Token("IDENTIFICADOR", myString.substr(start, aux), line, start));
							}
							break;
						case 2:
							this->tokens->push_back(new Token("NUMERO", myString.substr(start, aux), line, start));
							break;
						case 3:
							this->tokens->push_back(new Token("SIMBOLO ESPECIAL", myString.substr(start, aux), line, start));
							break;
						case 4:
							this->tokens->push_back(new Token("SIMBOLO ESPECIAL COMPOSTO", myString.substr(start, aux), line, start));
							break;
					}
				}

				start = last_terminal + 1;
			}
			if(finalState == 21 || finalState == 22) {
				aux = start - index;
				if(aux < 0)
					aux = aux * (-1);
				if(this->error(finalState, myString.substr(start, aux), line, index)){
					start = last_terminal + 1;
					return false;
				}
				else{
					start++;
				}
			}
			current = this->startState;
			finalState = 0;
			index = start-1;
			aux = 0;
		}

		index++;
		if(start == myString.length())
			break;
	}
	return true;
}

Token *Lexicon::getTokenList(int index) {
	int count = 0;
	for (std::list<Token*>::iterator it = this->tokens->begin(); it != this->tokens->end(); ++it){
    	if(count == index) {
			return *it;
		}
		count++;
	}
	return NULL;
}