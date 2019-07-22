#include "Parser.h"

string Parser::eat(string symbol) {
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t != NULL){
		cout << t->var_value << endl;
		if(t->var_value.compare(symbol) == 0) {
			this->currentToken = this->currentToken + 1;
			return symbol;
		}
    }
    return "";
}

string Parser::eatVarType() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t != NULL){
    	cout << t->var_value << endl;
		if(t->var_value.compare("integer") == 0) {
			this->currentToken = this->currentToken + 1;
			return "integer";
		}
		if(t->var_value.compare("real") == 0) {
			this->currentToken = this->currentToken + 1;
			return "real";
		}
		if(t->var_value.compare("boolean") == 0) {
			this->currentToken = this->currentToken + 1;
			return "boolean";
		}
		if(t->var_value.compare("byte") == 0) {
			this->currentToken = this->currentToken + 1;
			return "byte";
		}
		if(t->var_value.compare("char") == 0) {
			this->currentToken = this->currentToken + 1;
			return "char";
		}
		if(t->var_value.compare("string") == 0) {
			this->currentToken = this->currentToken + 1;
			return "string";
		}
    }

    return "";
}

void Parser::eatFinal() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t != NULL){
		cout << "erro sintático: "<< t->var_value << " na linha: " << t->row << " e coluna: "<< t->col << endl;
		this->syntaxError = true;
    }
}

string Parser::eatType(string symbol) {
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t != NULL){
		cout << t->var_value << endl;
		if(t->var_type.compare(symbol) == 0) {
			this->currentToken = this->currentToken + 1;
			return t->var_value;
		}
    }
    return "";
}

void Parser::parsing(string inputFileName, string outPutHashFile, string tokensFileName, string astFileName, string symbolTableFileName){

	bool e = this->process_lexicon(inputFileName, outPutHashFile, tokensFileName);

	if(e){
		Lexicon::hashIdentifiers = new HashTable(211);

		programa();

		eatFinal();

		//Printar a AST
		if(!this->syntaxError){
			this->ast->printProgram(astFileName);
			this->hashIdentifiers = this->ast->createSymbolTable();
			this->hashIdentifiers->show2(symbolTableFileName);
			this->ast->removeProgram();
			this->hashIdentifiers->removeHashTable();
			delete this->hashIdentifiers;
			delete this->ast;
		}
	}

}

void Parser::programa() {
    Variable *programName = NULL;
    vector<Variable*> *listVariables = NULL;
    Block *block = NULL;
    string aux;

    if(eat("program") != "") {
    	aux = eatType("IDENTIFICADOR");

    	if(aux != ""){
			programName = new Variable(aux);

			if(eat("(") != ""){
				listVariables = listaDeIdentificadores();

				if(listVariables != NULL){

					if(eat(")") != "" && eat(";") != ""){

						block = bloco();
						if(block != NULL){

							if(eat(".") != ""){

								this->ast = new Program(programName, listVariables, block);

							}
						}
					}
				}
			}
    	}
    }
    else {
    	this->ast = NULL;
    }

}

Block *Parser::bloco(){
    vector<Number*> *labels = new vector<Number*>();
    vector<BlockType*> *types = new vector<BlockType*>();
	vector<BlockVar*> *variables = new vector<BlockVar*>();
	vector<DeclarationFunction*> *rotinas = new vector<DeclarationFunction*>();
	vector<Statement*> *statements = new vector<Statement*>();
	vector<Number*> *l = NULL;
	vector<BlockType*> *t = NULL;
	vector<BlockVar*> *v = NULL;
	vector<DeclarationFunction*> *r = NULL;
	vector<Statement*> *s = NULL;

    while(true) {
		l = parteDeDeclaracoesDeRotulos();
		if(l != NULL){
			labels->insert(
				labels->end(),
				std::make_move_iterator(l->begin()),
				std::make_move_iterator(l->end())
			);
		}

		t = parteDeDefinicoesDeTipos();
		if(t != NULL){
			types->insert(
				types->end(),
				std::make_move_iterator(t->begin()),
				std::make_move_iterator(t->end())
			);
		}

		v = parteDeDeclaracoesDeVariaveis();
		if(v != NULL){
			variables->insert(
				variables->end(),
				std::make_move_iterator(v->begin()),
				std::make_move_iterator(v->end())
			);
		}

		r = parteDeDeclaracoesDeSubRotinas();
		if(r != NULL){
			rotinas->insert(
				rotinas->end(),
				std::make_move_iterator(r->begin()),
				std::make_move_iterator(r->end())
			);
		}

		s = comandoComposto();
		if(s != NULL){
			statements->insert(
				statements->end(),
				std::make_move_iterator(s->begin()),
				std::make_move_iterator(s->end())
			);
		}

		if(l == NULL && t == NULL && v == NULL && r == NULL && s == NULL)
			break;

		l = NULL; t = NULL; v = NULL; r = NULL; s = NULL;
    }

    if(labels == NULL && types == NULL && variables == NULL && rotinas == NULL && statements == NULL)
    	return NULL;

    return new Block(labels, types, variables, rotinas, statements);
}

