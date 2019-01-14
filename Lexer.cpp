#include "Lexer.hpp"

void		Lexer::checkLine( void ) {
	eCommandType cmdType = getCommand();

	if (cmdType == push || cmdType == assert) {
		getType();
		if (std::regex_match(_line, _argMatch)) {
			_line = regex_replace(_line, _argReplace, "");
		}
		else {
			_msg = "Error [ line " + std::to_string(_iter) + " ] : Wrong argument";
			throw Exception(_msg.c_str());
		}
	}
	if (_line != "" && !std::regex_match(_line, std::regex("\\s*((?=;).*|\\s*)")) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Invalid symbols after command";
		throw Exception(_msg.c_str());
	}
}
