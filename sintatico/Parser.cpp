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
    vector<BlockType*> *types = parteDeDefinicoesDeTipos();
    vector<BlockVar*> *variables = parteDeDeclaracoesDeVariaveis();
    vector<DeclarationFunction*> *rotinas = parteDeDeclaracoesDeSubRotinas();
    vector<Statement*> *statements = comandoComposto();

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

        }

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

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }
    }

    return labels;
}

vector<BlockType*> *Parser::parteDeDefinicoesDeTipos() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<BlockType*> *types = new vector<BlockType*>();
    
    if(t->var_value.compare("type") == 0) {
        eat(t->var_value);

        types->push_back(definicaoDeTipo());

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
    return types;
}

BlockType *Parser::definicaoDeTipo() {
    Token *t = Lexicon::getTokenList(this->currentToken);

    Type *ntype;
    Variable *variable;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        eat(t->var_value);

        variable = new Variable(t->var_value);
        
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("=") == 0) {
            eat(t->var_value);

            ntype = tipo();

        }
    }
    return new BlockType(variable, ntype);
}

Type *Parser::tipo() {
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

        }
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
            
        }
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
    return listVariables;
}

vector<DeclarationFunction*> *Parser::parteDeDeclaracoesDeSubRotinas(){
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

    return rotinas;
}

DeclarationFunction *Parser::declaracaoDeProcedimento(){
    Token *t;

    Variable *functionName; vector<Variable*> *listParams; Block *block;

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

        }
    }
    return new DeclarationFunction(functionName, listParams, NULL, block);
}

DeclarationFunction *Parser::declaracaoDeFuncao(){
    Token *t;

    Variable *functionName; vector<Variable*> *listParams; Variable *functionType; Block *block;

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);

            functionName = new Variable(t->var_value);

            listParams = parametrosFormais();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
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
                }
            }
    
        }
    }
    return new DeclarationFunction(functionName, listParams, functionType, block);
}

vector<Variable*> *Parser::parametrosFormais(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    vector<Variable*> *params = NULL;
    if(t->var_value.compare("(") == 0) {
        eat(t->var_value);

        params = secoesDeParametrosFormais();

        while(true){
            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);

                vector<Variable*> *paramsAux = secoesDeParametrosFormais();
                params->insert(
                    params->end(),
                    std::make_move_iterator(paramsAux->begin()),
                    std::make_move_iterator(paramsAux->end())
                );
            } else {
                break;
            }
        }
      
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(")") == 0) {
            eat(t->var_value);
        }
    }
    return params;
}

vector<Variable*> *Parser::secoesDeParametrosFormais(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    vector<Variable*> *params = NULL;

    if(t->var_value.compare("var") == 0 || t->var_type.compare("IDENTIFICADOR") == 0) {
        if(t->var_value.compare("var") == 0){
            eat(t->var_value);
        }

        params = listaDeIdentificadores();

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);
        }

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_type.compare("IDENTIFICADOR") == 0){
            eat(t->var_value);

            if(params == NULL) {
                params = new vector<Variable*>();
                params->push_back(new Variable(t->var_value));
            } else {
                params->push_back(new Variable(t->var_value));
            }

        }
    }
    else if(t->var_value.compare("function") == 0) {
        eat(t->var_value);

            params = listaDeIdentificadores();

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_value.compare(";") == 0) {
                eat(t->var_value);
            }

            t = Lexicon::getTokenList(this->currentToken);
            if(t->var_type.compare("IDENTIFICADOR") == 0){
                eat(t->var_value);

                if(params == NULL) {
                    params = new vector<Variable*>();
                    params->push_back(new Variable(t->var_value));
                } else {
                    params->push_back(new Variable(t->var_value));
                }

            }
    }
    else if(t->var_value.compare("procedure") == 0) {
        eat(t->var_value);

        params = listaDeIdentificadores();

    }
    return params;
}

vector<Statement*> *Parser::comandoComposto(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    
    vector<Statement*> *st = NULL;
    Statement*s = NULL;

    if(t->var_value.compare("begin") == 0) {
        eat(t->var_value);

        st = new vector<Statement*>();

        s = comando();
        
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

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value.compare("end") == 0) {
            eat(t->var_value);
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
        if(t->var_value.compare(";") == 0) {
            eat(t->var_value);

            st = comandoSemRotulo();
        
        }

    } else {
        st = comandoSemRotulo();
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

    t = Lexicon::getTokenList(this->currentToken);
    if(t->var_type.compare("IDENTIFICADOR") == 0){
        t = Lexicon::getTokenList(this->currentToken + 1);
        if(t->var_value.compare("[") == 0) {
            assign = atribuicoes();
        }
        else {
            if(t->var_value.compare("(") == 0) {
                declaration = chamadaDeFuncao();
            }
        }
    }
    else if(t->var_value.compare("goto") == 0) {
        goto_ = desvios();
    }
    else if(t->var_value.compare("begin") == 0) {
        statements = comandoComposto();
    }
    else if(t->var_value.compare("if") == 0) {
        branch = comandoCondicional();
    }
    else {
        loop = comandoRepetitivo();
    }
    return new StatementWithoutLabel(assign, declaration, goto_, statements, branch, loop);
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
    }
    return n;
}

