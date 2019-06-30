#include "Parser.h"

void Parser::eat(string symbol) {
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t != NULL){
		//cout << t->var_value << endl;
		if(t->var_value.compare(symbol) == 0) {
			this->currentToken = this->currentToken + 1;
		} else {
			cout << "erro sintático: "<< t->var_value << " na linha: " << t->row << " e coluna: "<< t->col << endl;
			this->syntaxError = true;
		}
    }
}


void Parser::parsing(string inputFileName, string outPutHashFile, string tokensFileName, string astFileName, string symbolTableFileName){

	bool e = this->process_lexicon(inputFileName, outPutHashFile, tokensFileName);

	if(e){
		Lexicon::hashIdentifiers = new HashTable(211);

		programa();

		eat(" ");

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
    Token *t = Lexicon::getTokenList(this->currentToken);
    Variable *programName = NULL;
    vector<Variable*> *listVariables = NULL; 
    Block *block = NULL;

    if(t->var_value.compare("program") == 0){
        eat(t->var_value);
        
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);

            programName = new Variable(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);

            if(t->var_value == "(") {
                eat(t->var_value);
                
                listVariables = listaDeIdentificadores();
                
                if(listVariables != NULL) {
					t = Lexicon::getTokenList(this->currentToken);

					if(t->var_value == ")") {
						eat(t->var_value);
						t = Lexicon::getTokenList(this->currentToken);

						if(t->var_value == ";") {
							eat(t->var_value);

							block = bloco();

							if(block != NULL) {
								t = Lexicon::getTokenList(this->currentToken);
								if(t->var_value == ".") {
									eat(t->var_value);
								}
							}
						}
					}
               }
            }
        }
    }

    this->ast = new Program(programName, listVariables, block);
}

Block *Parser::bloco(){
    vector<Number*> *labels = new vector<Number*>();
    vector<BlockType*> *types = new vector<BlockType*>();
	vector<BlockVar*> *variables = new vector<BlockVar*>();
	vector<DeclarationFunction*> *rotinas = new vector<DeclarationFunction*>();
	vector<Statement*> *statements = new vector<Statement*>();
	Token *t = NULL;

    while(true) {
    	t = Lexicon::getTokenList(this->currentToken);
    	if(t->var_value.compare("label") == 0){
    		vector<Number*> *l = parteDeDeclaracoesDeRotulos();
    		if(l != NULL){
				labels->insert(
					labels->end(),
					std::make_move_iterator(l->begin()),
					std::make_move_iterator(l->end())
				);
    		} else {
    			return NULL;
    		}
    	} else
    	if(t->var_value.compare("type") == 0){
    		vector<BlockType*> *t = parteDeDefinicoesDeTipos();
    		if(t != NULL){
				types->insert(
					types->end(),
					std::make_move_iterator(t->begin()),
					std::make_move_iterator(t->end())
				);
    		} else {
    			return NULL;
    		}
    	} else
    	if(t->var_value.compare("var") == 0){
    		vector<BlockVar*> *v = parteDeDeclaracoesDeVariaveis();
    		if(v != NULL){
				variables->insert(
					variables->end(),
					std::make_move_iterator(v->begin()),
					std::make_move_iterator(v->end())
				);
    		} else {
    			return NULL;
    		}
    	} else
    	if(t->var_value.compare("procedure") == 0 || t->var_value.compare("function") == 0){
    		vector<DeclarationFunction*> *r = parteDeDeclaracoesDeSubRotinas();
    		if(r != NULL){
				rotinas->insert(
					rotinas->end(),
					std::make_move_iterator(r->begin()),
					std::make_move_iterator(r->end())
				);
    		} else {
    			return NULL;
    		}
    	} else
    	if(t->var_value.compare("begin") == 0) {
    		vector<Statement*> *s = comandoComposto();
    		if(s != NULL){
				statements->insert(
					statements->end(),
					std::make_move_iterator(s->begin()),
					std::make_move_iterator(s->end())
				);
    		} else {
    			return NULL;
    		}
    	}
    	else {
    		break;
    	}
    }

    if(labels == NULL && types == NULL && variables == NULL && rotinas == NULL && statements == NULL)
    	return NULL;

    return new Block(labels, types, variables, rotinas, statements);
}

