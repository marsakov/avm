#include <iostream>
#include <fstream>
#include "Operand.hpp"
#include "VM.hpp"


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

	for (int i = 1; ifs.getline(line, 256) && std::string(line) != ";;"; i++)
	{
		if (std::string(line) != "" && !std::regex_match(line, std::regex("\\s*;+.*")) ) {
			vm.setLine(line);
			vm.setIter(i);
			vm.parseLine();
		}
	}

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
	reading_code(file_name);
	system("rm -rf temp_file");
	return (0);
}
