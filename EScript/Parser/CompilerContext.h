// CompilerContext.h
// This file is part of the EScript programming language.
// See copyright notice in EScript.h
// ------------------------------------------------------
#ifndef COMPILER_CONTEXT_H
#define COMPILER_CONTEXT_H

#include "../Utils/StringId.h"
#include "../Objects/Internals/Instruction.h"
#include "InstructionBlock.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>

namespace EScript {

/*! Compiler
	Input: Syntax tree made of Expressions
	Output: Byte Code made of EAssembler-Instructions */

/*! Collection of "things" used during the compilation process.
	As the compilation process is currently under development, it is not clear how this class changes 
	in the near future.	*/
class CompilerContext {
		InstructionBlock & instructions;

		typedef std::map<StringId,size_t> indexNameMapping_t;
		std::vector<indexNameMapping_t> currentLocalVariableStack;
		int currentLine;
		uint32_t currentMarkerId;
	public:
		CompilerContext(InstructionBlock & _instructions) : 
				instructions(_instructions),currentLine(-1),currentMarkerId(Instruction::JMP_TO_MARKER_OFFSET){}
		
		void addInstruction(const Instruction & newInstruction)	{	instructions.addInstruction(newInstruction,currentLine);	}

		uint32_t createMarker()									{	return currentMarkerId++;	}
		uint32_t declareString(const std::string & str)			{	return instructions.declareString(str);	}

		std::string getInstructionsAsString()const				{	return instructions.toString();	}
		StringId getLocalVarName(const int index)const			{	return instructions.getLocalVarName(index);	}

		size_t getNumLocalVars()const							{	return instructions.getNumLocalVars();	}
		std::string getStringConstant(const uint32_t index)const{	return instructions.getStringConstant(index);	}
		int getVarIndex(const StringId name)const;
	
		void pushLocalVars(const std::set<StringId> & variableNames);
		void popLocalVars();
		void setLine(int l)										{	currentLine=l;	}
	
		/*! (static) \todo // move to Compiler
			- Replaces the markers inside the assembly by jump addresses.	*/
		static void finalizeInstructions( InstructionBlock & instructions ); 
	
		std::ostringstream out; // temporary

};
}

#endif // COMPILER_CONTEXT_H