#include "../include/MessageGenerator.h"

void MessageGenerator::buildMsg(MessageType type = MessageType::DEBUG_SCANNER, string tokenType = "", string tokenValue = "", int lineNumber = 0, int pointer = 0)
{
	msg = "";
	switch (type)
	{
	case DEBUG_PARSER:
		msg += format("> [+] ", Color::GREEN, false, false) + "Debug: ";
		break;
	case DEBUG_SCANNER:
		msg += format("> [+] ", Color::GREEN, false, false) + "Debug: ";
		break;
	case INFO:
		msg += format("> [!] ", Color::CYAN, false, false) + "Info: ";
		break;
	case LEXICAL_ERROR:
		// msg += "Lexical Error: ";
		msg += format("> [-] ", Color::RED, false, false) + "Lexical Error: ";
		break;
	case SYNTAX_ERROR:
		// msg += "Syntax Error: ";
		msg += format("> [-] ", Color::RED, false, false) + "Syntax Error: ";
		break;
	}

	if (type == MessageType::DEBUG_PARSER)
	{
		msg += tokenValue + " on line " + to_string(lineNumber) + ".";
		// msg = format(msg, Color::BRIGHT_WHITE, false, false);
	}
	else if (type == MessageType::DEBUG_SCANNER)
	{
		msg += "[" + tokenType + "|" + tokenValue + "] on line " + to_string(lineNumber) + ", column " + to_string(pointer) + ".";
		// msg = format(msg, Color::BRIGHT_WHITE, false, false);
	}
	else if (type == MessageType::INFO)
	{
		msg += tokenValue;
		// msg = format(msg, Color::CYAN, true, false);
	}
	else
	{
		if (type == MessageType::SYNTAX_ERROR)
		{
			msg += tokenValue + " on line " + to_string(lineNumber) + ".";
		}
		else
		{
			msg += tokenValue + " on line " + to_string(lineNumber) + ", column " + to_string(pointer) + ".";
		}
		// msg = format(msg, Color::RED, false, false);
	}

	msg += "\n";
}
string MessageGenerator::getMessage()
{
	return msg;
}
void MessageGenerator::printMessage()
{
	cout << msg;
}

string MessageGenerator::format(const std::string message, Color color, bool bold, bool underline)
{
	string formatCode;

	if (bold)
	{
		formatCode += "\033[1m";
	}
	if (underline)
	{
		formatCode += "\033[4m";
	}

	switch (color)
	{
	case Color::DEFAULT:
		formatCode += "\033[0m";
		break;
	case Color::BLACK:
		formatCode += "\033[30m";
		break;
	case Color::RED:
		formatCode += "\033[31m";
		break;
	case Color::GREEN:
		formatCode += "\033[32m";
		break;
	case Color::YELLOW:
		formatCode += "\033[33m";
		break;
	case Color::BLUE:
		formatCode += "\033[34m";
		break;
	case Color::MAGENTA:
		formatCode += "\033[35m";
		break;
	case Color::CYAN:
		formatCode += "\033[36m";
		break;
	case Color::WHITE:
		formatCode += "\033[37m";
		break;
	case Color::BRIGHT_BLACK:
		formatCode += "\033[90m";
		break;
	case Color::BRIGHT_RED:
		formatCode += "\033[91m";
		break;
	case Color::BRIGHT_GREEN:
		formatCode += "\033[92m";
		break;
	case Color::BRIGHT_YELLOW:
		formatCode += "\033[93m";
		break;
	case Color::BRIGHT_BLUE:
		formatCode += "\033[94m";
		break;
	case Color::BRIGHT_MAGENTA:
		formatCode += "\033[95m";
		break;
	case Color::BRIGHT_CYAN:
		formatCode += "\033[96m";
		break;
	case Color::BRIGHT_WHITE:
		formatCode += "\033[97m";
		break;
	default:
		formatCode += "\033[0m";
		break;
	}
	// cout << formatCode << message << "\033[0m" << '\n';
	return formatCode + message + "\033[0m";
}

// int main() {
//     // Ejemplos de diferentes tipos de impresión
//     printFormatted("Mensaje normal", Color::DEFAULT, false, false);
//     printFormatted("Mensaje en negrita", Color::DEFAULT, true, false);
//     printFormatted("Mensaje subrayado", Color::DEFAULT, false, true);
//     printFormatted("Mensaje en negrita y subrayado", Color::DEFAULT, true, true);

//     printFormatted("Mensaje en color rojo", Color::RED, false, false);
//     printFormatted("Mensaje en color verde", Color::GREEN, false, false);
//     printFormatted("Mensaje en color azul", Color::BLUE, false, false);

//     printFormatted("Mensaje en color amarillo y negrita", Color::YELLOW, true, false);
//     printFormatted("Mensaje en color magenta y subrayado", Color::MAGENTA, false, true);
//     printFormatted("Mensaje en color cian, negrita y subrayado", Color::CYAN, true, true);

//     return 0;
// }