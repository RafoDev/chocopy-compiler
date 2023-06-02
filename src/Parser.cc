
#include "../include/Parser.h"

bool Parser::matchType(string expectedType)
{
    if (currentToken.type == expectedType)
    {
        consume();
        return true;
    }
    else
    {
        msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Expected Type '" + expectedType + "'", currentToken.line, 0);
        if (debugConsume)
            msgGen.printMessage();
        errors.push_back(msgGen.getMessage());
        errorCounter++;

        while (currentToken.value != "NEWLINE")
            currentToken = scanner.getToken();

        panicMode = true;
        return false;
        // error("Error de sintaxis. Se esperaba: " + expectedType);
    }
}

bool Parser::matchValue(string expectedValue)
{
    if (currentToken.value == expectedValue)
    {
        consume();
        return true;
    }
    else
    {
        msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Expected value '" + expectedValue + "'", currentToken.line, 0);
        if (debugConsume)
            msgGen.printMessage();
        errors.push_back(msgGen.getMessage());
        errorCounter++;
        // currentToken = scanner.getToken();

        while (currentToken.value != "NEWLINE")
            currentToken = scanner.getToken();
        panicMode = true;

        // error("Error de sintaxis. Se esperaba: " + expectedValue);
        return false;
    }
}
void Parser::consume()
{
    msgGen.buildMsg(MessageType::DEBUG_PARSER, currentToken.type, "Consumed token '" + currentToken.value + "'", currentToken.line, 0);
    if (debugConsume)
        msgGen.printMessage();

    // cout << "Consuming : " << currentToken.value << '\n';
    currentToken = scanner.getToken();
}
void Parser::Program()
{
    if (debug)
        cout << "(OPEN) Program\n";
    DefList();
    StatementList();
    if (debug)
        cout << "(CLOSE) Program\n";
}
void Parser::DefList()
{
    if (debug)
        cout << "(OPEN) DefList\n";

    if (currentToken.value == "def")
    {
        Def();
        DefList();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) DefList\n";
}

void Parser::Def()
{
    if (debug)
        cout << "(OPEN) Def\n";

    matchValue("def");
    matchType("ID");
    matchValue("(");
    TypedVarList();
    matchValue(")");
    if (!panicMode)
    {
        Return();
        matchValue(":");
        panicMode = false;
    }
    Block();
    if (debug)
        cout << "(CLOSE) Def\n";
}

void Parser::TypedVarList()
{
    if (debug)
        cout << "(OPEN) TypedVarList\n";

    if (currentToken.type == "ID")
    {
        TypedVar();
        TypedVarListTail();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) TypedVarList\n";
}

void Parser::TypedVarListTail()
{
    if (debug)
        cout << "(OPEN) TypedVarListTail\n";

    if (currentToken.value == ",")
    {
        matchValue(",");
        TypedVar();
        TypedVarListTail();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) TypedVarListTail\n";
}

void Parser::TypedVar()
{
    if (debug)
        cout << "(OPEN) TypedVar\n";

    matchType("ID");
    matchValue(":");

    Type();
    if (debug)
        cout << "(CLOSE) TypedVar\n";
}

void Parser::Type()
{
    if (debug)
        cout << "(OPEN) Type\n";

    if (currentToken.value == "int" || currentToken.value == "str")
    {
        consume();
    }
    else if (currentToken.value == "[")
    {
        matchValue("[");
        Type();
        matchValue("]");
    }
    else
    {
        msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Expected a valid type, returned '" + currentToken.value + "'", currentToken.line, 0);
        if (debugConsume)
            msgGen.printMessage();
        errors.push_back(msgGen.getMessage());
        errorCounter++;

        while (currentToken.value != "NEWLINE")
            currentToken = scanner.getToken();
        // currentToken = scanner.getToken();

        // error("Error de sintaxis. Se esperaba un tipo válido.");
    }
    if (debug)
        cout << "(CLOSE) Type\n";
}

void Parser::Return()
{
    if (debug)
        cout << "(OPEN) Return\n";

    if (currentToken.value == "->")
    {
        matchValue("->");
        Type();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) Return\n";
}

void Parser::Block()
{
    if (debug)
        cout << "(OPEN) Block\n";

    matchType("NEWLINE");
    matchType("INDENT");
    Statement();
    StatementList();
    matchType("DEDENT");
    if (debug)
        cout << "(CLOSE) Block\n";
}

