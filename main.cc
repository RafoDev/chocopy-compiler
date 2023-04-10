#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>
#include <stack>
#include <map>
#include <iterator>
#include <vector>
#include <queue>

using namespace std;

char currChar = ' ';

vector<string> keywords = {
		"False", "None", "True", "and", "as", "assert", "async", "await", "break", "class", "continue", "def", "del", "elif", "else", "except", "finally", "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try", "while", "with", "yield"};
vector<string> operators = {
		"+", "-", "*", "%", "/", "<",
		">", "<=", ">=", "=="};
vector<string> delimiters = {
		")",
		"(",
		"[",
		"]",
		",",
		".",
		";",
		"=",
		":",
		"->",
		"=>",
};
vector<string> comments = {"//", "#"};

regex idRegexp("[a-z|A-Z][a-z|A-Z|0-9|_]*");

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

	while (
			(pointer < endLine) &&
			!(int(currChar) == 32 || int(currChar) == 9) &&
			(find(operators.begin(), operators.end(), charStr) == operators.end()) &&
			(find(delimiters.begin(), delimiters.end(), charStr) == delimiters.end()))
	{

		tokenValue += currChar;
		currChar = getChar(line, pointer);
		charStr.assign(1, currChar);
	}

	token.value = tokenValue;
	token.type = "ANY";
	if (regex_match(tokenValue, idRegexp))
		token.type = "ID";
	else
		token.type = "KEY";

	// while ((pointer < endLine) && !(int(currChar) == 32 || int(currChar) == 9) && (regex_match(currString, identifiersReg) || currChar == '_'))
	// {
	// 	// cout << regex_match(currString, identifiersReg) << " " << currString;
	// 	identifier += currChar;
	// 	// cout << currChar << ' ' << int(currChar) << "\n";
	// 	pointer++;
	// 	currChar = peekChar(line, pointer);
	// 	currString.assign(currChar, currChar + 1);
	// }
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
				MToken currToken = getToken(line, pointer, endLine);
				// if (identifier.length())
				showMessage("DEBUG", currToken.type, currToken.value, lineNumber, pointer);

				// char currChar = peekChar(line, pointer);
				// cout << currChar << ' ' << int(currChar) << "\n";

				currChar = getChar(line, pointer);
			}
			cout << "------------NEWLINE----------------\n";
			// showMessage("DEBUG", "NEWLINE", "", lineNumber, pointer);
			lineNumber++;
		}
	}

	file.close();
}

int main(int argc, char *argv[])
{
	scan(argv[1]);
}