vector<Number*> *Parser::parteDeDeclaracoesDeRotulos() {
	    vector<Number*> *labels = new vector<Number*>();
	    string aux;
	    if(eat("label") != "") {
	    	aux = eatType("NUMERO");

	        if(aux != ""){

	            labels->push_back(new Number(aux));

	            int token;
	            while(true){
	                if(eat(",") != "") {
	                	token = this->currentToken;

	                	aux = eatType("NUMERO");
	                    if(aux != ""){

	                        labels->push_back(new Number(aux));

	                    }
	                    else {
	                    	this->currentToken = token;
	                    	break;
	                    }

	                } else {
	                    break;
	                }
	            }

	        }
	        else {
	        	delete labels;
	            return NULL;
	        }

	        if(eat(";") != "") {
	        	return labels;
	        }
	    }


	    return NULL;
}


vector<BlockType*> *Parser::parteDeDefinicoesDeTipos() {
    vector<BlockType*> *types = new vector<BlockType*>();
    BlockType *type;

    if(eat("type") != "") {

        type = definicaoDeTipo();
        if(type != NULL){

        	types->push_back(type);
        	if(eat(";") == ""){
        		delete types;
        		return NULL;
        	}
        }

        //ParteDeDefinicoesDeTiposAux
        while(true){

			type = definicaoDeTipo();
			if(type != NULL){
				types->push_back(type);

	        	if(eat(";") == ""){
	            	delete types;
	            	return NULL;
	            }

			}else
				break;

        }

        return types;
    }

    return NULL;
}

BlockType *Parser::definicaoDeTipo() {
    Type *ntype = NULL;
    Variable *variable = NULL;
    string aux;

    aux = eatType("IDENTIFICADOR");
    if(aux != ""){

        variable = new Variable(aux);

        if(eat("=") != "") {

            ntype = tipo();

            if(ntype != NULL)
            	return new BlockType(variable, ntype);
        }
    }

    return NULL;
}

Type *Parser::tipo() {
    vector<Number*> *indices = NULL;
    Variable *variable = NULL;
    Type *newtype = NULL;
    string aux;
    int token;

	aux = eatVarType();
	if(aux != "") {

		variable = new Variable(aux);
		return new Type(variable, indices, newtype);
    }
    else if(eat("array") != "") {

        if(eat("[") != "") {

            indices = indice();

            while(true){
            	token = this->currentToken;
                if(eat(",") != "") {
                    vector<Number*> *indicesAux = indice();
                    if(indicesAux != NULL) {
						indices->insert(
							indices->end(),
							std::make_move_iterator(indicesAux->begin()),
							std::make_move_iterator(indicesAux->end())
						);
                    } else {
                    	this->currentToken = token;
                    	break;
                    }
                } else {
                    break;
                }
            }

            if(eat("]") != "") {

                if(eat("of") != "") {

                    newtype = tipo();

                    if(newtype != NULL)
                    	return new Type(variable, indices, newtype);

                }

            }

        }
    }

    return NULL;
}


