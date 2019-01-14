#pragma once

#include "Exception.hpp"
#include "IOperand.h"
#include "VM.hpp"

class Parser : public VM {

public:
	
	void			checkLine( void );

};
