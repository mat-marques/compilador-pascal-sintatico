#include "Parser.h"

void Parser::eat(string symbol) {
    Token *t = Lexicon::getTokenList(this->currentToken);
    cout << t->var_value << endl;
    if(t->var_value.compare(symbol) == 0) {
        this->currentToken = this->currentToken + 1;
    } else {
            cout << "erro sintático: " << t->var_value << " linha: " << t->row << " coluna:"<< t->col + 1<< endl;
    }
}


void Parser::parsing(){
    Lexicon::hashIdentifiers = new HashTable(211);
    
    programa();

    //Printar a AST
    printAst("../saida/ast.txt");

    //Printar a hashtable
    //Lexicon::hashIdentifiers->show("../saida/tabelaDeSimbolos.txt");
}

void Parser::printAst(string astFileName) {
    ofstream astFile;
  	astFile.open (astFileName);

  	astFile << "Árvore Abstrata Sintática" << "\n\n";

    if(this->ast->programName != NULL)
    	astFile << this->ast->programName->variable << " ";

    if(this->ast->listVariables != NULL){
        for (std::vector<Variable*>::iterator it = this->ast->listVariables->begin(); it != this->ast->listVariables->end(); ++it) {
            astFile << (*it)->variable << " ";
        }
    }

    astFile.close();
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
                        } else {
                            eat(t->var_value);
                        }
                    } else {
                        eat(";");
                    }
                } else {
                    eat(")");
                }
                
            } else {
                eat("(");
            }
        } else {
            eat("IDENTIFICADOR");
        }
    } else {
        eat("program");
    }

    this->ast = new Program(programName, listVariables, block);
}

Block *Parser::bloco(){
    cout << "Bloco" << endl;
    vector<Number*> *labels = parteDeDeclaracoesDeRotulos();
    vector<BlockType*> *types = parteDeDefinicoesDeTipos();
    vector<BlockVar*> *variables = parteDeDeclaracoesDeVariaveis();
    vector<DeclarationFunction*> *rotinas = parteDeDeclaracoesDeSubRotinas();
    vector<Statement*> *statements = comandoComposto();

    return new Block(labels, types, variables, rotinas, statements);
}

vector<Number*> *Parser::parteDeDeclaracoesDeRotulos() {
    cout << "parteDeDeclaracoesDeRotulos" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);
 
    vector<Number*> *labels = new vector<Number*>();

    if(t->var_value.compare("label") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("NUMERO") == 0){
            eat(t->var_value);
            
            labels->push_back(new Number(t->var_value));

            //ParteDeDeclaracoesDeRotulosAux
            while(true){
                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_value.compare(",") == 0) {
                    eat(t->var_value);

                    t = Lexicon::getTokenList(this->currentToken);
                    if(t->var_type.compare("NUMERO") == 0){
                        eat(t->var_value);

                        labels->push_back(new Number(t->var_value));

                    }

                } else {
                    break;
                }
            }

        } 
        else {
            return NULL;
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }

    return labels;
}

vector<BlockType*> *Parser::parteDeDefinicoesDeTipos() {
    cout << "parteDeDefinicoesDeTipos" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<BlockType*> *types = new vector<BlockType*>();
    
    if(t->var_value.compare("type") == 0) {
        eat(t->var_value);

        types->push_back(definicaoDeTipo());

        //ParteDeDefinicoesDeTiposAux
        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                types->push_back(definicaoDeTipo());

            } else {
                break;
            }
        }

    }
    else {
        return NULL;
    }
    return types;
}

BlockType *Parser::definicaoDeTipo() {
    cout << "definicaoDeTipo" << endl;
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
            return NULL;
        }
    }
    else {
        return NULL;
    }
    return new BlockType(variable, ntype);
}

Type *Parser::tipo() {
    cout << "Tipo" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<Number*> *indices = NULL;
    Variable *variable = NULL;
    Type *newtype = NULL;

    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
        variable = new Variable(t->var_value);
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
                    eat(t->var_value);

                    vector<Number*> *indicesAux = indice();
                    indices->insert(
                        indices->end(),
                        std::make_move_iterator(indicesAux->begin()),
                        std::make_move_iterator(indicesAux->end())
                    );

                } else {
                    break;
                }
            }

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare("of") == 0) {
                eat(t->var_value);

                newtype = tipo();

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_value.compare("]") == 0) {
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
    cout << "Indice" << endl;
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
                return NULL;
            }
            
        }  
        else {
            return NULL;
        }
    } else {
        return NULL;
    }
    return indices;
}

