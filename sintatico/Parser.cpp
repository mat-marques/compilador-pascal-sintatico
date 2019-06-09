#include "Parser.h"

void Parser::eat(string symbol) {
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(symbol) == 0) {
        this->currentToken++;
    } else {
            cout << "erro sintático: " << t->var_value << " linha: " << t->row << " coluna:"<< t->col << endl;
    }
    this->currentToken++;
}


void Parser::programa() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("program") == 0){
        eat(t->var_value);
        identificador();
        t = Lexicon::getTokenList(this->currentToken);

        if(t->var_value == "(") {
            eat(t->var_value);
            listaDeIdentificadores();
            t = Lexicon::getTokenList(this->currentToken);

            if(t->var_value == ")") {
                eat(t->var_value);
                t = Lexicon::getTokenList(this->currentToken);

                if(t->var_value == ";") {
                    eat(t->var_value);
                    bloco();
                    t = Lexicon::getTokenList(this->currentToken);

                    if(t->var_value == ".") {
                        eat(t->var_value);
                    }
                }
            }
        }
    }
}

void Parser::bloco(){
    parteDeDeclaracoesDeRotulos();
    parteDeDefinicoesDeTipos();
    parteDeDeclaracoesDeVariaveis();
    parteDeDeclaracoesDeSubRotinas();
    comandoComposto();
}

void Parser::parteDeDeclaracoesDeRotulos() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("label") == 0) {
        eat(t->var_value);

        numero();

        if(t->var_value.compare("{") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(",") == 0) {
                eat(t->var_value);

                numero();

                if(t->var_value.compare("}") == 0) {
                    eat(t->var_value);
                    t = Lexicon::getTokenList(this->currentToken);

                    if(t->var_value.compare(";") == 0) {
                        eat(t->var_value);
                    }

                }
            }
        }
    }   
}

void Parser::parteDeDefinicoesDeTipos() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("type") == 0) {
        eat(t->var_value);

        definicaoDeTipo();

        if(t->var_value.compare("{") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                definicaoDeTipo();

                if(t->var_value.compare("}") == 0) {
                    eat(t->var_value);
                    t = Lexicon::getTokenList(this->currentToken);

                    if(t->var_value.compare(";") == 0) {
                        eat(t->var_value);
                    }

                }
            }
        }
    }  
}

void Parser::definicaoDeTipo() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    identificador();

    if(t->var_value.compare("=") == 0) {
        eat(t->var_value);

        tipo();
    }
}

void Parser::tipo() {
    identificador();
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("array") == 0) {
        eat(t->var_value);

        indice();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("{") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(",") == 0) {
                eat(t->var_value);

                indice();

                if(t->var_value.compare("}") == 0) {
                    eat(t->var_value);
                }
            }
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("{") == 0) {
            eat(t->var_value);

            tipo();

        }
    }
}

void Parser::indice() {
    Token *t;

    numero();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("..") == 0) {
        eat(t->var_value);

        numero();
        
    }
}

void Parser::parteDeDeclaracoesDeVariaveis() {
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("var") == 0) {
        eat(t->var_value);

        declaracoesDeVariaveis();
        
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("{") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                declaracoesDeVariaveis();

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_value.compare("}") == 0) {
                    eat(t->var_value);

                    t = Lexicon::getTokenList(this->currentToken);
                    if(t->var_value.compare(";") == 0) {
                        eat(t->var_value);
                    }
                }
            }
        }
    }
}

void Parser::declaracoesDeVariaveis() {
   Token *t;

    listaDeIdentificadores();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(":") == 0) {
        eat(t->var_value);

        tipo();

    }
}

void Parser::listaDeIdentificadores(){
    Token *t;

    identificador();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("{") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(",") == 0) {
            eat(t->var_value);

            identificador();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare("}") == 0) {
                eat(t->var_value);
            }
        }
    }
}

void Parser::parteDeDeclaracoesDeSubRotinas(){
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("{") == 0) {
        eat(t->var_value);

        declaracaoDeProcedimento();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }

        declaracaoDeFuncao();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }

    }
}