vector<Number*> *Parser::parteDeDeclaracoesDeRotulos() {
    Token *t = Lexicon::getTokenList(this->currentToken);
 
    vector<Number*> *labels = new vector<Number*>();

    if(t->var_value.compare("label") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("NUMERO") == 0){
            eat(t->var_value);
            
            labels->push_back(new Number(t->var_value));

            //ParteDeDeclaracoesDeRotulosAux
            int aux;
            while(true){
                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_value.compare(",") == 0) {
                	aux = this->currentToken;
                    eat(t->var_value);

                    t = Lexicon::getTokenList(this->currentToken);
                    if(t->var_type.compare("NUMERO") == 0){
                        eat(t->var_value);

                        labels->push_back(new Number(t->var_value));

                    }
                    else {
                    	this->currentToken = aux;
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

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }
        else {
            delete labels;
            return NULL;
        }
    }
    else {
        delete labels;
        return NULL;
    }

    return labels;
}

vector<BlockType*> *Parser::parteDeDefinicoesDeTipos() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<BlockType*> *types = new vector<BlockType*>();
    BlockType *type;

    if(t->var_value.compare("type") == 0) {
        eat(t->var_value);

        type = definicaoDeTipo();
        if(type != NULL){
        	types->push_back(type);
        	t = Lexicon::getTokenList(this->currentToken);
        	if(t->var_value.compare(";") == 0)
        		eat(t->var_value);
        	else {
        		delete types;
        		return NULL;
        	}
        }else {
        	return NULL;
        }

        //ParteDeDefinicoesDeTiposAux
        while(true){

			type = definicaoDeTipo();
			if(type != NULL){
				types->push_back(type);

	            t = Lexicon::getTokenList(this->currentToken);
	        	if(t->var_value.compare(";") == 0)
	        		eat(t->var_value);
	            else {
	            	delete types;
	            	return NULL;
	            }

			}else
				break;

        }

    }
    else {
        delete types;
        return NULL;
    }
    return types;
}

BlockType *Parser::definicaoDeTipo() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    Type *ntype = NULL;
    Variable *variable = NULL;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        variable = new Variable(t->var_value);
        
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("=") == 0) {
            eat(t->var_value);

            ntype = tipo();

        }
        else {
        	delete variable;
            return NULL;
        }
    }
    else {
        return NULL;
    }
    return new BlockType(variable, ntype);
}

Type *Parser::tipo() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<Number*> *indices = NULL;
    Variable *variable = NULL;
    Type *newtype = NULL;
    int aux;

    if(t->var_type.compare("IDENTIFICADOR") == 0){
    	if(t->var_value.compare("integer") == 0 || t->var_value.compare("real") == 0 ||
		   t->var_value.compare("boolean") == 0 || t->var_value.compare("byte") == 0 ||
		   t->var_value.compare("char") == 0 || t->var_value.compare("string") == 0 ||
		   t->var_value.compare("text") == 0)
    	{

			eat(t->var_value);
			variable = new Variable(t->var_value);
    	}
    }
    else if(t->var_value.compare("array") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("[") == 0) {
            eat(t->var_value);

            indices = indice();


            while(true){
                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_value.compare(",") == 0) {
                	aux = this->currentToken;
                    eat(t->var_value);

                    vector<Number*> *indicesAux = indice();
                    if(indicesAux != NULL) {
						indices->insert(
							indices->end(),
							std::make_move_iterator(indicesAux->begin()),
							std::make_move_iterator(indicesAux->end())
						);
                    } else {
                    	this->currentToken = aux;
                    	break;
                    }
                } else {
                    break;
                }
            }

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare("]") == 0) {
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_value.compare("of") == 0) {
                    eat(t->var_value);

                    newtype = tipo();

                }
                else {
                    return NULL;
                }

            }
            else {
                return NULL;
            }

        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
    return new Type(variable, indices, newtype);
}