vector<BlockVar*> *Parser::parteDeDeclaracoesDeVariaveis() {
    cout << "parteDeDeclaracoesDeVariaveis" << endl;
    Token *t;

    vector<BlockVar*> *listV = new vector<BlockVar*>();
    BlockVar *bt;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("var") == 0) {
        eat(t->var_value);

        bt = declaracoesDeVariaveis();

        listV->push_back(bt);

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                bt = declaracoesDeVariaveis();

                listV->push_back(bt);

            } else {
                break;
            }
        }
    }
    else {
        return NULL;
    }
    return listV;
}

BlockVar *Parser::declaracoesDeVariaveis() {
    cout << "declaracoesDeVariaveis" << endl;
    Token *t;

    vector<Variable*> *listaID = listaDeIdentificadores();
    Type *p;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare(":") == 0) {
        eat(t->var_value);

        p = tipo();

    }
    else {
        return NULL;
    }
    return new BlockVar(listaID, p);
}

vector<Variable*> *Parser::listaDeIdentificadores(){
    cout << "listaDeIdentificadores" << endl;
    Token *t;
    vector<Variable*> *listVariables = new vector<Variable*>();
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        listVariables->push_back(new Variable(t->var_value));
        
        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(",") == 0) {
                eat(t->var_value);

                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_type.compare("IDENTIFICADOR") == 0){
                    eat(t->var_value);

                    listVariables->push_back(new Variable(t->var_value));

                }

            } else {
                break;
            }
        }
    }
    else {
        return NULL;
    }
    return listVariables;
}

vector<DeclarationFunction*> *Parser::parteDeDeclaracoesDeSubRotinas(){
    cout << "parteDeDeclaracoesDeSubRotinas" << endl;
    Token *t;

    vector<DeclarationFunction*> *rotinas = new vector<DeclarationFunction*>();

    while(true) {
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("procedure") == 0){

            DeclarationFunction *p = declaracaoDeProcedimento();

            rotinas->push_back(p);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }
        } 
        else if(t->var_value.compare("function") == 0){

            DeclarationFunction *f = declaracaoDeFuncao();

            rotinas->push_back(f);

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }
        }
        else {
            break;
        }
    }

    if(rotinas->size() == 0) {
        return NULL;
    }

    return rotinas;
}

DeclarationFunction *Parser::declaracaoDeProcedimento(){
    cout << "declaracaoDeProcedimento" << endl;
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
    cout << "declaracaoDeFuncao" << endl;
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
                    eat(t->var_value);

                    functionType = new Variable(t->var_value);

                    t = Lexicon::getTokenList(this->currentToken);
                    if(t->var_value.compare(";") == 0) {
                        eat(t->var_value);
                        
                        block = bloco();

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
    }
    else {
        return NULL;
    }
    return new DeclarationFunction(functionName, listParams, functionType, block);
}


vector<FormalParms*> *Parser::parametrosFormais(){
    cout << "parametrosFormais" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);

    FormalParms *p;
    vector<FormalParms*> *params = new vector<FormalParms*>();

    if(t->var_value.compare("(") == 0) {
        eat(t->var_value);

        p = secoesDeParametrosFormais();
        params->push_back(p);

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(":") == 0) {
                eat(t->var_value);

                p = secoesDeParametrosFormais();
                params->push_back(p);

            } else {
                break;
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
    cout << "secoesDeParametrosFormais" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);

    string op = "", type = "";
    vector<Variable*> *params = NULL;

    if(t->var_value.compare("var") == 0 || t->var_type.compare("IDENTIFICADOR") == 0) {
        if(t->var_value.compare("var") == 0){
            eat(t->var_value);
        }

        op = t->var_value;
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
            return NULL;
        }
    }
    else if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

            op = t->var_value;
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
                return NULL;
            }

    }
    else if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

        op = t->var_value;
        params = listaDeIdentificadores();

        if(params == NULL)
            return NULL;

    }
    else {
        return NULL;
    }
    return new FormalParms(op, params, type);
}

