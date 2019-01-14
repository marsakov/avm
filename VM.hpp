#pragma once

#include "IOperand.h"
#include "Creator.hpp"
#include "Exception.hpp"

class VM {

	std::vector<const IOperand*>	_array;

protected:

	int								_iter;
	Creator 						_creator;
	std::string						_line;
	std::string 					_msg;
	std::regex						_cmdMatch;
	std::regex						_cmdReplace;
	std::regex						_typeMatch;
	std::regex						_typeReplace;
	std::regex						_argMatch;
	std::regex						_argReplace;


public:
	
	bool							exitInstr;

	VM( void );
	~VM( void );
	
	void							checkLine( void );
	void							setLine(std::string line, int i);
	std::string						getLine( void );
	eOperandType 					getType( void );
	eCommandType 					getCommand( void );

private:
	void							notEmpty(std::string s);
	void							mathOp(eCommandType cmdType);
	void							sqrFunc();
	void							pushFunc();
	void							assertFunc();
	void							maxFunc();
	void							minFunc();
	void							printFunc();
	void							dumpFunc();

};
