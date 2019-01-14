#pragma once

#include "Exception.hpp"
#include "IOperand.h"
#include "VM.hpp"

class Lexer : public VM {

public:
	
	void	checkLine( void );
};
