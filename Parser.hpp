#pragma once

#include "IOperand.h"
#include "Creator.hpp"
#include "Exception.hpp"
#include <regex>
#include <vector>
#include <iostream>
#include <sstream>

class Parser {

	Creator 						_creator;
	std::string						_line;
	int								_iter;
	std::vector<const IOperand*>	_array;
	std::string 					_msg;

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