vector<Number*> *Parser::indice() {

    Token *t;
    vector<Number*> *indices = new vector<Number*>();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("NUMERO") == 0){
        eat(t->var_value);
        
        indices->push_back(new Number(t->var_value));

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("..") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_type.compare("NUMERO") == 0){
                eat(t->var_value);

                indices->push_back(new Number(t->var_value));

            }
            else {
                delete indices;
                return NULL;
            }
            
        }  
        else {
            delete indices;
            return NULL;
        }
    } else {
        delete indices;
        return NULL;
    }
    return indices;
}


vector<BlockVar*> *Parser::parteDeDeclaracoesDeVariaveis() {
    Token *t;

    vector<BlockVar*> *listV = new vector<BlockVar*>();
    BlockVar *bt;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("var") == 0) {
        eat(t->var_value);

        bt = declaracoesDeVariaveis();

        if(bt != NULL){
        	listV->push_back(bt);
        	t = Lexicon::getTokenList(this->currentToken);
			if(t->var_value.compare(";") == 0){
				eat(t->var_value);
			}else{
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

				t = Lexicon::getTokenList(this->currentToken);
				if(t->var_value.compare(";") == 0){
					eat(t->var_value);
	            } else {
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
    Token *t;

    vector<Variable*> *listaID = listaDeIdentificadores();
    Type *p;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(":") == 0) {
        eat(t->var_value);

        p = tipo();
        if(p == NULL){
        	delete listaID;
        	return NULL;
        }
    }
    else {
    	delete listaID;
        return NULL;
    }
    return new BlockVar(listaID, p);
}


vector<Variable*> *Parser::listaDeIdentificadores(){
    Token *t;
    vector<Variable*> *listVariables = new vector<Variable*>();
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        listVariables->push_back(new Variable(t->var_value));
        int aux;
        
        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(",") == 0) {
            	aux = this->currentToken;
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_type.compare("IDENTIFICADOR") == 0){
                    eat(t->var_value);

                    listVariables->push_back(new Variable(t->var_value));

                }
                else {
                	this->currentToken = aux;
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
    Token *t;

    vector<DeclarationFunction*> *rotinas = new vector<DeclarationFunction*>();

    while(true) {
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("procedure") == 0){

            DeclarationFunction *p = declaracaoDeProcedimento();

            if(p != NULL) {
            	rotinas->push_back(p);

				t = Lexicon::getTokenList(this->currentToken);
				if(t->var_value.compare(";") == 0) {
					eat(t->var_value);
				} else {
					delete rotinas;
					return NULL;
				}
            }else{
                delete rotinas;
            	return NULL;
            }
        } 
        else if(t->var_value.compare("function") == 0){

            DeclarationFunction *f = declaracaoDeFuncao();

            if(f != NULL) {
            	rotinas->push_back(f);

				t = Lexicon::getTokenList(this->currentToken);
				if(t->var_value.compare(";") == 0) {
					eat(t->var_value);
				}
            }
            else{
                delete rotinas;
            	return NULL;
            }
        }
        else {
            break;
        }
    }

    if(rotinas->size() == 0) {
        delete rotinas;
        return NULL;
    }

    return rotinas;
}


DeclarationFunction *Parser::declaracaoDeProcedimento(){
    Token *t;

    Variable *functionName; vector<FormalParms*> *listParams; Block *block;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);

            functionName = new Variable(t->var_value);

            listParams = parametrosFormais();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
                
                block = bloco();

                if(block == NULL) {
                	return NULL;
                }
            }
            else {
                return NULL;
            }

        } 
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
    return new DeclarationFunction(functionName, listParams, NULL, block);
}


