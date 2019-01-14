#include "VM.hpp"

VM::VM() {
	_cmdMatch = std::regex("(push\\s+.*)|(pop.*)|(dump.*)|(assert\\s+.*)|(add.*)|(sub.*)|(mul.*)|(div.*)|(mod.*)|(print.*)|(exit.*)|(max.*)|(min.*)|(clear.*)|(sqr.*)|(size.*)");
	_cmdReplace = std::regex("(push)|(pop)|(dump)|(assert)|(add)|(sub)|(mul)|(div)|(mod)|(print)|(exit)|(max)|(min)|(clear)|(sqr)|(size)");

	_typeMatch = std::regex("(int8\\(.*)|(int16\\(.*)|(int32\\(.*)|(float\\(.*)|(double\\(.*)");
	_typeReplace = std::regex("(int8\\()|(int16\\()|(int32\\()|(float\\()|(double\\()");
	
	_argMatch = std::regex("([-]?[0-9]+\\).*)|([-]?[0-9]+\\.[0-9]+\\).*)");
	_argReplace = std::regex("([-]?[0-9]+\\))|([-]?[0-9]+\\.[0-9]+\\))");

	exitInstr = false;
}

VM::~VM() { }

eOperandType 	VM::getType( void ) {
	std::cmatch match;

	_line = std::regex_replace(_line, std::regex("^\\s*"), "");
	if (std::regex_match(_line.c_str(), match, _typeMatch)) {
		for (size_t n = 1; n < match.size(); ++n) {
			if (match[n].matched) {
				_line = regex_replace(_line, _typeReplace, "");
				return (eOperandType(n - 1));
			}
		}
	}
	_msg = "Error [ line " + std::to_string(_iter) + " ] : Wrong argument type. Use {int8, int16, int32, float, double}";
	throw Exception(_msg.c_str());
}


eCommandType 	VM::getCommand( void ) {
	std::cmatch match;

	if (std::regex_match(_line.c_str(), match, _cmdMatch)) {
		for (size_t n = 1; n < match.size(); ++n) {
			if (match[n].matched) {
				_line = regex_replace(_line, _cmdReplace, "");
				return (eCommandType(n - 1));
			}
		}
	}
	_msg = "Error [ line " + std::to_string(_iter) + " ] : Unknown command'" + _line + "'";
	throw Exception(_msg.c_str());
}

void			VM::assertFunc() {
	eOperandType opType;
	const IOperand* operand;

	opType = getType();
	operand = _creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType);
	notEmpty("assert");

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
		_msg = "Error [ line " + std::to_string(_iter) + " ] : no enought arguments in stack for command";
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
		if (stof(operand2->toString()) == 0)
			throw Exception("div by 0");
		_array.push_back(_creator.createOperand( *operand1 / *operand2 ) );
		break;
	case mod:
		if (stof(operand2->toString()) == 0)
			throw Exception("mod by 0");
		_array.push_back(_creator.createOperand( *operand1 % *operand2 ) );
		break;
	default:
		break;
	}
}

void			VM::printFunc() {
	notEmpty("print");
	if (_array[_array.size() - 1]->getType() != Int8) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : print wrong arg __must_be_int8__)";
		throw Exception(_msg.c_str());
	}
	else
		std::cout << static_cast<char>(stoi(_array[_array.size() - 1]->toString())) << std::endl;
}

void			VM::dumpFunc() {
	notEmpty("dump");
	for (int i = 0; i < _array.size(); i++)
		std::cout << _array[i]->toString() << std::endl;
}

void			VM::pushFunc() {
	eOperandType opType;

	opType = getType();
	_array.push_back(_creator.createOperand( regex_replace(_line, std::regex("\\).*"), ""), opType));
}

void			VM::minFunc() {
	const IOperand* operand;

	notEmpty("min");
	operand = _array[0];
	for (int i = 1; i < _array.size(); i++) {
		if (stof(_array[i]->toString()) < stof(operand->toString()))
			operand = _array[i];
	}
	std::cout << operand->toString() << std::endl;
}

void			VM::maxFunc() {
	const IOperand* operand;

	notEmpty("max");
	operand = _array[0];
	for (int i = 1; i < _array.size(); i++) {
		if (stof(_array[i]->toString()) > stof(operand->toString()))
			operand = _array[i];
	}
	std::cout << operand->toString() << std::endl;
}

void			VM::sqrFunc() {
	notEmpty("sqr");
	const IOperand* operand = _array[_array.size() - 1];
	_array.pop_back();
	_array.push_back( _creator.createOperand( (*operand) * (*operand) ) );
}

void			VM::notEmpty(std::string s) {
	if (_array.size() == 0) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : " + s + " on empty stack";
		throw Exception(_msg.c_str());
	}
}

void			VM::checkLine() {
	eCommandType cmdType;

	cmdType = getCommand();
	switch (cmdType) {
		case push:
			pushFunc(); break ;
		case pop: {
			notEmpty("pop");
			_array.pop_back();
			break ;
		}
		case dump:
			dumpFunc(); break ;
		case assert:
			assertFunc(); break ;
		case add:
			mathOp(add); break ;
		case sub:
			mathOp(sub); break ;
		case mul:
			mathOp(mul); break ;
		case Div:
			mathOp(Div); break ;
		case mod:
			mathOp(mod); break ;
		case print:
			printFunc(); break ;
		case Exit: {
			system("rm -rf temp_file");
			exit(0);
		}
		case Min:
			minFunc(); break ;
		case Max:
			maxFunc(); break ;
		case sqr:
			sqrFunc(); break ;
		case size:
			std::cout << _array.size() << std::endl; break ;
		case clear: {
			notEmpty("clear");
			_array.clear();
			break ;
		}
	}
}

void		VM::setLine(std::string line, int i) {
	_line = std::regex_replace(line, std::regex("^\\s*"), "");
	_iter = i;
}


std::string	VM::getLine() {
	return (_line);
}