Branch *Parser::comandoCondicional(){
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
    }

    return new Branch(ex, st1, st2);
}

Loop *Parser::comandoRepetitivo(){
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
    }

    return new Loop(ex, st1);
}

vector<Expression*> *Parser::listaDeExpressoes(){
    vector<Expression*> *listEx = NULL;
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

    return listEx;
}

Expression *Parser::expressoes(){
    SimpleExpression *simpleEx1 = expressaoSimples();
    string s = relacao();
    SimpleExpression *simpleEx2 = expressaoSimples();

    return new Expression(simpleEx1, s, simpleEx2);
}

string Parser::relacao(){
    Token *t = Lexicon::getTokenList(this->currentToken);
    if(t->var_value == "=") {
        eat(t->var_value);
        return "=";
    } else if(t->var_value == "<>") {
        eat(t->var_value);
        return "<>";
    } else if(t->var_value == "<") {
        eat(t->var_value);
        return "<";
    } else if(t->var_value == "<=") {
        eat(t->var_value);
        return "<=";
    } else if(t->var_value == ">") {
        eat(t->var_value);
        return ">";
    } else if(t->var_value == ">=") {
        eat(t->var_value);
        return ">=";
    } else {
        return NULL;
    }
}

SimpleExpression *Parser::expressaoSimples(){
    Token *t = Lexicon::getTokenList(this->currentToken);

    string op, opAux;
    Term *term = NULL, *TAux = NULL;
    SimpleExpression *simpleEx = NULL, *simpleExAux1, *simpleExAux2;
    bool firstTime;

    if(t->var_value == "+") {
        eat(t->var_value);
        op = t->var_value;
    } else if(t->var_value == "-") {
        eat(t->var_value);
        op = t->var_value;
    }

    term = termo();

    
    while(true) {

        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "+") {
            eat(t->var_value);
            opAux = t->var_value;
            TAux = termo();
        } else if(t->var_value == "-") {
            eat(t->var_value);
            opAux = t->var_value;
            TAux = termo();
        } else if(t->var_value == "or") {
            eat(t->var_value);
            opAux = t->var_value;
            TAux = termo();
        } else {
            break;
        }

        //Cria uma lista ligada
        if(firstTime) {
            simpleEx = new SimpleExpression(opAux, TAux, NULL);
            simpleExAux1 = simpleEx;
        } else {
            simpleExAux2 = new SimpleExpression(opAux, TAux, NULL);
            simpleExAux1->simpleExpressions = simpleExAux2;
        }
        
        TAux = NULL;
    }

    return new SimpleExpression(op, term, simpleEx);
}

Term *Parser::termo(){
    Factor *f = NULL;
    f = fator();

    vector<Factor*> *listF = new vector<Factor*>();
    Factor *f = NULL;

    Token *t = Lexicon::getTokenList(this->currentToken);
    while(true) {
        t = Lexicon::getTokenList(this->currentToken);
        if(t->var_value == "*") {
            eat(t->var_value);
            f = fator();
        } else if(t->var_value == "div") {
            eat(t->var_value);
            f = fator();
        } else if(t->var_value == "and") {
            eat(t->var_value);
            f = fator();
        } else {
            break;
        }

        if(f != NULL){
            listF->push_back(f);
        }

        f = NULL;
    }

    return new Term(f, listF);
}

Factor *Parser::fator(){
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
        
        if(t->var_value.compare(")") == 0) {
            eat(t->var_value);
        }
    }
    else if(t->var_value == "not") {
        eat(t->var_value);

        f1 = fator();
    
    }

    return new Factor(d1, n1, ex1, f1);
}

vector<Expression*> *Parser::fatorAux() {
    Token *t = Lexicon::getTokenList(this->currentToken);
    vector<Expression*> *exs = NULL;
    if(t->var_value.compare("(") != 0){

        exs = listaDeExpressoes();
    
    } else {
        if(t->var_value.compare("(") == 0) {
            eat(t->var_value);
    
            exs = listaDeExpressoes();

            if(t->var_value.compare(")") == 0) {
                eat(t->var_value);
            }
        }
    }
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
        
        exs = listaDeExpressoes();
    
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
        }

    }
    return new Declaration(v, exs);
}