DeclarationFunction *Parser::declaracaoDeFuncao(){
    Token *t;

    Variable *functionName; vector<FormalParms*> *listParams; Variable *functionType; Block *block;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);

            functionName = new Variable(t->var_value);

            listParams = parametrosFormais();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(":") == 0) {
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_type.compare("IDENTIFICADOR") == 0){
                	if(t->var_value.compare("integer") == 0 || t->var_value.compare("real") == 0 ||
					   t->var_value.compare("boolean") == 0 || t->var_value.compare("byte") == 0 ||
					   t->var_value.compare("char") == 0 || t->var_value.compare("string") == 0 ||
					   t->var_value.compare("text") == 0)
                	{
						eat(t->var_value);

						functionType = new Variable(t->var_value);

						t = Lexicon::getTokenList(this->currentToken);
						if(t->var_value.compare(";") == 0) {
							eat(t->var_value);

							block = bloco();

							if(block == NULL) {
								return NULL;
							}

						}
						else {
							return NULL;
						}
                }
                else {
                    return NULL;
                }
              }
            }
            else {
                return NULL;
            }
    
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
    return new DeclarationFunction(functionName, listParams, functionType, block);
}


vector<FormalParms*> *Parser::parametrosFormais(){

    Token *t = Lexicon::getTokenList(this->currentToken);

    FormalParms *p;
    vector<FormalParms*> *params = new vector<FormalParms*>();

    if(t->var_value.compare("(") == 0) {
        eat(t->var_value);

        p = secoesDeParametrosFormais();

        if(p != NULL){
        	params->push_back(p);

        	int aux;
			while(true){
				t = Lexicon::getTokenList(this->currentToken);
				if(t->var_value.compare(",") == 0) {
					aux = this->currentToken;
					eat(t->var_value);

					p = secoesDeParametrosFormais();
					if(p != NULL){
						params->push_back(p);
					}
	                else {
	                	this->currentToken = aux;
	                	break;
	                }

				} else {
					break;
				}
			}
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(")") == 0) {
            eat(t->var_value);
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
    return params;
}


FormalParms *Parser::secoesDeParametrosFormais(){

    Token *t = Lexicon::getTokenList(this->currentToken);

    string op = "", type = "";
    vector<Variable*> *params = NULL;

    if(t->var_value.compare("var") == 0 || t->var_type.compare("IDENTIFICADOR") == 0) {
        if(t->var_value.compare("var") == 0){
            eat(t->var_value);
            op = "var";
        }

        params = listaDeIdentificadores();

        if(params == NULL)
        	return NULL;

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(":") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_type.compare("IDENTIFICADOR") == 0){
                eat(t->var_value);

                type = t->var_value;

            }
        }
        else {
            delete params;
            return NULL;
        }
    }
    else if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

            op = "function";
            params = listaDeIdentificadores();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(":") == 0) {
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_type.compare("IDENTIFICADOR") == 0){
                    eat(t->var_value);

                    type = t->var_value;

                }
            }
            else {
                delete params;
                return NULL;
            }

    }
    else if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

        op = "procedure";
        params = listaDeIdentificadores();

    }
    else
    	return NULL;
    return new FormalParms(op, params, type);
}


vector<Statement*> *Parser::comandoComposto(){

    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<Statement*> *st = NULL;
    Statement*s = NULL;

    if(t->var_value.compare("begin") == 0) {
        eat(t->var_value);

        st = new vector<Statement*>();

        s = comando();
        if(s != NULL){
        	if(s->st == NULL)
        		return NULL;

        	st->push_back(s);
        	t = Lexicon::getTokenList(this->currentToken);
        	if(t->var_value.compare(";") == 0) {
        		eat(t->var_value);
        	} else {
        		delete st;
        		return NULL;
        	}

            while(true){
                t = Lexicon::getTokenList(this->currentToken);
				s = comando();
				if(s != NULL){
					st->push_back(s);
					t = Lexicon::getTokenList(this->currentToken);
		        	if(t->var_value.compare(";") == 0) {
		        		eat(t->var_value);
		        	} else {
		        		delete st;
		        		return NULL;
		        	}
				} else {
					break;
                }
            }
        }
        else {
            delete st;
        	return NULL;
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("end") == 0) {
            eat(t->var_value);
        }
        else {
            return NULL;
        }
    }
    return st;
}


