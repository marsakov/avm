#pragma once

#include "Exception.hpp"
#include "IOperand.hpp"

class Parser : public VM {

public:

	Parser( void );
	~Parser( void );
	
	void			checkLine( void );

};