vector<Number*> *Parser::indice() {
    vector<Number*> *indices = new vector<Number*>();
    string aux;

    aux = eatType("NUMERO");
    if(aux != ""){
    	indices->push_back(new Number(aux));

        if(eat("..") != "") {

        	aux = eatType("NUMERO");
            if(aux != ""){

                indices->push_back(new Number(aux));

                return indices;

            }
        }
    }
    return NULL;
}


vector<BlockVar*> *Parser::parteDeDeclaracoesDeVariaveis() {
    vector<BlockVar*> *listV = new vector<BlockVar*>();
    BlockVar *bt;

    if(eat("var") != "") {
        bt = declaracoesDeVariaveis();

        if(bt != NULL){
        	listV->push_back(bt);
			if(eat(";") == ""){
				delete listV;
				return NULL;
			}
        }else {
            delete listV;
        	return NULL;
        }

        while(true){
			bt = declaracoesDeVariaveis();

			if(bt != NULL){
				listV->push_back(bt);

				if(eat(";") == ""){
	                break;
	            }

			} else {
				break;
			}

        }
    }
    else {
        delete listV;
        return NULL;
    }
    return listV;
}


BlockVar *Parser::declaracoesDeVariaveis() {
    vector<Variable*> *listaID = NULL;
    Type *p;

    listaID = listaDeIdentificadores();
    if(listaID != NULL && eat(":") != "") {
        p = tipo();
        if(p == NULL){
        	delete listaID;
        	return NULL;
        }

        return new BlockVar(listaID, p);
    }

    return NULL;
}


vector<Variable*> *Parser::listaDeIdentificadores(){
    vector<Variable*> *listVariables = new vector<Variable*>();
    string aux;

    aux = eatType("IDENTIFICADOR");
    if(aux != ""){
        listVariables->push_back(new Variable(aux));
        int token;

        while(true){
        	token = this->currentToken;
            if(eat(",") != "") {
            	aux = eatType("IDENTIFICADOR");
                if(aux != ""){

                    listVariables->push_back(new Variable(aux));

                }
                else {
                	this->currentToken = token;
                	break;
                }

            } else {
                break;
            }
        }
    }
    else {
    	delete listVariables;
        return NULL;
    }
    return listVariables;
}


vector<DeclarationFunction*> *Parser::parteDeDeclaracoesDeSubRotinas(){
    vector<DeclarationFunction*> *rotinas = new vector<DeclarationFunction*>();

    while(true) {

		DeclarationFunction *p = declaracaoDeProcedimento();
		if(p != NULL) {
			rotinas->push_back(p);

			if(eat(";") == "") {
				delete rotinas;
				return NULL;
			}
		}

		DeclarationFunction *f = declaracaoDeFuncao();
		if(f != NULL) {
			rotinas->push_back(f);

			if(eat(";") == "") {
				delete rotinas;
				return NULL;
			}
		}

        if(p == NULL && f == NULL)
        	break;

    }

    if(rotinas->size() == 0) {
        delete rotinas;
        return NULL;
    }

    return rotinas;
}


DeclarationFunction *Parser::declaracaoDeProcedimento(){
    Variable *functionName; vector<FormalParms*> *listParams; Block *block;
    string aux;
    if(eat("procedure") != "") {
    	aux = eatType("IDENTIFICADOR");
        if(aux != ""){

            functionName = new Variable(aux);

            listParams = parametrosFormais();

            if(listParams != NULL && eat(";") != "") {

                block = bloco();

                if(block != NULL) {
                	return new DeclarationFunction(functionName, listParams, NULL, block);
                }
            }
        }
    }

    return NULL;
}


DeclarationFunction *Parser::declaracaoDeFuncao(){
    Variable *functionName; vector<FormalParms*> *listParams; Variable *functionType; Block *block;
    string aux;

    if(eat("function") != "") {
    	aux = eatType("IDENTIFICADOR");

        if(aux != ""){

            functionName = new Variable(aux);

            listParams = parametrosFormais();

            if(listParams != NULL && eat(":") != "") {

            	aux = eatVarType();
				if(aux != "") { //Tipo do elemento
					functionType = new Variable(aux);

					if(eat(";") != "") {

						block = bloco();

						if(block != NULL) {
							return new DeclarationFunction(functionName, listParams, functionType, block);
						}
					}
				}

            }
        }
    }

    return NULL;
}


