#include "Parser.hpp"
#include <iostream>
#include <sstream>

Parser::Parser() {
}

Parser::~Parser() { }

eOperandType 	Parser::getType( void ) {

	std::regex arrayMatch[5] = {std::regex("int8\\([-]?[0-9]+\\).*"),
								 std::regex("int16\\([-]?[0-9]+\\).*"),
								 std::regex("int32\\([-]?[0-9]+\\).*"),
								std::regex("float\\([-]?[0-9]+.[0-9]+\\).*"),
								std::regex("double\\([-]?[0-9]+.[0-9]+\\).*")
	};

	std::regex arrayReplace[5] = {std::regex("^int8\\("),
								   std::regex("^int16\\("),
								   std::regex("^int32\\("),
								  std::regex("^float\\("),
								  std::regex("^double\\(")
	};

	for (int i = 0; i < 5; i++) {
		if ( std::regex_match(_line, arrayMatch[i] )){
			_line = regex_replace(_line, arrayReplace[i], "");
			return (eOperandType(i));
		}
	}

	std::cout << "Error [ line " << _iter << " ] : Wrong argument" << std::endl;
	exit(0);

}

eCommandType 	Parser::getCommand( void ) {

	std::regex arrayMatch[11] = {std::regex("^(push).*"), 
								std::regex("^(pop).*"),
								std::regex("^(dump).*"),
								std::regex("^(assert).*"),
								std::regex("^(add).*"),
								std::regex("^(sub).*"),
								std::regex("^(mul).*"),
								std::regex("^(div).*"),
								std::regex("^(mod).*"),
								std::regex("^(print).*"),
								std::regex("^(exit).*")
	};

	std::regex arrayReplace[11] = {std::regex("^(push)\\s+"), 
								std::regex("^(pop)\\s*"),
								std::regex("^(dump)\\s*"),
								std::regex("^(assert)\\s+"),
								std::regex("^(add)\\s*"),
								std::regex("^(sub)\\s*"),
								std::regex("^(mul)\\s*"),
								std::regex("^(div)\\s*"),
								std::regex("^(mod)\\s*"),
								std::regex("^(print)\\s*"),
								std::regex("^(exit)\\s*")
	};

	for (int i = 0; i < 11; i++) {
		if ( std::regex_match(_line, arrayMatch[i] )){
			_line = regex_replace(_line, arrayReplace[i], "");
			return (eCommandType (i));
		}
	}
	std::cout << "Error [ line " << _iter << " ] Unknown command'" << _line << "'" << std::endl;
	exit(0);
}

void			Parser::pushFunc() {
	eOperandType opType;

	opType = getType();
	_array.push_back(_creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType));
	if (_line != "" && std::regex_match(_line, std::regex("\\)\\s*((?=;).*|\\s*)")) ) {
		std::cout << "Error [ line " << _iter << " ] : Invalid symbols" << std::endl;
		exit(0);
	}
	_line = regex_replace(_line, std::regex("^\\)"), "");
}

void			Parser::assertFunc() {
	eOperandType opType;
	const IOperand* operand;

	opType = getType();
	operand = _creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType);

	if (_array.size() < 1) {
		std::cout << "Error [ line " << _iter << " ] : assert on empty stack" << std::endl;
		exit(0);
	}

	if (operand->getType() != _array[_array.size() - 1]->getType() || operand->toString() != _array[_array.size() - 1]->toString()) {
		std::cout << "Error [ line " << _iter << " ] : assert error" << std::endl;
		exit(0);
	}

	if (_line != "" && std::regex_match(_line, std::regex("\\)\\s*((?=;).*|\\s*)")) ) {
		std::cout << "Error [ line " << _iter << " ] : Invalid symbols" << std::endl;
		exit(0);
	}
	_line = regex_replace(_line, std::regex("^\\)"), "");
}

void			Parser::mathOp(eCommandType cmdType) {

	if (_array.size() < 2) {
		std::cout << "Error [ line " << _iter << " ] : no enought arguments in stack for ' " << _line << " ' command" << std::endl;
		exit(0);
	}

	switch (cmdType) {
		case add:
		_array.push_back(_creator.createOperand( *(_array[_array.size() - 1]) + *(_array[_array.size() - 2])) );
		break;
	case sub:
		_array.push_back(_creator.createOperand( *(_array[_array.size() - 1]) - *(_array[_array.size() - 2])) );
		break;
	case mul:
		_array.push_back(_creator.createOperand(*(_array[_array.size() - 1]) *  *(_array[_array.size() - 2])) );
		break;
	case Div:
		_array.push_back(_creator.createOperand(*(_array[_array.size() - 1]) / *(_array[_array.size() - 2])) );
		break;
	case mod:
		_array.push_back(_creator.createOperand(*(_array[_array.size() - 1]) % *(_array[_array.size() - 2])) );
		break;
	default:
		std::cout << "Error [ line " << _iter << " ] Unknown command'" << _line << "'" << std::endl;
	}
}

void			Parser::parseLine() {
	eCommandType cmdType;
	
	cmdType = getCommand();
	switch (cmdType) {
	case push:
		pushFunc(); break;
	case pop:
		if (_array.size() == 0) {
			std::cout << "Error [ line " << _iter << " ] : pop on empty stack" << std::endl;
			exit(0);
		}
		else
			_array.pop_back();
		break;
	case dump:
		for (int i = 0; i < _array.size(); i++)
			std::cout << _array[i]->toString() << std::endl;
		break;
	case assert:
		assertFunc(); break;
	case add:
		mathOp(add); break;
	case sub:
		mathOp(sub); break;
	case mul:
		mathOp(mul); break;
	case Div:
		mathOp(Div); break;
	case mod:
		mathOp(mod); break;
	case print:
		if (_array.size() == 0) {
			std::cout << "Error [ line " << _iter << " ] : print empty stack" << std::endl;
			exit(0);
		}
		else if (_array[_array.size() - 1]->getType() != Int8) {
			std::cout << "Error [ line " << _iter << " ] : print empty stack" << std::endl;
			exit(0);
		}
		else
			std::cout << static_cast<char>(stoi(_array[_array.size() - 1]->toString())) << std::endl; break;
	case Exit:
		exit(0);
	default:
		std::cout << "Error [ line " << _iter << " ] : Unknown command '" << _line << "'" << std::endl;
		exit(0);
	}
	if (_line != "" && std::regex_match(_line, std::regex("\\s*((?=;).*|\\s*)")) ) {
		std::cout << "Error [ line " << _iter << " ] : Invalid symbols" << std::endl;
		exit(0);
	}
}

void		Parser::setLine(std::string line) {
	_line = line;
}

void		Parser::setIter(int i) {
	_iter = i;
}

std::string	Parser::getLine() {
	return (_line);
}

