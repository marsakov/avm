#include "Parser.hpp"

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
	_msg = "Error [ line " + std::to_string(_iter) + " ] : Wrong argument";
	throw Exception(_msg.c_str());
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
	_msg = "Error [ line " + std::to_string(_iter) + " ] : Unknown command'" + _line + "'";
	throw Exception(_msg.c_str());
}

void			Parser::pushFunc() {
	eOperandType opType;

	opType = getType();
	_array.push_back(_creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType));
	if (_line != "" && std::regex_match(_line, std::regex("\\)\\s*((?=;).*|\\s*)")) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Invalid symbols";
		throw Exception(_msg.c_str());
	}
	_line = regex_replace(_line, std::regex("^\\)"), "");
}

void			Parser::assertFunc() {
	eOperandType opType;
	const IOperand* operand;

	opType = getType();
	operand = _creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType);

	if (_array.size() < 1) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : assert on empty stack";
		throw Exception(_msg.c_str());
	}

	if (operand->getType() != _array[_array.size() - 1]->getType() || operand->toString() != _array[_array.size() - 1]->toString()) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : assert error";
		throw Exception(_msg.c_str());
	}

	if (_line != "" && std::regex_match(_line, std::regex("\\)\\s*((?=;).*|\\s*)")) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Invalid symbols";
		throw Exception(_msg.c_str());
	}
	_line = regex_replace(_line, std::regex("^\\)"), "");
}

void			Parser::mathOp(eCommandType cmdType) {

	if (_array.size() < 2) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : no enought arguments in stack for ' " + _line + "' command";
		throw Exception(_msg.c_str());
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
	    if (stof(_array[_array.size() - 2]->toString()) == 0)
            throw Exception("div by 0");
		_array.push_back(_creator.createOperand(*(_array[_array.size() - 1]) / *(_array[_array.size() - 2])) );
		break;
	case mod:
        if (stof(_array[_array.size() - 2]->toString()) == 0)
            throw Exception("mod by 0");
		_array.push_back(_creator.createOperand(*(_array[_array.size() - 1]) % *(_array[_array.size() - 2])) );
		break;
	default:
	    _msg = "Error [ line " + std::to_string(_iter) + " ] : Unknown command'" + _line + "'";
	    throw Exception(_msg.c_str());
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
			_msg = "Error [ line " + std::to_string(_iter) + " ] : pop on empty stack";
			throw Exception(_msg.c_str());
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
			_msg = "Error [ line " + std::to_string(_iter) + " ] : print empty stack";
			throw Exception(_msg.c_str());
		}
		else if (_array[_array.size() - 1]->getType() != Int8) {
			_msg = "Error [ line " + std::to_string(_iter) + " ] : print wrong arg __must_be_int8__)";
			throw Exception(_msg.c_str());
		}
		else
			std::cout << static_cast<char>(stoi(_array[_array.size() - 1]->toString())) << std::endl; break;
	case Exit:
		exit(0);
	default:
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Unknown command'" + _line + "'";
		throw Exception(_msg.c_str());
	}
	if (_line != "" && std::regex_match(_line, std::regex("\\s*((?=;).*|\\s*)")) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Invalid symbols";
		throw Exception(_msg.c_str());
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

