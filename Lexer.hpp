#pragma once

#include "Exception.hpp"
#include "IOperand.hpp"

class Lexer : public VM {

public:
	
	Lexer( void );
	~Lexer( void );
	
	void							checkLine( void );
	// void							setLine(std::string line);
	// void							setIter(int i);
	// std::string						getLine( void );
};