void Parser::declaracaoDeProcedimento(){
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

       identificador();
       parametrosFormais();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }

        bloco();

    }
}

void Parser::declaracaoDeFuncao(){
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

       identificador();
       parametrosFormais();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }
        
        identificador();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }

        bloco();

    }
}

void Parser::parametrosFormais(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("(") == 0) {
        eat(t->var_value);

        secoesDeParametrosFormais();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("{") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }

            secoesDeParametrosFormais();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare("}") == 0) {
                eat(t->var_value);
            }

        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(")") == 0) {
            eat(t->var_value);
        }
    }
}

void Parser::secoesDeParametrosFormais(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("var") == 0) {
        eat(t->var_value);
    }

    listaDeIdentificadores();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(";") == 0) {
        eat(t->var_value);
    }

    identificador();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

            listaDeIdentificadores();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }

            identificador();
    }

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

        listaDeIdentificadores();

    }
}

void Parser::comandoComposto(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("begin") == 0) {
        eat(t->var_value);

        comando();

        if(t->var_value.compare("{") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                comando();

                if(t->var_value.compare("}") == 0) {
                    eat(t->var_value);
                    t = Lexicon::getTokenList(this->currentToken);

                    if(t->var_value.compare("end") == 0) {
                        eat(t->var_value);
                    }

                }
            }
        }
    }   
}

void Parser::comando(){
    Token *t;
    
    numero();

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(";") == 0) {
        eat(t->var_value);
    }

    comandoSemRotulo();
}

void Parser::comandoSemRotulo(){
    atribuicoes();
    chamadaDeProcedimentos();
    desvios();
    comandoComposto();
    comandoCondicional();
    comandoRepetitivo();
}

void Parser::atribuicoes(){
    Token *t;
    variavel();
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(":=") == 0) {
        eat(t->var_value);
        expressoes();
    }
}

void Parser::chamadaDeProcedimentos(){
    identificador();
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "(") {
        eat(t->var_value);

        listaDeExpressoes();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == ")") {
            eat(t->var_value);
        }
    }
}

void Parser::desvios(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "goto") {
        eat(t->var_value);
        numero();
    }
}

void Parser::comandoCondicional(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "if") {
        eat(t->var_value);

        expressoes();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "then") {
            eat(t->var_value);

            comandoSemRotulo();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == "else") {
                eat(t->var_value);
                comandoSemRotulo();
            }
        }
    }
}


void Parser::comandoRepetitivo(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "while") {
        eat(t->var_value);

        expressoes();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "do") {
            eat(t->var_value);
            comandoSemRotulo();
        }
    }

}

void Parser::listaDeExpressoes(){
    
    expressoes();

    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "{") {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == ",") {
            eat(t->var_value);

            expressoes();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == "}") {
                eat(t->var_value);
            }

        }
    }

}

void Parser::expressoes(){
    expressaoSimples();
    relacao();
    expressaoSimples();
}

void Parser::relacao(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "=") {
        eat(t->var_value);
    } else if(t->var_value == "<>") {
        eat(t->var_value);
    } else if(t->var_value == "<") {
        eat(t->var_value);
    } else if(t->var_value == "<=") {
        eat(t->var_value);
    } else if(t->var_value == ">") {
        eat(t->var_value);
    } else if(t->var_value == ">=") {
        eat(t->var_value);
    }
}

void Parser::expressaoSimples(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "+") {
        eat(t->var_value);
    } else if(t->var_value == "-") {
        eat(t->var_value);
    }

    termo();

    if(t->var_value == "{") {
        eat(t->var_value);
        t = Lexicon::getTokenList(this->currentToken);

        if(t->var_value == "(") {
            eat(t->var_value);
            t = Lexicon::getTokenList(this->currentToken);

            if(t->var_value == "+") {
                eat(t->var_value);
            } else if(t->var_value == "-") {
                eat(t->var_value);
            } else if(t->var_value == "or") {
                eat(t->var_value);
            }

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == ")") {
                eat(t->var_value);
            }
        }

        termo();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "}") {
            eat(t->var_value);
        }
    }
}

