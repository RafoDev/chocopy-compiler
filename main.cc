#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <stack>
#include <map>
#include <iterator>
using namespace std;

regex identifiersReg("[a-z|A-Z][a-z|A-Z|0-9|_]*");
// struct tokens{

// };

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

bool RegexMatcher(string str, regex reg)
{
    return regex_match(str, reg);
}

// info: get the char and move the pointer
char getChar(string &line, int &pointer)
{
    return line[++pointer];
}
// info: get the char without moving the pointer
char peekChar(string &line, int pointer)
{
    return line[pointer + 1];
}

int computeIndent(string &line, int &pointer, int endLine)
{
    int indent = 0;
    char currChar = peekChar(line, pointer);

    while ((pointer < endLine) && (int(currChar) == 32 || int(currChar) == 9))
    {
        // cout << currChar << ' ' << int(currChar) << "\n";
        pointer++;
        indent++;
        currChar = peekChar(line, pointer);
    }
    return indent;
}

string parseIdentifier(string &line, int &pointer, int endLine)
{
    string identifier = "";
    char currChar = peekChar(line, pointer);
    string currString(1, currChar);
    while ((pointer < endLine) && !(int(currChar) == 32 || int(currChar) == 9) && (regex_match(currString, identifiersReg) || currChar == '_'))
    {
        // cout << regex_match(currString, identifiersReg) << " " << currString;
        identifier += currChar;
        // cout << currChar << ' ' << int(currChar) << "\n";
        pointer++;
        currChar = peekChar(line, pointer);
        currString.assign(currChar, currChar + 1);
    }
    return identifier;
}

void parse(string filename)
{
    fstream file;
    file.open(filename, ios::in);
    int lineNumber = 1;
    cout << "INFO SCAN - Start scanning...\n";

    if (file.is_open())
    {
        string line;
        stack<int> indentStack;
        indentStack.push(0);

        while (getline(file, line))
        {
            int pointer = -1;
            int endLine = line.length();
            int inden = computeIndent(line, pointer, endLine);
            if (inden != indentStack.top())
            {
                indentStack.push(inden);
                showMessage("DEBUG", "IDENT", "", lineNumber, pointer);
            }
            while (pointer < endLine)
            {
                string identifier = parseIdentifier(line, pointer, endLine);
                if (identifier.length())
                    showMessage("DEBUG", "ID", identifier, lineNumber, pointer);

                char currChar = peekChar(line, pointer);
                // cout << currChar << ' ' << int(currChar) << "\n";
                pointer++;
            }
            cout << "------------NEWLINE----------------\n";
            lineNumber++;
        }
    }

    file.close();
}

int main(int argc, char *argv[])
{
    parse(argv[1]);
}