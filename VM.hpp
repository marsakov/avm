#pragma once

#include "IOperand.h"
#include "Creator.hpp"
#include "Exception.hpp"

class VM {

	int								_iter;
	Creator 						_creator;
	std::string						_line;
	std::string 					_msg;
	std::vector<const IOperand*>	_array;
	std::regex						_arrayMatchCmd[11];
	std::regex						_arrayReplaceCmd[11];
	std::regex						_arrayMatchType[5];
	std::regex						_arrayReplaceType[5];


public:
	
	bool							exitInstr;

	VM( void );
	~VM( void );
	
	void							parseLine( void );
	void							setLine(std::string line);
	void							setIter(int i);
	std::string						getLine( void );
	eOperandType 					getType( void );
	eCommandType 					getCommand( void );

	void							pushFunc();
	void							assertFunc();
	void							mathOp(eCommandType cmdType);
};
