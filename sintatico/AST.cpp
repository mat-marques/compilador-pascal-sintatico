#include "AST.h"

void Program::printProgram(string fileName) {
    ofstream ast;
  	ast.open (fileName);

  	ast << "Árvore Sintática Abstrata" << "\n\n";

	if(this->programName != NULL) {
		ast << this->programName->variable << " ";
	}

	if(this->listVariables != NULL) {
        for (std::vector<Variable*>::iterator it = this->listVariables->begin(); it != this->listVariables->end(); ++it) {
            ast << (*it)->variable << " ";
        }
        ast << "\n\n";
	}

	if(this->block != NULL) {
		this->block->printBlock(ast);
	}

	ast.close();
}


void Variable::printVariable(ofstream & hFile){
	hFile << this->variable;
}

void Number::printNumber(ofstream & hFile) {
	hFile << this->value;
}

void Assign::printAssign(ofstream & hFile){
	this->dec->printDeclaration(hFile);
	hFile << ":= ";
	this->ex->printExpression(hFile);
}

void BlockType::printBlockType(ofstream & hFile){

	hFile << this->v->variable << " ";

	Type *nextA = this->t;
	while(nextA != NULL) {

		if(nextA->type != NULL) {
			hFile << nextA->type->variable << " ";
			hFile << "\n";
		} else {
			hFile << "array ";
		}
		nextA = nextA->types;
	}

}

void BlockVar::printBlockVar(ofstream & hFile) {

	for (std::vector<Variable*>::iterator it = this->v->begin(); it != this->v->end(); ++it) {
		hFile << (*it)->variable << " ";
	}

	Type *nextA = this->t;
	while(nextA != NULL) {

		if(nextA->type != NULL) {
			hFile << nextA->type->variable << " ";
			hFile << "\n";
		} else {
			hFile << "array ";
		}
		nextA = nextA->types;
	}
}

void Block::printBlock(ofstream & hFile) {
	if(this->labels != NULL){
		hFile << "labels:\n";
		for (std::vector<Number*>::iterator it = this->labels->begin(); it != this->labels->end(); ++it) {
			hFile << (*it)->value << " ";
		}
		hFile << "\n\n";
	}

	if(this->varTypes != NULL){
		hFile << "types:\n";
        for (std::vector<BlockType*>::iterator it = this->varTypes->begin(); it != this->varTypes->end(); ++it) {
        	(*it)->printBlockType(hFile);
        }
        hFile << "\n";
	}

	if(this->varVariables != NULL){
		hFile << "var:\n";
        for (std::vector<BlockVar*>::iterator it = this->varVariables->begin(); it != this->varVariables->end(); ++it) {
        	(*it)->printBlockVar(hFile);
        }
        hFile << "\n";
	}

	if(this->rotinas != NULL){
        for (std::vector<DeclarationFunction*>::iterator it = this->rotinas->begin(); it != this->rotinas->end(); ++it) {
        	(*it)->printDeclarationFunction(hFile);
        }
        hFile << "\n";
	}

	if(this->statements != NULL){
		hFile << "begin ";
        for (std::vector<Statement*>::iterator it = this->statements->begin(); it != this->statements->end(); ++it) {
        	(*it)->printStatement(hFile);
        }
		hFile << "\n";
		hFile << "end";
		hFile << "\n\n";
	}
}


void Branch::printBranch(ofstream & hFile) {
	if(this->ex != NULL){
		hFile << "if ";
		this->ex->printExpression(hFile);
	}

	if(this->st1 != NULL){
		hFile << " then" << "\n";
		this->st1->printStatementWithoutLabel(hFile);
	}

	if(this->st2 != NULL){
		hFile << "else" << "\n";
		this->st2->printStatementWithoutLabel(hFile);
		hFile << "\n";
	}

}


void Declaration::printDeclaration(ofstream & hFile) {
	if(this->v != NULL)
		hFile << this->v->variable << " ";

	if(this->ex != NULL) {
		for (std::vector<Expression*>::iterator it = this->ex->begin(); it != this->ex->end(); ++it) {
			(*it)->printExpression(hFile);
		}
	}
}

void DeclarationFunction::printDeclarationFunction(ofstream & hFile) {
	if(this->functionType != NULL)
		hFile << "function ";
	else
		hFile << "procedure ";

	if(this->functionName != NULL)
		hFile << this->functionName->variable << " ";

	if(this->listParams != NULL) {
		for (std::vector<FormalParms*>::iterator it = this->listParams->begin(); it != this->listParams->end(); ++it) {
			(*it)->printFormalParms(hFile);
		}
	}

	if(this->functionType != NULL)
		hFile << this->functionType->variable << "\n";
	else
		hFile << "\n";

	if(this->block != NULL)
		this->block->printBlock(hFile);
}

void SimpleExpression::printSimpleExpression(ofstream & hFile) {
	if(this->op != "")
		hFile << this->op << " ";

	if(this->t != NULL)
		this->t->printTerm(hFile);

	if(this->simpleExpressions != NULL)
		this->simpleExpressions->printSimpleExpression(hFile);
}

