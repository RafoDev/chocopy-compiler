#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>
#include <iterator>
#include <vector>
#include <queue>

using namespace std;

char currChar = ' ';

// todo: add more types
vector<string> keywords = {
	"False", "None", "True", "and", "as", "assert", "async", "await", "bool", "break", "class", "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "global", "if", "import", "in", "is", "int", "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try", "while", "with", "yield"};
vector<string> operators = {
	"+", "-", "*", "%", "/", "<",
	">", "<=", ">=", "==", "!="};
vector<string> delimiters = {
	")",
	"(",
	"[",
	"]",
	",",
	".",
	"=",
	":",
	"->"};

regex idRegexp("[a-z|A-Z][a-z|A-Z|0-9|_]*");
regex numberRegexp("[0-9]+");
regex escapeRegexp("[\"'ntrbfv\\0]");

int errorCounter = 0;

struct MToken
{
	string type;
	string value;
};

void showMessage(string type, string tokenType, string token, int lineNumber, int pointer)
{
	string message = type + " SCAN - ";
	message += tokenType;
	message += " [";
	message += token;
	message += "] found at (";
	message += to_string(lineNumber);
	message += ":";
	message += to_string(pointer);
	message += ")\n";
	cout << message;
}

char getChar(string &line, int &pointer)
{
	return line[++pointer];
}

char peekChar(string &line, int pointer)
{
	return line[pointer + 1];
}

int computeIndent(string &line, int &pointer, int endLine)
{
	int indent = 0;
	currChar = getChar(line, pointer);

	while ((pointer < endLine) && (int(currChar) == 32 || int(currChar) == 9))
	{
		currChar = getChar(line, pointer);
		indent++;
	}

	return indent;
}

MToken getToken(string &line, int &pointer, int endLine)
{
	MToken token;
	string tokenValue = "";
	string charStr(1, currChar);

	if (currChar == '#')
	{
		token.type = "COMMENT";
		token.value = "";
		return token;
	}

	if (find(operators.begin(), operators.end(), charStr) != operators.end())
	{
		token.value += currChar;
		token.type = "OP";
		char nextChar = getChar(line, pointer);

		if (currChar == '-' && nextChar == '>')
		{
			token.value += nextChar;
			token.type = "DEL";
			nextChar = getChar(line, pointer);
			nextChar = getChar(line, pointer);
		}
		else if ((currChar == '>' && nextChar == '=') || (currChar == '<' && nextChar == '='))
		{
			token.value += nextChar;
			nextChar = getChar(line, pointer);
		}
		else
		{
			token.type = "OP";
		}
		currChar = nextChar;
	}
	else if (find(delimiters.begin(), delimiters.end(), charStr) != delimiters.end())
	{
		// = ==
		token.value += currChar;
		char nextChar = getChar(line, pointer);

		if (currChar == '=' && nextChar == '=')
		{
			token.value += nextChar;
			token.type = "OP";
			nextChar = getChar(line, pointer);
		}
		else
		{
			token.type = "DEL";
		}
		currChar = nextChar;
	}
	else
	{
		while (
			(pointer < endLine) &&
			((int(currChar) >= 48 && int(currChar) <= 57) ||
			 (int(currChar) >= 65 && int(currChar) <= 90) ||
			 (int(currChar) >= 97 && int(currChar) <= 122) ||
			 (int(currChar) == 95)) &&

			(find(operators.begin(), operators.end(), charStr) == operators.end()) &&
			(find(delimiters.begin(), delimiters.end(), charStr) == delimiters.end()))
		{
			tokenValue += currChar;
			currChar = getChar(line, pointer);
			charStr.assign(1, currChar);
		}

		token.value = tokenValue;
		if (find(keywords.begin(), keywords.end(), tokenValue) != keywords.end())
			token.type = "KEY";
		else if (regex_match(tokenValue, idRegexp))
			token.type = "ID";

		// Si encuentra comillas (")
		else if (int(currChar) == 34)
		{
			token.type = "LITSTR";
			tokenValue = currChar;
			currChar = getChar(line, pointer);
			bool error = false;
			bool final = false;

			while (!final && (pointer < endLine))
			{
				// Caracter ASCII valido
				if (int(currChar) >= 32 && int(currChar) <= 126)
				{
					tokenValue += currChar;
					if (currChar == '"')
						final = true;
					else if (int(currChar) == 92)
					{
						// secuencias de escape válidas -> " ' n t r b f v 0
						char nextChar = getChar(line, pointer);
						tokenValue += currChar + nextChar;
						if (!(pointer < endLine) || !regex_match(string(1, nextChar), std::regex(escapeRegexp)))
						{
							error = true;
						}
					}
				}

				currChar = getChar(line, pointer);
			}

			// Sin errores y con comilla de cierre (No se hace nada, el token se retorna)
			// Hay una secuencia de escape inválida:
			if (error)
			{
				token.type = "LEXICAL ERROR";
				token.value = "Not recognized: " + tokenValue;
				errorCounter++;
			}
			else if (!final)
			{
				token.type = "LEXICAL ERROR";
				token.value = "Expected close symbol: " + tokenValue;
				errorCounter++;
			}
			else
			{
				token.value = tokenValue;
			}
		}

		else if (regex_match(tokenValue, numberRegexp))
		{
			token.type = "LITNUM";
			if (tokenValue[0] == '0' && tokenValue.size() > 1)
			{
				token.type = "LEXICAL ERROR";
				token.value = "Remove the 0: " + tokenValue;
				errorCounter++;
			}
			int max = 2147483647;
			if (stol(tokenValue) > max)
			{
				token.type = "LEXICAL ERROR";
				token.value = "Out of range: " + tokenValue;
				errorCounter++;
			}
		}
		else
		{
			while (
				(pointer < endLine) &&
				!(int(currChar) == 32 || int(currChar) == 9 || int(currChar) == 34) &&
				!((int(currChar) >= 48 && int(currChar) <= 57) ||
				 (int(currChar) >= 65 && int(currChar) <= 90) ||
				 (int(currChar) >= 97 && int(currChar) <= 122) ||
				 (int(currChar) == 95)))
				{
					tokenValue += currChar;
					currChar = getChar(line, pointer);
					charStr.assign(1, currChar);
				}
			token.type = "LEXICAL ERROR";
			token.value = "Not supported token: " + tokenValue;
		}
	}
	return token;
}

void scan(string filename)
{
	fstream file;
	file.open(filename, ios::in);
	int lineNumber = 1;
	cout << "INFO SCAN - Start scanning...\n";

	if (file.is_open())
	{
		string line;
		priority_queue<int> indentStack;
		indentStack.push(0);

		while (getline(file, line))
		{
			MToken currToken;
			int pointer = -1;
			int endLine = line.length();
			int currInden = computeIndent(line, pointer, endLine);

			if (currInden > indentStack.top())
			{
				indentStack.push(currInden);
				showMessage("DEBUG", "INDENT", "", lineNumber, pointer / 4);
			}
			while (currInden < indentStack.top())
			{
				indentStack.pop();
				showMessage("DEBUG", "DEDENT", "", lineNumber, pointer / 4);
			}

			while (pointer < endLine)
			{
				while (currChar == ' ')
					currChar = getChar(line, pointer);

				currToken = getToken(line, pointer, endLine);

				if (currToken.type == "COMMENT")
				{
					pointer = endLine;
				}
				else
				{
					showMessage("DEBUG", currToken.type, currToken.value, lineNumber, pointer - currToken.value.length() + 1);
					// cout<<"currChar : "<<currChar<<'\n';
				}
			}
			if (currToken.type != "COMMENT")
			{
				showMessage("DEBUG", "NEWLINE", "", lineNumber, pointer + 1);
			}

			lineNumber++;
		}
		cout << "INFO SCAN - Completed with " << errorCounter << " errors\n";
	}

	file.close();
}

int main(int argc, char *argv[])
{
	scan(argv[1]);
}