void Parser::StatementList()
{
    // ", if, while, for, pass, return, -, (, ID, None, True, False, INTEGER, STRING, [

    if (debug)
        cout << "(OPEN) StatementList\n";

    if (
        (currentToken.value == "[") || (currentToken.type == "ID") ||
        (currentToken.value == "if") || (currentToken.value == "pass") ||
        (currentToken.value == "while") || (currentToken.value == "return") ||
        (currentToken.value == "for") || (currentToken.value == "-") ||
        (currentToken.value == "None") || (currentToken.value == "True") ||
        (currentToken.value == "False") || (currentToken.type == "LITNUM") ||
        (currentToken.type == "LITSTR") || (currentToken.value == "("))
    {
        Statement();
        StatementList();
    }
    else if (currentToken.type == "NEWLINE")
    {
        consume();
    }
    // else if (currentToken.type == "DEDENT")
    // {
    // 	consume();
    // }
    if (debug)
        cout << "(CLOSE) StatementList\n";
}

void Parser::Statement()
{
    if (debug)
        cout << "(OPEN) Statement\n";

    if (currentToken.value == "pass")
    {
        matchValue("pass");
    }
    else if (currentToken.value == "return")
    {
        matchValue("return");
        ReturnExpr();
    }
    else if (currentToken.value == "if")
    {
        matchValue("if");
        Expr();
        matchValue(":");
        Block();
        ElifList();
        Else();
    }
    else if (currentToken.value == "while")
    {
        matchValue("while");
        Expr();
        matchValue(":");
        Block();
    }
    else if (currentToken.value == "for")
    {
        matchValue("for");
        matchType("ID");
        matchValue("in");
        Expr();
        matchValue(":");
        Block();
    }
    else
    {
        SimpleStatement();
        matchType("NEWLINE");
    }
    if (debug)
        cout << "(CLOSE) Statement\n";
}

void Parser::ElifList()
{
    if (debug)
        cout << "(OPEN) ElifList\n";

    if (currentToken.value == "elif")
    {
        Elif();
        ElifList();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) ElifList\n";
}

void Parser::Elif()
{
    if (debug)
        cout << "(OPEN) Elif\n";

    matchValue("elif");
    Expr();
    matchValue(":");
    Block();
    if (debug)
        cout << "(CLOSE) Elif\n";
}

void Parser::Else()
{
    if (debug)
        cout << "(OPEN) Else\n";

    if (currentToken.value == "else")
    {
        matchValue("else");
        matchValue(":");
        Block();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) Else\n";
}

void Parser::SimpleStatement()
{
    if (debug)
        cout << "(OPEN) SimpleStatement\n";
    if (
        (currentToken.value == "(") || (currentToken.type == "ID") ||
        (currentToken.value == "pass") || (currentToken.value == "return") ||
        (currentToken.value == "-") || (currentToken.value == "None") ||
        (currentToken.value == "True") || (currentToken.value == "False") ||
        (currentToken.type == "LITNUM") || (currentToken.type == "LITSTR") ||
        (currentToken.value == "["))
    {
        Expr();
        SSTail();
    }
    if (debug)
        cout << "(CLOSE) SimpleStatement\n";
}

void Parser::SSTail()
{
    if (debug)
        cout << "(OPEN) SSTail\n";

    if (currentToken.value == "=")
    {
        matchValue("=");
        Expr();
    }
    if (debug)
        cout << "(CLOSE) SSTail\n";
}

void Parser::ReturnExpr()
{
    if (debug)
        cout << "(OPEN) ReturnExpr\n";

    if (
        (currentToken.value == "(") || (currentToken.type == "ID") ||
        (currentToken.value == "-") || (currentToken.value == "None") ||
        (currentToken.value == "True") || (currentToken.value == "False") ||
        (currentToken.type == "LITNUM") || (currentToken.type == "LITSTR"))
    {
        Expr();
    }
    if (debug)
        cout << "(CLOSE) ReturnExpr\n";
}

void Parser::Expr()
{
    if (debug)
        cout << "(OPEN) Expr\n";

    orExpr();
    ExprPrime();
    if (debug)
        cout << "(CLOSE) Expr\n";
}

void Parser::ExprPrime()
{
    if (debug)
        cout << "(OPEN) ExprPrime\n";

    if (currentToken.value == "if")
    {
        matchValue("if");
        andExpr();
        matchValue("else");
        andExpr();
        ExprPrime();
    }
    if (debug)
        cout << "(CLOSE) ExprPrime\n";
}