void Parser::termo(){
    fator();
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value == "{") {
        eat(t->var_value);
        t = Lexicon::getTokenList(this->currentToken);

        if(t->var_value == "(") {
            eat(t->var_value);
            t = Lexicon::getTokenList(this->currentToken);

            if(t->var_value == "*") {
                eat(t->var_value);
            } else if(t->var_value == "div") {
                eat(t->var_value);
            } else if(t->var_value == "and") {
                eat(t->var_value);
            }

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == ")") {
                eat(t->var_value);
            }
        }
        
        fator();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "}") {
            eat(t->var_value);
        }
    }
}

void Parser::fator(){
    Token *t;
    variavel();
    chamadaDeFuncao();
    expressoes();
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "not") {
        fator();
    }
}

void Parser::variavel(){
    identificador();
    listaDeExpressoes();
}

void Parser::chamadaDeFuncao(){
    identificador();
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "(") {
        eat(t->var_value);

        listaDeExpressoes();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == ")") {
            eat(t->var_value);
        }
    }
}

void Parser::numero(){
    digito();
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value == "{") {
        eat(t->var_value);

        digito();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "}") {
            eat(t->var_value);
        }
        
    }
}

void Parser::digito(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "0") {
        eat(t->var_value);
    } else if(t->var_value == "1") {
        eat(t->var_value);
    } else if(t->var_value == "2") {
        eat(t->var_value);
    } else if(t->var_value == "3") {
        eat(t->var_value);
    } else if(t->var_value == "4") {
        eat(t->var_value);
    } else if(t->var_value == "5") {
        eat(t->var_value);
    } else if(t->var_value == "6") {
        eat(t->var_value);
    } else if(t->var_value == "7") {
        eat(t->var_value);
    } else if(t->var_value == "8") {
        eat(t->var_value);
    } else if(t->var_value == "9") {
        eat(t->var_value);
    }
}

void Parser::identificador(){
    letra();
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value == "{") {
        eat(t->var_value);

        letra();
        digito();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "}") {
            eat(t->var_value);
        }

    }
}

void Parser::letra(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "a") {
        eat(t->var_value);
    } else if(t->var_value == "b") {
        eat(t->var_value);
    } else if(t->var_value == "c") {
        eat(t->var_value);
    } else if(t->var_value == "d") {
        eat(t->var_value);
    } else if(t->var_value == "e") {
        eat(t->var_value);
    } else if(t->var_value == "f") {
        eat(t->var_value);
    } else if(t->var_value == "g") {
        eat(t->var_value);
    } else if(t->var_value == "h") {
        eat(t->var_value);
    } else if(t->var_value == "i") {
        eat(t->var_value);
    } else if(t->var_value == "j") {
        eat(t->var_value);
    } else if(t->var_value == "k") {
        eat(t->var_value);
    } else if(t->var_value == "l") {
        eat(t->var_value);
    } else if(t->var_value == "m") {
        eat(t->var_value);
    } else if(t->var_value == "n") {
        eat(t->var_value);
    } else if(t->var_value == "o") {
        eat(t->var_value);
    } else if(t->var_value == "p") {
        eat(t->var_value);
    } else if(t->var_value == "q") {
        eat(t->var_value);
    } else if(t->var_value == "r") {
        eat(t->var_value);
    } else if(t->var_value == "s") {
        eat(t->var_value);
    } else if(t->var_value == "t") {
        eat(t->var_value);
    } else if(t->var_value == "u") {
        eat(t->var_value);
    } else if(t->var_value == "v") {
        eat(t->var_value);
    } else if(t->var_value == "w") {
        eat(t->var_value);
    } else if(t->var_value == "x") {
        eat(t->var_value);
    } else if(t->var_value == "y") {
        eat(t->var_value);
    } else if(t->var_value == "z") {
        eat(t->var_value);
    }
}