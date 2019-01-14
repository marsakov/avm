#include "Parser.hpp"

void		Parser::checkLine( void ) {
	eCommandType cmdType = getCommand();

	if (cmdType == push || cmdType == assert) {
		eOperandType opType = getType();
		std::cmatch match;

		if (std::regex_match(_line.c_str(), match, _argMatch)) {
			_line = regex_replace(_line, _argReplace, "");
			if ((match[1].matched && opType > 2) || (match[2].matched && opType < 3) ) {
				_msg = "Error [ line " + std::to_string(_iter) + " ] : Wrong argument. __different_type__";
				throw Exception(_msg.c_str());
			}
		}
		else {
			_msg = "Error [ line " + std::to_string(_iter) + " ] : Wrong argument. __bad_format__";
			throw Exception(_msg.c_str());
		}
	}
	if (_line != "" && !std::regex_match(_line, std::regex("\\s*((?=;).*|\\s*)")) ) {
		_msg = "Error [ line " + std::to_string(_iter) + " ] : Invalid symbols after command";
		throw Exception(_msg.c_str());
	}
}
