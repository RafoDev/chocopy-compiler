#include "../include/Scanner.h"
#include "../include/Symbols.h"

void Scanner::showMessage(string type, string tokenType, string token, int lineNumber, int pointer)
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

char Scanner::getChar(string line, int &pointer)
{
	return line[++pointer];
}

char Scanner::peekChar(string line, int pointer)
{
	return line[pointer + 1];
}

int Scanner::computeIndent(string line, int &pointer, int endLine)
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

MToken Scanner::scanOperator(MToken token, string &line, int &pointer, int endLine)
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
	return token;
}

MToken Scanner::scanDelimiter(MToken token, string &line, int &pointer, int endLine)
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
	return token;
}

string Scanner::getValidTokenValue(MToken token, string &line, int &pointer, int endLine)
{
	string tokenValue = "";
	string charStr(1, currChar);
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
	return tokenValue;
}

MToken Scanner::getInvalidToken(MToken token, string &line, int &pointer, int endLine)
{
	string tokenValue = "";
	string charStr(1, currChar);

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
	return token;
}

MToken Scanner::scanLiteralString(MToken token, string &line, int &pointer, int endLine)
{
	token.type = "LITSTR";
	token.value += '"';
	currChar = getChar(line, pointer);
	bool error = false;
	bool final = false;

	while (!final && (pointer < endLine))
	{
		// Caracter ASCII valido
		if (int(currChar) >= 32 && int(currChar) <= 126)
		{
			token.value += currChar;
			if (currChar == '"')
				final = true;
			else if (int(currChar) == 92)
			{
				// secuencias de escape válidas -> " ' n t r b f v 0
				char nextChar = getChar(line, pointer);
				token.value += currChar + nextChar;
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
		token.value = "Not recognized: " + token.value;
		errorCounter++;
	}
	else if (!final)
	{
		token.type = "LEXICAL ERROR";
		token.value = "Expected close symbol: " + token.value;
		errorCounter++;
	}

	return token;
}

MToken Scanner::scanLiteralNumber(MToken token, string &line, int &pointer, int endLine)
{
	token.type = "LITNUM";
	int max = 2147483647;
	if (token.value[0] == '0' && token.value.size() > 1)
	{
		token.type = "LEXICAL ERROR";
		token.value = "Remove the 0: " + token.value;
		errorCounter++;
	}
	else if (stol(token.value) > max)
	{
		token.type = "LEXICAL ERROR";
		token.value = "Out of range: " + token.value;
		errorCounter++;
	}

	return token;
}

MToken Scanner::getTokenUtil(string line, int &pointer, int endLine)
{

	while (currChar == ' ' || currChar == '	')
		currChar = getChar(line, pointer);

	MToken token;
	string charStr(1, currChar);

	if (currChar == '#')
	{
		token.type = "COMMENT";
		token.value = "";
		return token;
	}

	if (find(operators.begin(), operators.end(), charStr) != operators.end())
		token = scanOperator(token, line, pointer, endLine);
	else if (find(delimiters.begin(), delimiters.end(), charStr) != delimiters.end())
		token = scanDelimiter(token, line, pointer, endLine);
	else
	{
		token.value = getValidTokenValue(token, line, pointer, endLine);

		if (find(keywords.begin(), keywords.end(), token.value) != keywords.end())
			token.type = "KEY";
		else if (regex_match(token.value, idRegexp))
			token.type = "ID";
		else if (currChar == '"')
			token = scanLiteralString(token, line, pointer, endLine);
		else if (regex_match(token.value, numberRegexp))
			token = scanLiteralNumber(token, line, pointer, endLine);
		else
			token = getInvalidToken(token, line, pointer, endLine);
	}
	return token;
}

void Scanner::storeLines(string filename)
{
	fstream file;
	file.open(filename, ios::in);

	if (file.is_open())
	{
		string line;
		while (getline(file, line))
			fileLines.push_back(line);
	}
	file.close();
}

void Scanner::scan()
{
	int lineNumber = 1;
	cout << "INFO SCAN - Start scanning...\n";

	priority_queue<int> indentStack;
	indentStack.push(0);

	for (const string &line : fileLines)
	{
		MToken currToken;
		int pointer = -1;
		int endLine = line.length();
		int currInden = computeIndent(line, pointer, endLine);

		if (currInden > indentStack.top())
		{
			indentStack.push(currInden);
			showMessage("DEBUG", "INDENT", "", lineNumber, pointer / 4);
			currToken.type = "INDENT";
			currToken.line = lineNumber;

			currTokens.push(currToken);
		}
		while (currInden < indentStack.top())
		{
			indentStack.pop();
			showMessage("DEBUG", "DEDENT", "", lineNumber, pointer / 4);
			currToken.type = "DEDENT";
			currToken.line = lineNumber;

			currTokens.push(currToken);
		}

		while (pointer < endLine)
		{
			currToken = getTokenUtil(line, pointer, endLine);

			if (currToken.type == "COMMENT")
			{
				pointer = endLine;
			}
			else
			{
				showMessage("DEBUG", currToken.type, currToken.value, lineNumber, pointer - currToken.value.length() + 1);
				// cout<<"currChar : "<<currChar<<'\n';
				currToken.line = lineNumber;
				currTokens.push(currToken);
			}
		}
		if (currToken.type != "COMMENT")
		{
			showMessage("DEBUG", "NEWLINE", "", lineNumber, pointer + 1);
			currToken.type = "NEWLINE";
			currToken.line = lineNumber;
			currTokens.push(currToken);
		}

		lineNumber++;
	}
	cout << "INFO SCAN - Completed with " << errorCounter << " errors\n";
}

MToken Scanner::getToken()
{
	MToken currToken = currTokens.front();
	currTokens.pop();
	return currToken;
}
