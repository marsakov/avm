#pragma once

#include "IOperand.h"
#include "Creator.hpp"
#include <regex>
#include <vector>

class Parser {

	Creator 						_creator;
	std::string						_line;
	int								_iter;
	std::vector<const IOperand*>	_array;

public:

	Parser( void );
	~Parser( void );
	
	void			parseLine( void );
	void			setLine(std::string line);
	void			setIter(int i);
	std::string		getLine( void );
	eOperandType 	getType( void );
	eCommandType 	getCommand( void );

	void			pushFunc();
	void			assertFunc();
	void			mathOp(eCommandType cmdType);
};
