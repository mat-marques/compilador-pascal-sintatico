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
    Variable *programName;
    vector<Variable*> *listVariables; 
    Block *block;

    if(t->var_value.compare("program") == 0){
        eat(t->var_value);
        
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){

            programName = new Variable(t->var_value);
            
            eat(t->var_value);
            t = Lexicon::getTokenList(this->currentToken);

            if(t->var_value == "(") {
                eat(t->var_value);
                
                listVariables = listaDeIdentificadores();
                
                t = Lexicon::getTokenList(this->currentToken);

                if(t->var_value == ")") {
                    eat(t->var_value);
                    t = Lexicon::getTokenList(this->currentToken);

                    if(t->var_value == ";") {
                        eat(t->var_value);
                        
                        block = bloco();
                        
                        t = Lexicon::getTokenList(this->currentToken);

                        if(t->var_value == ".") {
                            eat(t->var_value);
                        }
                    }
                }
            }
        }
    }

    this->ast = new Program(programName, listVariables, block);
}

Block *Parser::bloco(){
    vector<Number*> *labels = parteDeDeclaracoesDeRotulos();
    parteDeDefinicoesDeTipos();
    parteDeDeclaracoesDeVariaveis();
    parteDeDeclaracoesDeSubRotinas();
    comandoComposto();
    return new Block(labels);
}

vector<Number*> *Parser::parteDeDeclaracoesDeRotulos() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    int x = 0;
    vector<Number*> *labels = new vector<Number*>();

    if(t->var_value.compare("label") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("NUMERO") == 0){
            eat(t->var_value);

            stringstream geek(t->var_value); 
  
            geek >> x;
            
            labels->push_back(new Number(x));

        }

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(",") == 0) {
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_type.compare("NUMERO") == 0){
                    eat(t->var_value);
                    
                    stringstream geek(t->var_value); 
  
                    geek >> x;

                    labels->push_back(new Number(x));

                }

            } else {
                break;
            }
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }
    }

    return labels;
}

vector<BlockType*> *Parser::parteDeDefinicoesDeTipos() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<BlockType*> *labels = new vector<BlockType*>();
    
    if(t->var_value.compare("type") == 0) {
        eat(t->var_value);

        definicaoDeTipo();

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                definicaoDeTipo();

            } else {
                break;
            }
        }

    }  
}

void Parser::definicaoDeTipo() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("=") == 0) {
            eat(t->var_value);

            tipo();
        }
    }
}

void Parser::tipo() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
    }
    else if(t->var_value.compare("array") == 0) {
        eat(t->var_value);

        indice();

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(",") == 0) {
                eat(t->var_value);

                indice();

            } else {
                break;
            }
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("of") == 0) {
            eat(t->var_value);

            tipo();

        }
    }
}

void Parser::indice() {
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("NUMERO") == 0){
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("..") == 0) {
            eat(t->var_value);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_type.compare("NUMERO") == 0){
                eat(t->var_value);
            }
            
        }
    }

}

void Parser::parteDeDeclaracoesDeVariaveis() {
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("var") == 0) {
        eat(t->var_value);

        declaracoesDeVariaveis();
        
        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                declaracoesDeVariaveis();

            } else {
                break;
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

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(",") == 0) {
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_type.compare("IDENTIFICADOR") == 0){
                    eat(t->var_value);
                }

            } else {
                break;
            }
        }
    }
}

void Parser::parteDeDeclaracoesDeSubRotinas(){
    Token *t;

    while(true) {
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("procedure") == 0){

            declaracaoDeProcedimento();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }
        } 
        else if(t->var_value.compare("function") == 0){

            declaracaoDeFuncao();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }
        }
        else {
            break;
        }
    }


}

void Parser::declaracaoDeProcedimento(){
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);

            parametrosFormais();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
                
                bloco();
            }

        }

    }
}