void Expression::printExpression(ofstream & hFile) {
	if(this->ex1 != NULL)
		this->ex1->printSimpleExpression(hFile);

	if(this->op != ""){
		hFile << this->op << " ";
		if(this->ex2 != NULL)
			this->ex2->printSimpleExpression(hFile);
	}
}

void Factor::printFactor(ofstream & hFile) {
	if(this->d != NULL) {
		this->d->printDeclaration(hFile);
	}
	else if(this->n != NULL) {
		hFile << this->n->value << " ";
	}
	else if(this->ex != NULL) {
		this->ex->printExpression(hFile);
	}
	else {
		if(this->f != NULL)
			this->f->printFactor(hFile);
	}
}


void Term::printTerm(ofstream & hFile) {
	if(this->f != NULL)
		this->f->printFactor(hFile);

	if(this->op != ""){
		hFile << this->op << " ";
		if(this->t != NULL)
			this->t->printTerm(hFile);
	}
}

void FormalParms::printFormalParms(ofstream & hFile) {
	if(this->variables != NULL){
		for (std::vector<Variable*>::iterator it = this->variables->begin(); it != this->variables->end(); ++it) {
			hFile << (*it)->variable << " ";
		}
	}

	if(this->type != "")
		hFile << this->type << " ";

}

void Loop::printLoop(ofstream & hFile) {
	if(this->ex != NULL){
		hFile << "while ";
		this->ex->printExpression(hFile);
	}

	if(this->statement != NULL) {
		hFile << "do" << "\n";
		this->statement->printStatementWithoutLabel(hFile);
	}
}

void Type::printType(ofstream & hFile) {
	if(this->type != NULL)
		hFile << this->type->variable;

	if(this->types != NULL){
		hFile << "array ";
		this->types->printType(hFile);
	}
}


void StatementWithoutLabel::printStatementWithoutLabel(ofstream & hFile) {
	if(this->assign != NULL)
		this->assign->printAssign(hFile);

	else if(this->branch != NULL)
		this->branch->printBranch(hFile);

	else if(this->declaration != NULL)
		this->declaration->printDeclaration(hFile);

	else if(this->goto_ != NULL)
		hFile << this->goto_->value;

	else if(this->loop != NULL)
		this->loop->printLoop(hFile);

	else if(this->read != NULL)
		this->read->printRead(hFile);

	else if(this->write != NULL)
		this->write->printWrite(hFile);

	else {
		if(this->statements != NULL){
			for (std::vector<Statement*>::iterator it = this->statements->begin(); it != this->statements->end(); ++it) {
				(*it)->printStatement(hFile);
			}
		}
	}


}

void Statement::printStatement(ofstream & hFile) {
	if(this->label != NULL) {
		hFile << this->label->value << "\n";
	} else {
		hFile << "\n";
	}

	if(this->st != NULL)
		this->st->printStatementWithoutLabel(hFile);

}

void Read::printRead(ofstream & hFile) {
	for (std::vector<Variable*>::iterator it = this->variables->begin(); it != this->variables->end(); ++it) {
		(*it)->printVariable(hFile);
	}
}


void Write::printWrite(ofstream & hFile) {
	for (std::vector<Expression*>::iterator it = this->expressions->begin(); it != this->expressions->end(); ++it) {
		(*it)->printExpression(hFile);
	}
}


void Program::removeProgram() {

	if(this->programName != NULL) {
		delete this->programName;
	}

	if(this->listVariables != NULL) {
        for (std::vector<Variable*>::iterator it = this->listVariables->begin(); it != this->listVariables->end(); ++it) {
            delete (*it);
        }
        delete this->listVariables;
	}

	if(this->block != NULL) {
		this->block->removeBlock();
		delete this->block;
	}

}

void Assign::removeAssign(){
	if(this->dec != NULL){
		this->dec->removeDeclaration();
		delete this->dec;
	}
	if(this->ex != NULL){
		this->ex->removeExpression();
		delete this->ex;
	}
}

void BlockType::removeBlockType(){

	delete this->v;

	this->t->removeType();
	delete this->t;

}

void BlockVar::removeBlockVar() {

	for (std::vector<Variable*>::iterator it = this->v->begin(); it != this->v->end(); ++it) {
		delete (*it);
	}
	if(this->t != NULL){
		this->t->removeType();
		delete this->t;
	}
}

void Block::removeBlock() {
	if(this->labels != NULL){
		for (std::vector<Number*>::iterator it = this->labels->begin(); it != this->labels->end(); ++it) {
			 delete (*it);
		}
	}

	if(this->varTypes != NULL){
        for (std::vector<BlockType*>::iterator it = this->varTypes->begin(); it != this->varTypes->end(); ++it) {
        	(*it)->removeBlockType();
        }
        delete this->varTypes;
	}

	if(this->varVariables != NULL){
        for (std::vector<BlockVar*>::iterator it = this->varVariables->begin(); it != this->varVariables->end(); ++it) {
        	(*it)->removeBlockVar();
        }
        delete this->varVariables;
	}

	if(this->rotinas != NULL){
        for (std::vector<DeclarationFunction*>::iterator it = this->rotinas->begin(); it != this->rotinas->end(); ++it) {
        	(*it)->removeDeclarationFunction();
        }
        delete this->rotinas;
	}

	if(this->statements != NULL){
        for (std::vector<Statement*>::iterator it = this->statements->begin(); it != this->statements->end(); ++it) {
        	(*it)->removeStatement();
        }
        delete this->statements;
	}
}