vector<Statement*> *Parser::comandoComposto(){
    cout << "comandoComposto" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<Statement*> *st = NULL;
    Statement*s = NULL;

    if(t->var_value.compare("begin") == 0) {
        eat(t->var_value);

        st = new vector<Statement*>();

        s = comando();
        if(s != NULL){
            st->push_back(s);

            while(true){
                t = Lexicon::getTokenList(this->currentToken);
                if(t->var_value.compare(";") == 0) {
                    eat(t->var_value);

                    s = comando();

                    st->push_back(s);

                } else {
                    break;
                }
            }
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
    cout << "comando" << endl;
    Token *t;
    
    Number *n = NULL; StatementWithoutLabel *st = NULL;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("NUMERO") == 0){
        eat(t->var_value);

        n = new Number(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
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
    cout << "comandoSemRotulo" << endl;
    Token *t;

    Assign *assign = NULL;
    Declaration *declaration = NULL;
    Number *goto_ = NULL;
    vector<Statement*> *statements = NULL;
    Branch *branch = NULL;
    Loop *loop = NULL;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        
        //Revisar essa parte da gramática
        t = Lexicon::getTokenList(this->currentToken + 1);
        if(t->var_value.compare(":=") == 0 || t->var_value.compare("(") != 0) {
            assign = atribuicoes();
        }
        else {
            if(t->var_value.compare("(") == 0) {
                declaration = chamadaDeFuncao();
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
    else {
        loop = comandoRepetitivo();
        if(loop == NULL)
            return NULL;
    }
    return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop);
}

Assign *Parser::atribuicoes(){
    cout << "atribuicoes" << endl;
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
    cout << "chamadaDeProcedimentos" << endl;
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
    else {
        return NULL;
    }
    return new Declaration(v, exs);
}

Number *Parser::desvios(){
    cout << "desvio" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);

    Number *n= NULL;

    if(t->var_value == "goto") {
        eat(t->var_value);
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("NUMERO") == 0){
            eat(t->var_value);

            n = new Number(t->var_value);

        }
    }
    else {
        return NULL;
    }
    return n;
}

Branch *Parser::comandoCondicional(){
    cout << "comandoCondicional" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);

    Expression *ex = NULL; StatementWithoutLabel *st1 = NULL; StatementWithoutLabel *st2 = NULL;

    if(t->var_value == "if") {
        eat(t->var_value);

        ex = expressoes();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "then") {
            eat(t->var_value);

            st1 = comandoSemRotulo();

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
    else {
        return NULL;
    }

    return new Branch(ex, st1, st2);
}

Loop *Parser::comandoRepetitivo(){
    cout << "comandoRepetitivo" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);

    Expression *ex = NULL; StatementWithoutLabel *st1 = NULL;

    if(t->var_value == "while") {
        eat(t->var_value);

        ex = expressoes();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "do") {
            eat(t->var_value);

            st1 = comandoSemRotulo();
        
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }

    return new Loop(ex, st1);
}

vector<Expression*> *Parser::listaDeExpressoes(){
    cout << "listaDeExpressoes" << endl;
    vector<Expression*> *listEx = new vector<Expression*>();
    Expression *ex = NULL;

    ex = expressoes();

    if(ex != NULL)
        listEx->push_back(ex);

    ex = NULL;

    Token *t;
    while(true){
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(",") == 0) {
            eat(t->var_value);

            ex = expressoes();

            if(ex != NULL)
                listEx->push_back(ex);

            ex = NULL;
        } else {
            break;
        }
    }
    if(listEx->size() == 0)
        return NULL;
    return listEx;
}

Expression *Parser::expressoes(){
    cout << "expressao" << endl;
    SimpleExpression *simpleEx1 = expressaoSimples();
    string s = relacao();
    SimpleExpression *simpleEx2 = expressaoSimples();
    if(simpleEx1 == NULL)
        return NULL;
    return new Expression(simpleEx1, s, simpleEx2);
}

string Parser::relacao(){
    cout << "relacao" << endl;
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
    cout << "expressaoSimples" << endl;
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
    cout << "termo" << endl;
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
    cout << "fator" << endl;
    Token *t;
    Variable *v;
    Declaration *d1 = NULL; Number *n1 = NULL; Expression *ex1 = NULL; Factor *f1 = NULL;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);
        
        v = new Variable(t->var_value);

        d1 = new Declaration(v, fatorAux());

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
    cout << "fatorAux" << endl;
    Token *t = Lexicon::getTokenList(this->currentToken);
    vector<Expression*> *exs = NULL;

	if(t->var_value.compare("(") == 0) {
		eat(t->var_value);

		exs = listaDeExpressoes();

		t = Lexicon::getTokenList(this->currentToken);
		if(t->var_value.compare(")") == 0) {
			eat(t->var_value);
		} else {
			return NULL;
		}
	} else {
		if(t->var_value.compare("+") == 0 || t->var_value.compare("-") == 0 ||
		   t->var_type.compare("NUMERO") == 0 || t->var_type.compare("IDENTIFICADOR") == 0 ||
		   t->var_type.compare("(") == 0 || t->var_value.compare("not") == 0) {
			exs = listaDeExpressoes();
		} else {
			return NULL;
		}
	}

    if(exs == NULL)
        return NULL;
    return exs;
}

Declaration *Parser::variavel(){
    cout << "variavel" << endl;
    Token *t;
    Variable *v = NULL;
    vector<Expression*> *exs = NULL;
    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        v = new Variable(t->var_value);
        
        exs = listaDeExpressoes();
    
    }
    else {
        return NULL;
    }
    return new Declaration(v, exs);
}

Declaration *Parser::chamadaDeFuncao(){
    cout << "chamadaDeFuncao" << endl;
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
    else {
        return NULL;
    }
    return new Declaration(v, exs);
}