Statement *Parser::comando(){

    Token *t;
    
    Number *n = NULL; StatementWithoutLabel *st = NULL;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("NUMERO") == 0){
        eat(t->var_value);

        n = new Number(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(":") == 0) {
            eat(t->var_value);

            st = comandoSemRotulo();
            if(st == NULL)
                return NULL;
        }

    } else {
        st = comandoSemRotulo();
        if(st == NULL)
            return NULL;
    }

    return new Statement(n , st);
}


StatementWithoutLabel *Parser::comandoSemRotulo(){

    Token *t;
    Assign *assign = NULL;
    Declaration *declaration = NULL;
    Number *goto_ = NULL;
    vector<Statement*> *statements = NULL;
    Branch *branch = NULL;
    Loop *loop = NULL;
    Read *read = NULL;
    Write *write = NULL;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        
        t = Lexicon::getTokenList(this->currentToken + 1);
        if(t->var_value.compare(":=") == 0 || t->var_value.compare("(") != 0) {
            assign = atribuicoes();
            if(assign == NULL)
            	return NULL;
        }
        else {
            if(t->var_value.compare("(") == 0) {
                declaration = chamadaDeFuncao();
                if(declaration == NULL)
                	return NULL;
            }
            else {
                return NULL;
            }
        }
    }
    else if(t->var_value.compare("goto") == 0) {
        goto_ = desvios();
        if(goto_ == NULL)
            return NULL;
    }
    else if(t->var_value.compare("begin") == 0) {
        statements = comandoComposto();
        if(statements == NULL)
            return NULL;
    }
    else if(t->var_value.compare("if") == 0) {
        branch = comandoCondicional();
        if(branch == NULL)
            return NULL;
    }

    else if(t->var_value.compare("while") == 0){
        loop = comandoRepetitivo();
        if(loop == NULL)
            return NULL;
    }
    else if(t->var_value.compare("read") == 0){
        read = readParser();
        if(read == NULL)
            return NULL;
    }
    else if(t->var_value.compare("write") == 0){
        write = writeParser();
        if(write == NULL)
            return NULL;
    }
    else
    	return NULL;
    return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop, read, write);
}


Assign *Parser::atribuicoes(){

    Token *t;
    Expression *ex;
    Declaration *dec = variavel();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(":=") == 0) {
        eat(t->var_value);

        ex = expressoes();

    }
    else {
        return NULL;
    }
    return new Assign(dec, ex);
}

Declaration *Parser::chamadaDeProcedimentos(){

    Token *t;
    Variable *v = NULL;
    vector<Expression*> *exs = NULL;
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        v = new Variable(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "(") {
            eat(t->var_value);

            exs = listaDeExpressoes();


            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == ")") {
                eat(t->var_value);
            }
            else {
                return NULL;
            }
        }
        else {
            return NULL;
        }
    }

    return new Declaration(v, exs);
}

Number *Parser::desvios(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    Number *n= NULL;

    if(t->var_value == "goto") {
        eat(t->var_value);
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("NUMERO") == 0){
            eat(t->var_value);

            n = new Number(t->var_value);

        }
        else
        	return NULL;
    }

    return n;
}