void Parser::orExpr()
{
    if (debug)
        cout << "(OPEN) orExpr\n";

    andExpr();
    orExprPrime();
    if (debug)
        cout << "(CLOSE) orExpr\n";
}

void Parser::orExprPrime()
{
    if (debug)
        cout << "(OPEN) orExprPrime\n";

    if (currentToken.value == "or")
    {
        matchValue("or");
        andExpr();
        orExprPrime();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) orExprPrime\n";
}

void Parser::andExpr()
{
    if (debug)
        cout << "(OPEN) andExpr\n";

    notExpr();
    andExprPrime();
    if (debug)
        cout << "(CLOSE) andExpr\n";
}

void Parser::andExprPrime()
{
    if (debug)
        cout << "(OPEN) andExprPrime\n";

    if (currentToken.value == "and")
    {
        matchValue("and");
        notExpr();
        andExprPrime();
    }
    if (debug)
        cout << "(CLOSE) andExprPrime\n";
}

void Parser::notExpr()
{
    if (debug)
        cout << "(OPEN) notExpr\n";

    if (currentToken.value == "not")
    {
        matchValue("not");
    }
    CompExpr();
    notExprPrime();
    if (debug)
        cout << "(CLOSE) notExpr\n";
}

void Parser::notExprPrime()
{
    if (debug)
        cout << "(OPEN) notExprPrime\n";

    // Regla epsilon
    if (currentToken.value == "not")
    {
        matchValue("not");
        CompExpr();
        notExprPrime();
    }
    if (debug)
        cout << "(CLOSE) notExprPrime\n";
}

void Parser::CompExpr()
{
    if (debug)
        cout << "(OPEN) CompExpr\n";

    IntExpr();
    CompExprPrime();
    if (debug)
        cout << "(CLOSE) CompExpr\n";
}

void Parser::CompExprPrime()
{
    if (debug)
        cout << "(OPEN) CompExprPrime\n";

    // Regla epsilon
    if (currentToken.value == "==" || currentToken.value == "!=" ||
        currentToken.value == "<" || currentToken.value == ">" ||
        currentToken.value == "<=" || currentToken.value == ">=" ||
        currentToken.value == "is")
    {
        CompOp();
        IntExpr();
        CompExprPrime();
    }
    if (debug)
        cout << "(CLOSE) CompExprPrime\n";
}

void Parser::IntExpr()
{
    if (debug)
        cout << "(OPEN) IntExpr\n";

    Term();
    IntExprPrime();
    if (debug)
        cout << "(CLOSE) IntExpr\n";
}
void Parser::IntExprPrime()
{
    if (debug)
        cout << "(OPEN) IntExprPrime\n";

    if (currentToken.value == "+")
    {
        matchValue("+");
        Term();
        IntExprPrime();
    }
    else if (currentToken.value == "-")
    {
        matchValue("-");
        Term();
        IntExprPrime();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) IntExprPrime\n";
}

void Parser::Term()
{
    if (debug)
        cout << "(OPEN) Term\n";

    Factor();
    TermPrime();
    if (debug)
        cout << "(CLOSE) Term\n";
}

void Parser::TermPrime()
{
    if (debug)
        cout << "(OPEN) TermPrime\n";

    if (currentToken.value == "*")
    {
        matchValue("*");
        Factor();
        TermPrime();
    }
    else if (currentToken.value == "//")
    {
        matchValue("//");
        Factor();
        TermPrime();
    }
    else if (currentToken.value == "%")
    {
        matchValue("%");
        Factor();
        TermPrime();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) TermPrime\n";
}

void Parser::Factor()
{
    if (debug)
        cout << "(OPEN) Factor\n";

    if (currentToken.value == "-")
    {
        matchValue("-");
        Factor();
    }
    else if (currentToken.type == "ID")
    {
        Name();
    }
    else if (currentToken.value == "None" || currentToken.value == "True" || currentToken.value == "False" ||
             currentToken.type == "LITNUM" || currentToken.type == "LITSTR")
    {
        Literal();
    }
    else if (currentToken.value == "[")
    {
        List();
    }
    else if (currentToken.value == "(")
    {
        matchValue("(");
        Expr();
        matchValue(")");
    }
    else
    {
        msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Expected a valid factor, returned '" + currentToken.value + "'", currentToken.line, 0);
        if (debugConsume)
            msgGen.printMessage();
        errors.push_back(msgGen.getMessage());
        errorCounter++;

        while (currentToken.value != "NEWLINE")
            currentToken = scanner.getToken();
        // currentToken = scanner.getToken();

        // error("Error de sintaxis. Se esperaba un factor válido.");
    }
    if (debug)
        cout << "(CLOSE) Factor\n";
}

