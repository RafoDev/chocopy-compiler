
#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include <fstream>
#include <iterator>
#include <vector>
#include <queue>
#include "MToken.h"
#include "MessageGenerator.h"
using namespace std;


class Scanner
{
private:
	MessageGenerator msgGen;
	char currChar;
	int errorCounter;
	vector<string> errors;
	vector<string> fileLines;
	string filename;
	queue<MToken> currTokens;
	bool debug;

	// void showMessage(string type, string tokenType, string token, int lineNumber, int pointer);
	char getChar(string line, int &pointer);
	char peekChar(string line, int pointer);
	int computeIndent(string line, int &pointer, int endLine);
	MToken scanOperator(MToken token, string &line, int &pointer, int endLine);
	MToken scanDelimiter(MToken token, string &line, int &pointer, int endLine);
	string getValidTokenValue(MToken token, string &line, int &pointer, int endLine);
	MToken getInvalidToken(MToken token, string &line, int &pointer, int endLine);
	MToken scanLiteralString(MToken token, string &line, int &pointer, int endLine);
	MToken scanLiteralNumber(MToken token, string &line, int &pointer, int endLine);
	MToken getTokenUtil(string line, int &pointer, int endLine);
	void storeLines(string filename);
	bool scanEspacesTabs(string line);
	void printErrors();

public:
	Scanner(){};
	Scanner(string _filename) { init(_filename); };
	void init(string _filename, bool _debug = false)
	{
		debug = _debug;
		currChar = ' ';
		errorCounter = 0;
		filename = _filename;
		storeLines(_filename);
	}
	void scan();
	MToken getToken();
};

#endif