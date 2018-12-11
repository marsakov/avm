#include "VM.hpp"

VM::VM() {
	_arrayMatchCmd[0] = std::regex("^(push).*");
	_arrayMatchCmd[1] = std::regex("^(pop).*");
	_arrayMatchCmd[2] = std::regex("^(dump).*");
	_arrayMatchCmd[3] = std::regex("^(assert).*");
	_arrayMatchCmd[4] = std::regex("^(add).*");
	_arrayMatchCmd[5] = std::regex("^(sub).*");
	_arrayMatchCmd[6] = std::regex("^(mul).*");
	_arrayMatchCmd[7] = std::regex("^(div).*");
	_arrayMatchCmd[8] = std::regex("^(mod).*");
	_arrayMatchCmd[9] = std::regex("^(print).*");
	_arrayMatchCmd[10] = std::regex("^(exit).*");

	_arrayReplaceCmd[0] = std::regex("^(push)\\s+");
	_arrayReplaceCmd[1] = std::regex("^(pop)\\s*");
	_arrayReplaceCmd[2] = std::regex("^(dump)\\s*");
	_arrayReplaceCmd[3] = std::regex("^(assert)\\s+");
	_arrayReplaceCmd[4] = std::regex("^(add)\\s*");
	_arrayReplaceCmd[5] = std::regex("^(sub)\\s*");
	_arrayReplaceCmd[6] = std::regex("^(mul)\\s*");
	_arrayReplaceCmd[7] = std::regex("^(div)\\s*");
	_arrayReplaceCmd[8] = std::regex("^(mod)\\s*");
	_arrayReplaceCmd[9] = std::regex("^(print)\\s*");
	_arrayReplaceCmd[10] = std::regex("^(exit)\\s*");

	_arrayMatchType[0] = std::regex("int8\\([-]?[0-9]+\\).*");
	_arrayMatchType[1] = std::regex("int16\\([-]?[0-9]+\\).*");
	_arrayMatchType[2] = std::regex("int32\\([-]?[0-9]+\\).*");
	_arrayMatchType[3] = std::regex("float\\([-]?[0-9]+\\.[0-9]+\\).*");
	_arrayMatchType[4] = std::regex("double\\([-]?[0-9]+\\.[0-9]+\\).*");

	_arrayReplaceType[0] = std::regex("^int8\\(");
	_arrayReplaceType[1] = std::regex("^int16\\(");
	_arrayReplaceType[2] = std::regex("^int32\\(");
	_arrayReplaceType[3] = std::regex("^float\\(");
	_arrayReplaceType[4] = std::regex("^double\\(");

	exitInstr = false;
}

VM::~VM() { }

eOperandType 	VM::getType( void ) {

	for (int i = 0; i < 5; i++) {
		if ( std::regex_match(_line, _arrayMatchType[i] )){
			_line = regex_replace(_line, _arrayReplaceType[i], "");
			return (eOperandType(i));
		}
	}
	_msg = "Error [ line " + std::to_string(_iter) + " ] : Wrong argument";
	throw Exception(_msg.c_str());
}

eCommandType 	VM::getCommand( void ) {

	for (int i = 0; i < 11; i++) {
		if ( std::regex_match(_line, _arrayMatchCmd[i] )){
			_line = regex_replace(_line, _arrayReplaceCmd[i], "");
			return (eCommandType (i));
		}
	}
	_msg = "Error [ line " + std::to_string(_iter) + " ] : Unknown command'" + _line + "'";
	throw Exception(_msg.c_str());
}

void			VM::pushFunc() {
	eOperandType opType;

	opType = getType();
	_array.push_back(_creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType));
	if (_line != "" && std::regex_match(_line, std::regex("\\)\\s*((?=;).*|\\s*)")) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Invalid symbols";
		throw Exception(_msg.c_str());
	}
	_line = regex_replace(_line, std::regex("^\\)"), "");
}

void			VM::assertFunc() {
	eOperandType opType;
	const IOperand* operand;

	opType = getType();
	operand = _creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType);
	
	if (_line != "" && std::regex_match(_line, std::regex("\\)\\s*((?=;).*|\\s*)")) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Invalid symbols";
		throw Exception(_msg.c_str());
	}

	if (_array.size() < 1) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : assert on empty stack";
		throw Exception(_msg.c_str());
	}

	if (operand->getType() != _array[_array.size() - 1]->getType()) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : assert error __different_type__";
		throw Exception(_msg.c_str());
	}

	if ((operand->getType() < 3 && std::stoi( operand->toString() ) !=  std::stoi( _array[_array.size() - 1]->toString() ) )
		|| (operand->getType() > 2 && std::stod( operand->toString() ) !=  std::stod( _array[_array.size() - 1]->toString() )) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : assert error __different_val__";
		throw Exception(_msg.c_str());
	}

	_line = regex_replace(_line, std::regex("^\\)"), "");
}

void			VM::mathOp(eCommandType cmdType) {

	if (_array.size() < 2) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : no enought arguments in stack for ' " + _line + "' command";
		throw Exception(_msg.c_str());
	}
	const IOperand* operand1 = _array[_array.size() - 1];
	const IOperand* operand2 = _array[_array.size() - 2];

	_array.pop_back();
	_array.pop_back();
	switch (cmdType) {
		case add:
		_array.push_back(_creator.createOperand( *operand1 + *operand2 ) );
		break;
	case sub:
		_array.push_back(_creator.createOperand( *operand1 - *operand2 ) );
		break;
	case mul:
		_array.push_back(_creator.createOperand( *operand1 * *operand2 ) );
		break;
	case Div:
	    if (stof(_array[_array.size() - 2]->toString()) == 0)
            throw Exception("div by 0");
		_array.push_back(_creator.createOperand( *operand1 / *operand2 ) );
		break;
	case mod:
        if (stof(_array[_array.size() - 2]->toString()) == 0)
            throw Exception("mod by 0");
		_array.push_back(_creator.createOperand( *operand1 % *operand2 ) );
		break;
	default:
	    _msg = "Error [ line " + std::to_string(_iter) + " ] : Unknown command'" + _line + "'";
	    throw Exception(_msg.c_str());
	}
}

void			VM::parseLine() {
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
		system("rm -rf temp_file");
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

void		VM::setLine(std::string line) {
	_line = line;
}

void		VM::setIter(int i) {
	_iter = i;
}

std::string	VM::getLine() {
	return (_line);
}

