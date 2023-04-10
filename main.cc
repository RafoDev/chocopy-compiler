#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <stack>
#include <map>
#include <iterator>
#include <vector>
#include <queue>

using namespace std;

char currChar = ' ';

struct Tokens
{
    string identifiersRegExp;
    vector<string> operators;
    map<string, string> delimiters;
    Tokens()
    {
        operators = {
            "+", "-", "*", "%",
            "<", ">", "<=", ">=", "==", "=", ":"};
        // delimiters["("] = "OPEN_PAR"
    }
};
// regex identifiersReg("[a-z|A-Z][a-z|A-Z|0-9|_]*");
// vector<string> operators = ;
// map<string, string> delimiters;
// delimiters["("] = "OPEN_PAR";
// delimiters[")"] = "CLOSE_PAR";

/*
Token categories
    Line structure
    Identifiers
    Keywords
    Literals
    Operators
    Delimiters
*/

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
    currChar = getChar(line, pointer);

    while ((pointer < endLine) && (int(currChar) == 32 || int(currChar) == 9))
    {
        currChar = getChar(line, pointer);
        indent++;
    }

    return indent;
}

// string parseIdentifier(string &line, int &pointer, int endLine)
// {
//     string identifier = "";
//     char currChar = peekChar(line, pointer);
//     string currString(1, currChar);
//     while ((pointer < endLine) && !(int(currChar) == 32 || int(currChar) == 9) && (regex_match(currString, identifiersReg) || currChar == '_'))
//     {
//         // cout << regex_match(currString, identifiersReg) << " " << currString;
//         identifier += currChar;
//         // cout << currChar << ' ' << int(currChar) << "\n";
//         pointer++;
//         currChar = peekChar(line, pointer);
//         currString.assign(currChar, currChar + 1);
//     }
//     return identifier;
// }

void parse(string filename)
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

            // while (pointer < endLine)
            // {
            //     string identifier = parseIdentifier(line, pointer, endLine);
            //     // if (identifier.length())
            //     showMessage("DEBUG", "ID", identifier, lineNumber, pointer);

            //     char currChar = peekChar(line, pointer);
            //     // cout << currChar << ' ' << int(currChar) << "\n";
            //     pointer++;
            // }
            cout << "------------NEWLINE----------------\n";
            // showMessage("DEBUG", "NEWLINE", "", lineNumber, pointer);
            lineNumber++;
        }
    }

    file.close();
}

int main(int argc, char *argv[])
{
    parse(argv[1]);
}