#include "../include/Parser.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void printAsciiArt(const std::string &filename)
{
	std::ifstream file(filename);
	if (!file)
	{
		std::cout << "No se pudo abrir el archivo: " << filename << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
	}

	file.close();
}

int main(int argc, char *argv[])
{
	cout << '\n';
	std::string filename = "data/chocopy.txt";
	printAsciiArt(filename);
	cout << '\n';
	Parser parser(argv[1], true);
	parser.parse();

	return 0;
}