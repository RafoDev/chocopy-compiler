#include "../include/Parser.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

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

namespace fs = std::filesystem;

vector<string> testFiles;
void getTestFiles()
{
	string folderPath = "tests"; // Reemplaza con la ruta de la carpeta deseada

	for (const auto &entry : fs::directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			testFiles.push_back(entry.path().filename());
			// std::cout << entry.path().filename() << std::endl;
		}
	}
}

int main(int argc, char *argv[])
{
	bool quit = false;
	getTestFiles();

	while (!quit)
	{
		cout << '\n';
		std::string filename = "data/chocopy.txt";
		printAsciiArt(filename);
		cout << '\n';

		int testNumber = testFiles.size();
		int option = 0;

		cout << "Escribe el número del archivo que deseas analizar:\n";
		int i = 0;
		for (; i < testNumber; i++)
		{
			cout << " └─ [" << i << "] " << testFiles[i] << '\n';
		}
		int all, quit;
		all = i;
		cout << " └─ [" << i << "] "
				 << "Todos\n";
		i++;
		quit = i;
		cout << " └─ [" << i << "] "
				 << "Salir\n";

		cout << "Tu opción: ";
		cin >> option;

		if (option < all)
		{
			Parser parser("tests/" + testFiles[option], true);
			parser.parse();
		}
		else if (option == all)
		{
			for (int j = 0; j < all; j++)
			{
				cout << "\n> " << testFiles[j] << '\n';
				Parser parser("tests/" + testFiles[j], true);
				parser.parse();
			}
			cout << '\n';
		}
		else
		{
			return 0;
		}
		char tmp;
		cout << "Presiona cualquier tecla para continuar...\n";
		cin>>tmp;
	}
	return 0;
}