Branch *Parser::comandoCondicional(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    Expression *ex = NULL; StatementWithoutLabel *st1 = NULL; StatementWithoutLabel *st2 = NULL;

    if(t->var_value == "if") {
        eat(t->var_value);

        ex = expressoes();

        if (ex == NULL)
        	return NULL;

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "then") {
            eat(t->var_value);

            st1 = comandoSemRotulo();

            if (st1 == NULL)
            	return NULL;

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == "else") {
                eat(t->var_value);

                st2 = comandoSemRotulo();

            }
        }
        else {
            return NULL;
        }
    }

    return new Branch(ex, st1, st2);
}

Loop *Parser::comandoRepetitivo(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    Expression *ex = NULL; StatementWithoutLabel *st1 = NULL;

    if(t->var_value == "while") {
        eat(t->var_value);

        ex = expressoes();

        if (ex == NULL)
        	return NULL;

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "do") {
            eat(t->var_value);

            st1 = comandoSemRotulo();
            if (st1 == NULL)
            	return NULL;
        }
        else {
            return NULL;
        }
    }

    return new Loop(ex, st1);
}

vector<Expression*> *Parser::listaDeExpressoes(){
    vector<Expression*> *listEx = new vector<Expression*>();
    Expression *ex = NULL;

    ex = expressoes();

    if(ex != NULL)
        listEx->push_back(ex);

    ex = NULL;

    Token *t;
    int aux;
    while(true){
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(",") == 0) {
        	aux = this->currentToken;
            eat(t->var_value);

            ex = expressoes();

            if(ex != NULL){
                listEx->push_back(ex);
            }
            else {
            	this->currentToken = aux;
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
    if(s != "" && simpleEx2 == NULL)
    	return NULL;

    return new Expression(simpleEx1, s, simpleEx2);
}

string Parser::relacao(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("=") == 0) {
        eat(t->var_value);
        return "=";
    } else if(t->var_value.compare("<>") == 0) {
        eat(t->var_value);
        return "<>";
    } else if(t->var_value.compare("<") == 0) {
        eat(t->var_value);
        return "<";
    } else if(t->var_value.compare("<=") == 0) {
        eat(t->var_value);
        return "<=";
    } else if(t->var_value.compare(">") == 0) {
        eat(t->var_value);
        return ">";
    } else if(t->var_value.compare(">=") == 0) {
        eat(t->var_value);
        return ">=";
    } else {
        return "";
    }
}

SimpleExpression *Parser::expressaoSimples(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    string op, opAux;
    Term *term = NULL, *termAux = NULL;
    SimpleExpression *simpleEx = NULL, *simpleExAux1, *simpleExAux2;

    if(t->var_value == "+") {
        eat(t->var_value);
        op = t->var_value;
    } else if(t->var_value == "-") {
        eat(t->var_value);
        op = t->var_value;
    }

    term = termo();

    if(term == NULL)
    	return NULL;

    simpleEx = new SimpleExpression(op, term, NULL);
    simpleExAux1 = simpleEx;

    while(true) {

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "+") {
            eat(t->var_value);
            opAux = t->var_value;
            termAux = termo();
        } else if(t->var_value == "-") {
            eat(t->var_value);
            opAux = t->var_value;
            termAux = termo();
        } else if(t->var_value == "or") {
            eat(t->var_value);
            opAux = t->var_value;
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

    Token *t = Lexicon::getTokenList(this->currentToken);
    while(true) {
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "*") {
            eat(t->var_value);
            f = fator();
            op = "*";
        } else if(t->var_value == "div") {
            eat(t->var_value);
            f = fator();
            op = "div";
        } else if(t->var_value == "and") {
            eat(t->var_value);
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
    Token *t, *t2;
    Variable *v;
    Declaration *d1 = NULL; Number *n1 = NULL; Expression *ex1 = NULL; Factor *f1 = NULL;

    t2 = Lexicon::getTokenList(this->currentToken-1);
    t = Lexicon::getTokenList(this->currentToken);
    if((t2->var_type.compare("IDENTIFICADOR") == 0) && (t->var_value.compare("[") == 0 || t->var_value.compare("(") == 0)) {
		vector<Expression*> *exs = fatorAux();

		d1 = new Declaration(NULL, exs);
    }
    else if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
        
        v = new Variable(t->var_value);
        vector<Expression*> *exs = fatorAux();

        d1 = new Declaration(v, exs);

    }
    else if(t->var_type.compare("NUMERO") == 0){
        eat(t->var_value);
        
        n1 = new Number(t->var_value);

    }
    else if(t->var_value.compare("(") == 0) {
        eat(t->var_value);
        
        ex1 = expressoes();
        if(ex1 == NULL)
            return NULL;

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(")") == 0) {
            eat(t->var_value);
        } else {
            return NULL;
        }
    }
    else if(t->var_value == "not") {
        eat(t->var_value);

        f1 = fator();

        if(f1 == NULL)
            return NULL;
    }
    else {
    	return NULL;
    }
    return new Factor(d1, n1, ex1, f1);
}

vector<Expression*> *Parser::fatorAux() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    vector<Expression*> *exs = NULL;

	if(t->var_value.compare("[") == 0) {
		eat(t->var_value);

		exs = listaDeExpressoes();

		t = Lexicon::getTokenList(this->currentToken);
		if(t->var_value.compare("]") == 0) {
			eat(t->var_value);
		} else {
			return NULL;
		}
	} else {
		if(t->var_value.compare("(") == 0) {
				eat(t->var_value);

				exs = listaDeExpressoes();

				t = Lexicon::getTokenList(this->currentToken);
				if(t->var_value.compare(")") == 0) {
					eat(t->var_value);
				} else {
					return NULL;
				}
			}
	}

    if(exs == NULL)
        return NULL;
    return exs;
}