void Parser::declaracaoDeFuncao(){
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);
            
            parametrosFormais();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_type.compare("IDENTIFICADOR") == 0){
                    eat(t->var_value);

                    t = Lexicon::getTokenList(this->currentToken);
                    if(t->var_value.compare(";") == 0) {
                        eat(t->var_value);
                        
                        bloco();

                    }
                }
            }
    
        }

    }
}

void Parser::parametrosFormais(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("(") == 0) {
        eat(t->var_value);

        secoesDeParametrosFormais();

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                secoesDeParametrosFormais();

            } else {
                break;
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

    if(t->var_value.compare("var") == 0 || t->var_type.compare("IDENTIFICADOR") == 0) {
        if(t->var_value.compare("var") == 0){
            eat(t->var_value);
        }

        listaDeIdentificadores();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);
        }
    }
    else if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

            listaDeIdentificadores();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_type.compare("IDENTIFICADOR") == 0){
                eat(t->var_value);
            }
    }
    else if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

        listaDeIdentificadores();

    }
}

void Parser::comandoComposto(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    if(t->var_value.compare("begin") == 0) {
        eat(t->var_value);

        comando();

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                comando();

            } else {
                break;
            }
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("end") == 0) {
            eat(t->var_value);
        }
    }   
}

void Parser::comando(){
    Token *t;
    
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("NUMERO") == 0){
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);

            comandoSemRotulo();
        
        }

    } else {
        comandoSemRotulo();
    }

}

void Parser::comandoSemRotulo(){
    Token *t;
    t = Lexicon::getTokenList(this->currentToken);

    if(t->var_type.compare("IDENTIFICADOR") == 0){
        atribuicoes();
        chamadaDeProcedimentos();
    }
    else if(t->var_value.compare("goto") == 0) {
        desvios();
    }
    else if(t->var_value.compare("begin") == 0) {
        comandoComposto();
    }
    else if(t->var_value.compare("if") == 0) {
        comandoCondicional();
    }
    else {
        comandoRepetitivo();
    }
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
    Token *t;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "(") {
            eat(t->var_value);

            listaDeExpressoes();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == ")") {
                eat(t->var_value);
            }
        }
    }
}

void Parser::desvios(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "goto") {
        eat(t->var_value);
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("NUMERO") == 0){
            eat(t->var_value);
        }
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

    Token *t;
    while(true){
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(",") == 0) {
            eat(t->var_value);

            expressoes();

        } else {
            break;
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

    while(true) {
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "+") {
            eat(t->var_value);
            termo();
        } else if(t->var_value == "-") {
            eat(t->var_value);
            termo();
        } else if(t->var_value == "or") {
            eat(t->var_value);
            termo();
        } else {
            break;
        }
    }

}

void Parser::termo(){
    fator();
    Token *t = Lexicon::getTokenList(this->currentToken);

    while(true) {
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "*") {
            eat(t->var_value);
            fator();
        } else if(t->var_value == "div") {
            eat(t->var_value);
            fator();
        } else if(t->var_value == "and") {
            eat(t->var_value);
            fator();
        } else {
            break;
        }
    }
}

void Parser::fator(){
    Token *t;
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        fatorAux();

        variavel();
        //Duas produções com o mesmo símbolo inicial. Ambiguidade.
        chamadaDeFuncao();
    }
    else if(t->var_value.compare("(") == 0) {
        eat(t->var_value);
        expressoes();
        if(t->var_value.compare(")") == 0) {
            eat(t->var_value);
        }
    }
    else if(t->var_value == "not") {
        fator();
    }
}

void Parser::fatorAux() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("(") != 0){

        listaDeExpressoes();
    
    } else {
        if(t->var_value.compare("(") == 0) {
            eat(t->var_value);
    
            listaDeExpressoes();

            if(t->var_value.compare(")") == 0) {
                eat(t->var_value);
            }
        }
    }

}

void Parser::variavel(){
    Token *t;
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
        listaDeExpressoes();
    }
}

void Parser::chamadaDeFuncao(){
    Token *t;
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
        
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "(") {
            eat(t->var_value);

            listaDeExpressoes();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value == ")") {
                eat(t->var_value);
            }
        }

    }

}