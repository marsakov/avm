#include <iostream>
#include <fstream>
#include "Operand.hpp"
#include "Parser.hpp"


void	create_file() {
	char line[256];
	std::ofstream ofs("temp_file");

	std::cin.getline(line, 256);
	while (strncmp(line, ";;", strlen(line)) || std::string(line) == "") {
		ofs << line << std::endl;
		std::cin.getline(line, 256);
	}
	ofs << line << std::endl;
}

void	reading_code(std::string file_name) {
	char line[256];
	std::ifstream ifs(file_name);
	Parser parser;

	for (int i = 0; ifs.getline(line, 256) && std::string(line) != ";;"; i++)
	{
		if (std::string(line) != "" && !std::regex_match(line, std::regex("\\s*;+.*")) ) {
			parser.setLine(line);
			parser.setIter(i);
			parser.parseLine();
		}
	}

}

int		main(int ac, char *av[]) {
	std::string						file_name = "";
	std::vector<const IOperand*>	array;
	Creator							creator;


	const IOperand* one = creator.createOperand("1", Int8);
	array.push_back(one);

	// std::regex rgx("^(push|assert)\\s*(int(8|16|32)|float|double)\\(-?[0-9]+.?[0-9]*\\)\\s*((?=;).*|\\s*)");

	// std::string source = "push    int8(-6.0) ;hgh";

	// if (std::regex_match(source, rgx)) {
	//     std::cout << source << std::endl;
	// }

	if (ac == 1) {
		create_file();
		file_name = "temp_file";
	}
	else if (ac == 2)
	    file_name = std::string(av[1]);
	else {
		std::cout << "Usage : ./avm [file_name] OR ./avm\n[YOUR CODE]" << std::endl;
		exit(1);
	}
	reading_code(file_name);
}