vector<FormalParms*> *Parser::parametrosFormais(){
	FormalParms *p;
    vector<FormalParms*> *params = new vector<FormalParms*>();

    if(eat("(") != "") {

        p = secoesDeParametrosFormais();

        if(p != NULL){
        	params->push_back(p);

        	int token;
			while(true){
				token = this->currentToken;
				if(eat(",") != "") {

					p = secoesDeParametrosFormais();
					if(p != NULL){
						params->push_back(p);
					}
	                else {
	                	this->currentToken = token;
	                	break;
	                }

				} else {
					break;
				}
			}
        }

        if(eat(")") != "") {
        	return params;
        }
    }

    return NULL;
}


FormalParms *Parser::secoesDeParametrosFormais(){

    string op = "", type = "", aux;
    vector<Variable*> *params = NULL;

	if(eat("var") != ""){

		op = "var";
	}

	params = listaDeIdentificadores();

	if(params != NULL && eat(":") != "") {

    	type = eatVarType();
		if(type != "") {

			return new FormalParms(op, params, type);

		}
	}



    if(eat("function") != "") {

		op = "function";
		params = listaDeIdentificadores();

		if(params != NULL && eat(":") != "") {

			aux = eatType("IDENTIFICADOR");
			if(aux != ""){

				type = aux;
				return new FormalParms(op, params, type);

			}
		}

    }

    if(eat("procedure") != "") {

        op = "procedure";
        params = listaDeIdentificadores();

        if(params != NULL)
            return new FormalParms(op, params, type);
    }

    return NULL;
}


vector<Statement*> *Parser::comandoComposto(){
    vector<Statement*> *st = NULL;
    Statement*s = NULL;

    if(eat("begin") != "") {
        st = new vector<Statement*>();

        s = comando();
        if(s != NULL){
        	if(s->st == NULL)
        		return NULL;

        	st->push_back(s);

        	if(eat(";") == "") {
        		delete st;
        		return NULL;
        	}

            while(true){
				s = comando();
				if(s != NULL){
					st->push_back(s);
		        	if(eat(";") == "") {
		        		delete st;
		        	}
				} else {
					break;
                }
            }
        }
        else {
            delete st;
        }

        if(eat("end") != "") {
        	return st;
        }
    }
    return NULL;
}


Statement *Parser::comando(){
    Number *n = NULL; StatementWithoutLabel *st = NULL;
    string aux;

    aux = eatType("NUMERO");
    if(aux != ""){

        n = new Number(aux);

        if(eat(":") != "") {

            st = comandoSemRotulo();
            if(st != NULL)
                return new Statement(n , st);
        }

    } else {
        st = comandoSemRotulo();
        if(st != NULL)
            return new Statement(n , st);
    }

    return NULL;
}


StatementWithoutLabel *Parser::comandoSemRotulo(){

    Assign *assign = NULL;
    Declaration *declaration = NULL;
    Number *goto_ = NULL;
    vector<Statement*> *statements = NULL;
    Branch *branch = NULL;
    Loop *loop = NULL;
    Read *read = NULL;
    Write *write = NULL;

    int token = this->currentToken;

	declaration = chamadaDeProcedimentos();
	if(declaration != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);
	else
		this->currentToken = token;

	assign = atribuicoes();
	if(assign != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);
	else
		this->currentToken = token;

	goto_ = desvios();
	if(goto_ != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);

	statements = comandoComposto();
	if(statements != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);

	branch = comandoCondicional();
	if(branch != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);

	loop = comandoRepetitivo();
	if(loop != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);

	read = readParser();
	if(read != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);

	write = writeParser();
	if(write != NULL)
		return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);

    return NULL;
}


Assign *Parser::atribuicoes(){

    Expression *ex;

    Declaration *dec = NULL;

    dec = variavel();

    if(dec != NULL && eat(":=") != "") {

        ex = expressoes();

        if(ex != NULL){
            return new Assign(dec, ex);
        }

    }

    return NULL;
}

