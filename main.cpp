#include <iostream>
#include <fstream>
#include "Operand.hpp"
#include "VM.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"

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
	VM vm;
	Parser parser;
	Lexer lexer;

	for (int i = 1; ifs.getline(line, 256) && std::string(line) != ";;"; i++)
	{
		if (std::string(line) != "" && !std::regex_match(line, std::regex("\\s*((?=;).*|\\s*)")) )
			try {
				parser.setLine(line, i);
				parser.checkLine();
				lexer.setLine(line, i);
				lexer.checkLine();
				vm.setLine(line, i);
				vm.checkLine();
			}
			catch (Exception e)
			{
				system("rm -rf temp_file");
				std::cout << e.what() << std::endl;
				exit(1);
			}
	}
	system("rm -rf temp_file");
	if (vm.exitInstr == false)
		throw Exception("Error : Expected exit command");
}

int		main(int ac, char *av[]) {
	std::string		file_name = "";

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
	try {
		reading_code(file_name);
	}
	catch (Exception e)
	{
		system("rm -rf temp_file");
		std::cout << e.what() << std::endl;
		exit(1);
	}
	return (0);
}
