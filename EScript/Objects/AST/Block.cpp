// Block.cpp
// This file is part of the EScript programming language.
// See copyright notice in EScript.h
// ------------------------------------------------------
#include "Block.h"
#include "../../Parser/CompilerContext.h"

#include <sstream>
#include <utility>

using namespace EScript;

//! (ctor)
Block::Block(int lineNr):
		filenameId(0),vars(NULL),line(lineNr),
		continuePos(POS_DONT_HANDLE),breakPos(POS_DONT_HANDLE),exceptionPos(POS_DONT_HANDLE),jumpPosA(0) {
	//ctor
}

//! (dtor)
Block::~Block() {

	delete vars;
	//dtor
}

//! ---|> [Object]
std::string Block::toString()const {
	static int depth=0;
	std::ostringstream sprinter;
	sprinter << "{" <<  std::endl;
	depth++;
	for ( cStatementCursor c = statements.begin();  c != statements.end(); ++c) {
		for (int i=0;i<depth;++i) sprinter << "\t";
			sprinter <<".";
//		sprinter <<  c->getObject()->toString() <<  std::endl;
	}
	depth--;
	for (int i=0;i<depth;++i) sprinter << "\t";
	sprinter << "}";
	return sprinter.str();
}

bool Block::declareVar(StringId id) {
	if(vars==NULL){
		vars=new declaredVariableMap_t();
	}
	std::pair<declaredVariableMap_t::iterator,bool> result= vars->insert(id);
	return result.second;
}

void Block::addStatement(const Statement & s) {
	if(s.isValid())
		statements.push_back(s);
}

//! ---|> Object
void Block::_asm(CompilerContext & ctxt){
	
//	ctxt.out<<"{\n";
	if(vars) 
		ctxt.pushLocalVars(*vars);

	for ( statementCursor c = statements.begin();  c != statements.end(); ++c) {
		c->_asm(ctxt);
//		out<<"\n";
	}
	if(vars){
		for(std::set<StringId>::const_iterator it = vars->begin();it!=vars->end();++it){
			ctxt.addInstruction(Instruction::createResetLocalVariable(ctxt.getVarIndex(*it)));
//			ctxt.out << "reset $" << ctxt.getVarIndex(*it) << "\n";
		}
		ctxt.popLocalVars();
	}

//	ctxt.out<<"}\n";

}