Declaration *Parser::chamadaDeProcedimentos(){
   string aux;
   Variable *v = NULL;
   vector<Expression*> *exs = NULL;

   aux = eatType("IDENTIFICADOR");
    if(aux != ""){

        v = new Variable(aux);

        if(eat("(") != "") {

            exs = listaDeExpressoes();

            if(eat(")") != "") {

                return new Declaration(v, exs);
            }
        }
    }

    return NULL;
}

Number *Parser::desvios(){
    string aux;

    if(eat("goto") != "") {

    	aux = eatType("NUMERO");
        if(aux != ""){

            return new Number(aux);

        }
    }

    return NULL;
}

Branch *Parser::comandoCondicional(){

    Expression *ex = NULL; StatementWithoutLabel *st1 = NULL; StatementWithoutLabel *st2 = NULL;

    if(eat("if") != "") {

        ex = expressoes();

        if (ex == NULL)
        	return NULL;

        if(eat("then") != "") {
            st1 = comandoSemRotulo();

            if (st1 == NULL)
            	return NULL;

            if(eat("else") != "") {
                st2 = comandoSemRotulo();

                if(st2 != NULL){
                	return new Branch(ex, st1, st2);
                }
            }

            return new Branch(ex, st1, st2);
        }

    }

    return NULL;
}

Loop *Parser::comandoRepetitivo(){

    Expression *ex = NULL; StatementWithoutLabel *st1 = NULL;

    if(eat("while") != "") {

        ex = expressoes();

        if (ex == NULL)
        	return NULL;

        if(eat("do") != "") {

            st1 = comandoSemRotulo();
            if (st1 == NULL)
            	return NULL;

            return new Loop(ex, st1);
        }

    }

    return NULL;
}

vector<Expression*> *Parser::listaDeExpressoes(){
    vector<Expression*> *listEx = new vector<Expression*>();
    Expression *ex = NULL;
    int token;

    ex = expressoes();

    if(ex != NULL)
        listEx->push_back(ex);
    else
    	return NULL;

    ex = NULL;

    while(true){

        if(eat(",") != "") {
        	token = this->currentToken;

            ex = expressoes();

            if(ex != NULL){
                listEx->push_back(ex);
            }
            else {
            	this->currentToken = token;
            	break;
            }

            ex = NULL;
        } else {
            break;
        }
    }

    if(listEx->size() == 0){
        delete listEx;
        return NULL;
    }
    return listEx;
}

Expression *Parser::expressoes(){
    SimpleExpression *simpleEx1 = expressaoSimples();
    string s = relacao();
    SimpleExpression *simpleEx2 = expressaoSimples();
    if(simpleEx1 == NULL)
        return NULL;

    return new Expression(simpleEx1, s, simpleEx2);
}

string Parser::relacao(){
    if(eat("=") != "") {
        return "=";
    } else if(eat("<>") != "") {
        return "<>";
    } else if(eat("<") != "") {
        return "<";
    } else if(eat("<=") != "") {
        return "<=";
    } else if(eat(">") != "") {
        return ">";
    } else if(eat(">=") != "") {
        return ">=";
    } else {
        return "";
    }
}

SimpleExpression *Parser::expressaoSimples(){
    string op, opAux;
    Term *term = NULL, *termAux = NULL;
    SimpleExpression *simpleEx = NULL, *simpleExAux1, *simpleExAux2;

    if(eat("+") != "") {
        op = "+";
    } else if(eat("-") != "") {
        op = "-";
    }

    term = termo();

    if(term == NULL)
    	return NULL;

    simpleEx = new SimpleExpression(op, term, NULL);
    simpleExAux1 = simpleEx;

    while(true) {

        if(eat("+") != "") {
        	opAux = "+";
            termAux = termo();
        } else if(eat("-") != "") {
            opAux = "-";
            termAux = termo();
        } else if(eat("or") != "") {
            opAux = "or";
            termAux = termo();
        } else {
            break;
        }


        simpleExAux2 = new SimpleExpression(opAux, termAux, NULL);
        simpleExAux1->simpleExpressions = simpleExAux2;
        simpleExAux1 = simpleExAux2;
    }

    return simpleEx;
}