Declaration *Parser::variavel(){
    Token *t;
    Variable *v = NULL;
    vector<Expression*> *exs = NULL;
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        v = new Variable(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("[") == 0){
        	exs = listaDeExpressoes();
        }

    }
    else {
        return NULL;
    }
    return new Declaration(v, exs);
}

Declaration *Parser::chamadaDeFuncao(){
    Token *t;
    Variable *v = NULL;
    vector<Expression*> *exs = NULL;
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
        
        v = new Variable(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "(") {
            eat(t->var_value);

            exs = listaDeExpressoes();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == ")") {
                eat(t->var_value);
            }
            else {
                return NULL;
            }
        }
        else {
            return NULL;
        }

    }

    return new Declaration(v, exs);
}

Read *Parser::readParser() {

	Token *t = Lexicon::getTokenList(this->currentToken);
	vector<Variable*> *variables = NULL;

	if(t->var_type.compare("read") == 0 || t->var_type.compare("readln") == 0){
		eat(t->var_value);

		t = Lexicon::getTokenList(this->currentToken);
		if(t->var_type.compare("(") == 0){
			eat(t->var_value);

			variables = listaDeIdentificadores();

			t = Lexicon::getTokenList(this->currentToken);
			if(t->var_type.compare(")") == 0){
				eat(t->var_value);

			}
			else
				return NULL;
		}
		else
			return NULL;
	}

	if(variables == NULL)
		return NULL;

	return new Read(variables);
}

Write *Parser::writeParser() {
	Token *t = Lexicon::getTokenList(this->currentToken);
	vector<Expression*> *expressions = NULL;

	if(t->var_type.compare("write") == 0 || t->var_type.compare("writeln") == 0){
		eat(t->var_value);

		t = Lexicon::getTokenList(this->currentToken);
		if(t->var_type.compare("(") == 0){
			eat(t->var_value);

			expressions = listaDeExpressoes();

			t = Lexicon::getTokenList(this->currentToken);
			if(t->var_type.compare(")") == 0){
				eat(t->var_value);

			}
			else
				return NULL;
		}
		else
			return NULL;
	}

	if(expressions == NULL)
		return NULL;

	return new Write(expressions);
}



