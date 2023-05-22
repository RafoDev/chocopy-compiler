
#ifndef MESSAGE_GENERATOR_H
#define MESSAGE_GENERATOR_H

#include <iostream>
#include <string>
using namespace std;

enum MessageType
{
	DEBUG_SCANNER,
	DEBUG_PARSER,
	INFO,
	LEXICAL_ERROR,
	SYNTAX_ERROR
};
enum class Color {
    DEFAULT,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE
};

class MessageGenerator
{
private:
	string msg;
	string format(const std::string message, Color color, bool bold, bool underline);
public:
	MessageGenerator(){};
	void buildMsg(MessageType type, string tokenType, string tokenValue, int lineNumber, int pointer);
	string getMessage();
	void printMessage();
};

#endif