Term *Parser::termo(){
    Factor *f = NULL;
    Term *term = NULL, *termAux1, *termAux2;
    string op = "";

    f = fator();

    if(f == NULL)
        return NULL;

    term = new Term(f, "", NULL);
    termAux1 = term;

    while(true) {
        if(eat("*") != "") {
            f = fator();
            op = "*";
        } else if(eat("div") != "") {
            f = fator();
            op = "div";
        } else if(eat("and") != "") {
            f = fator();
            op = "and";
        } else {
            break;
        }

        //Cria uma lista ligada
        termAux2 = new Term(f, "", NULL);
        termAux1->t = termAux2;
        termAux1->op = op;
        termAux1 = termAux2;

        f = NULL;
    }

    return term;
}

Factor *Parser::fator(){
	string aux;
    Variable *v;
    Declaration *d1 = NULL; Number *n1 = NULL; Expression *ex1 = NULL; Factor *f1 = NULL;

    aux = eatType("IDENTIFICADOR");
    if(aux != ""){

        v = new Variable(aux);
        vector<Expression*> *exs = fatorAux();

        d1 = new Declaration(v, exs);

        return new Factor(d1, n1, ex1, f1);
    }

    aux = eatType("NUMERO");
    if(aux != ""){

        n1 = new Number(aux);

        return new Factor(d1, n1, ex1, f1);
    }

    if(eat("(") != "") {

        ex1 = expressoes();
        if(ex1 == NULL)
            return NULL;

        if(eat(")") != "") {
        	return new Factor(d1, n1, ex1, f1);
        }
    }

    if(eat("not") != "") {

        f1 = fator();

        if(f1 != NULL)
        	return new Factor(d1, n1, ex1, f1);
    }

    return NULL;
}

vector<Expression*> *Parser::fatorAux() {
    vector<Expression*> *exs = NULL;

	if(eat("[") != "") {

		exs = listaDeExpressoes();

		if(exs != NULL && eat("]") != "") {
			return exs;
		}
	} else {
		if(eat("(") != "") {

			exs = listaDeExpressoes();

			if(eat(")") != "") {
				return exs;
			}
		}
	}


    return NULL;
}

Declaration *Parser::variavel(){
    Variable *v = NULL;
    vector<Expression*> *exs = NULL;
    string aux;
    int token = this->currentToken;

    aux = eatType("IDENTIFICADOR");
    if(aux != ""){
        v = new Variable(aux);

        token = this->currentToken;
        if(eat("[") != ""){
        	exs = listaDeExpressoes();

			if(exs != NULL && eat("]") != ""){
				return new Declaration(v, exs);
			}

        }

        this->currentToken = token;
        return new Declaration(v, NULL);
    }

    this->currentToken = token;
    return NULL;
}

Declaration *Parser::chamadaDeFuncao(){
    Variable *v = NULL;
    vector<Expression*> *exs = NULL;
    int token = this->currentToken;

    string aux = eatType("IDENTIFICADOR");
    if(aux != ""){

        v = new Variable(aux);

        token = this->currentToken;

        if(eat("(") != "") {

            exs = listaDeExpressoes();

            if(eat(")") != "") {
            	return new Declaration(v, exs);
            }
        }

        this->currentToken = token;
        return new Declaration(v, NULL);
    }

    this->currentToken = token;
    return NULL;
}

Read *Parser::readParser() {

	vector<Variable*> *variables = NULL;
	int token = this->currentToken;

	if(eat("read") != "" || eat("readln") != ""){

		if(eat("(") != ""){

			variables = listaDeIdentificadores();

			if(variables != NULL && eat(")") != ""){
				return new Read(variables);
			}

		}

	}

	this->currentToken = token;
	return NULL;
}

Write *Parser::writeParser() {
	vector<Expression*> *expressions = NULL;
	int token = this->currentToken;

	if(eat("write") != "" || eat("writeln") != ""){

		if(eat("(") != ""){

			expressions = listaDeExpressoes();

			if(expressions != NULL && eat(")") != ""){
				return new Write(expressions);
			}

		}

	}

	this->currentToken = token;
	return NULL;
}