void Parser::Name()
{
    if (debug)
        cout << "(OPEN) Name\n";

    matchType("ID");
    NameTail();
    if (debug)
        cout << "(CLOSE) Name\n";
}

void Parser::NameTail()
{
    if (debug)
        cout << "(OPEN) NameTail\n";

    if (currentToken.value == "(")
    {
        matchValue("(");
        ExprList();
        matchValue(")");
    }
    else if (currentToken.value == "[")
    {
        List();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) NameTail\n";
}

void Parser::Literal()
{
    if (debug)
        cout << "(OPEN) Literal\n";

    if (currentToken.value == "None" || currentToken.value == "True" || currentToken.value == "False" ||
        currentToken.type == "LITNUM" || currentToken.type == "LITSTR")
    {
        consume();
    }
    else
    {
        msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Expected a valid literal, returned '" + currentToken.value + "'", currentToken.line, 0);
        if (debugConsume)
            msgGen.printMessage();
        errors.push_back(msgGen.getMessage());
        errorCounter++;
        while (currentToken.value != "NEWLINE")
            currentToken = scanner.getToken();
        // currentToken = scanner.getToken();

        // error("Error de sintaxis. Se esperaba un literal válido.");
    }
    if (debug)
        cout << "(CLOSE) Literal\n";
}

void Parser::List()
{
    if (debug)
        cout << "(OPEN) List\n";

    matchValue("[");
    ExprList();
    matchValue("]");
    if (debug)
        cout << "(CLOSE) List\n";
}

void Parser::ExprList()
{
    if (debug)
        cout << "(OPEN) ExprList\n";

    // if (currentToken.value != "]")
    if (
        (currentToken.value == "(") || (currentToken.type == "ID") ||
        (currentToken.value == "-") || (currentToken.value == "None") ||
        (currentToken.value == "True") || (currentToken.value == "False") ||
        (currentToken.type == "LITNUM") || (currentToken.type == "LITSTR"))
    {
        // {
        Expr();
        ExprListTail();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) ExprList\n";
}

void Parser::ExprListTail()
{
    if (debug)
        cout << "(OPEN) ExprListTail\n";

    if (currentToken.value == ",")
    {
        matchValue(",");
        Expr();
        ExprListTail();
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) ExprListTail\n";
}

void Parser::CompOp()
{
    if (debug)
        cout << "(OPEN) CompOp\n";

    if (currentToken.value == "==" || currentToken.value == "!=" || currentToken.value == "<" ||
        currentToken.value == ">" || currentToken.value == "<=" || currentToken.value == ">=" || currentToken.value == "is")
    {
        consume();
    }
    else
    {
        msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Expected a valid operator, returned '" + currentToken.value + "'", currentToken.line, 0);
        if (debugConsume)
            msgGen.printMessage();
        errors.push_back(msgGen.getMessage());
        errorCounter++;
        while (currentToken.value != "NEWLINE")
            currentToken = scanner.getToken();
        // currentToken = scanner.getToken();

        // error("Error de sintaxis. Se esperaba un operador de comparación válido.");
    }
    if (debug)
        cout << "(CLOSE) CompOp\n";
}

// Función principal para iniciar el análisis sintáctico
void Parser::parse()
{
    msgGen.buildMsg(MessageType::INFO, "", "Start parsing...", 0, 0);
    // if (debug)
    msgGen.printMessage();
    Program();

    // Verificar si se han consumido todos los tokens
    if (currentToken.type != "EOF")
    {
        msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Not all tokens were consumed", currentToken.line, 0);
        if (debugConsume)
            msgGen.printMessage();
        errors.push_back(msgGen.getMessage());
        errorCounter++;
        while (currentToken.value != "NEWLINE")
            currentToken = scanner.getToken();
        // currentToken = scanner.getToken();

        // error("Error de sintaxis. No se han consumido todos los tokens.");
    }

    // cout << "Análisis sintáctico exitoso." << endl;
    printErrors();
}
void Parser::printErrors()
{
    msgGen.buildMsg(MessageType::INFO, "", "Parse completed with " + to_string(errorCounter) + " errors.", 0, 0);
    msgGen.printMessage();
    if (!debugConsume)
    {
        for (string error : errors)
            cout << " " << error;
        cout << '\n';
    }
}