void Branch::removeBranch() {
	if(this->ex != NULL){
		this->ex->removeExpression();
		delete this->ex;
	}

	if(this->st1 != NULL){
		this->st1->removeStatementWithoutLabel();
		delete this->st1;
	}

	if(this->st2 != NULL){
		this->st2->removeStatementWithoutLabel();
		delete this->st2;
	}

}


void Declaration::removeDeclaration() {
	if(this->v != NULL)
		 delete this->v;

	if(this->ex != NULL) {
		for (std::vector<Expression*>::iterator it = this->ex->begin(); it != this->ex->end(); ++it) {
			(*it)->removeExpression();
		}
		delete this->ex;
	}
}

void DeclarationFunction::removeDeclarationFunction() {

	if(this->functionName != NULL)
		delete this->functionName;

	if(this->listParams != NULL) {
		for (std::vector<FormalParms*>::iterator it = this->listParams->begin(); it != this->listParams->end(); ++it) {
			(*it)->removeFormalParms();
		}
		delete this->listParams;
	}

	if(this->functionType != NULL){
		 delete this->functionType;
	}

	if(this->block != NULL){
		this->block->removeBlock();
		delete this->block;
	}
}

void SimpleExpression::removeSimpleExpression() {

	if(this->t != NULL){
		this->t->removeTerm();
		delete this->t;
	}

	if(this->simpleExpressions != NULL){
		this->simpleExpressions->removeSimpleExpression();
		delete this->simpleExpressions;
	}
}

void Expression::removeExpression() {
	if(this->ex1 != NULL){
		this->ex1->removeSimpleExpression();
		delete this->ex1;
	}


	if(this->ex2 != NULL){
		this->ex2->removeSimpleExpression();
		delete this->ex2;
	}

}

void Factor::removeFactor() {
	if(this->d != NULL) {
		this->d->removeDeclaration();
		delete this->d;
	}
	else if(this->n != NULL) {
		 delete this->n;
	}
	else if(this->ex != NULL) {
		this->ex->removeExpression();
		delete this->ex;
	}
	else {
		if(this->f != NULL){
			this->f->removeFactor();
			delete this->f;
		}
	}
}


void Term::removeTerm() {
	if(this->f != NULL){
		this->f->removeFactor();
		delete this->f;
	}

	if(this->t != NULL){
		this->t->removeTerm();
		delete this->t;
	}
}

void FormalParms::removeFormalParms() {
	if(this->variables != NULL){
		for (std::vector<Variable*>::iterator it = this->variables->begin(); it != this->variables->end(); ++it) {
			 delete (*it);
		}
		delete this->variables;
	}

}

void Loop::removeLoop() {
	if(this->ex != NULL){
		this->ex->removeExpression();
		delete this->ex;
	}

	if(this->statement != NULL) {
		this->statement->removeStatementWithoutLabel();
		delete this->statement;
	}
}

void Type::removeType() {
	if(this->type != NULL)
		 delete this->type;

	if(this->types != NULL){
		this->types->removeType();
		delete this->types;
	}
}

void StatementWithoutLabel::removeStatementWithoutLabel() {
	if(this->assign != NULL){
		this->assign->removeAssign();
		delete this->assign;
	}else if(this->branch != NULL){
		this->branch->removeBranch();
		delete this->branch;
	}else if(this->declaration != NULL){
		this->declaration->removeDeclaration();
		delete this->declaration;
	}else if(this->goto_ != NULL){
		 delete this->goto_;
	}else if(this->loop != NULL){
		this->loop->removeLoop();
		delete this->loop;
	}else if(this->read != NULL){
		this->read->removeRead();
		delete this->read;
	}else if(this->write != NULL){
		this->write->removeWrite();
		delete this->write;
	}else {
		if(this->statements != NULL){
			for (std::vector<Statement*>::iterator it = this->statements->begin(); it != this->statements->end(); ++it) {
				(*it)->removeStatement();
			}
			delete this->statements;
		}
	}


}

void Statement::removeStatement() {
	if(this->label != NULL)
		 delete this->label;


	if(this->st != NULL){
		this->st->removeStatementWithoutLabel();
		delete this->st;
	}

}

void Read::removeRead() {
	for (std::vector<Variable*>::iterator it = this->variables->begin(); it != this->variables->end(); ++it) {
		delete (*it);
	}
	delete this->variables;
}


void Write::removeWrite() {
	for (std::vector<Expression*>::iterator it = this->expressions->begin(); it != this->expressions->end(); ++it) {
		(*it)->removeExpression();
	}
	delete this->expressions;
}









