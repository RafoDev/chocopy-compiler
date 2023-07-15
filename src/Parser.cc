
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
    if (currentToken.type == "LITSTR")
    {

        string subLit = currentToken.value.substr(1, currentToken.value.size() - 2);
        currentToken.value = subLit;
    }
}
void Parser::Program()
{
    auto syntaxTree = ASTNode::GetNewInstance(ASTNodeType::Default, "Program");
    if (debug)
        cout << "(OPEN) Program\n";

    syntaxTree->addChild(DefList());
    syntaxTree->addChild(StatementList());

    if (debug)
        cout << "(CLOSE) Program\n";

    if (errorCounter == 0)
    {
        ASTNodeWalker walker(syntaxTree);
        walker.buildDotFormat();
        // std::cout << walker.getDotFormat() << std::endl;
        std::ofstream{"dot_format.txt", std::ios::trunc | std::ios::out} << walker.getDotFormat();
        system("dot -O -Tpng dot_format.txt");
    }
}
AST Parser::DefList()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) DefList\n";

    if (currentToken.value == "def")
    {
        localTree->addChild(Def());
        localTree->addChild(DefList());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) DefList\n";

    return localTree;
}

AST Parser::Def()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "def");
    if (debug)
        cout << "(OPEN) Def\n";

    matchValue("def");

    auto IDLeave = ASTNode::GetNewInstance(ASTNodeType::Id, currentToken.value);
    localTree->addChild(IDLeave);

    matchType("ID");

    if (!panicMode)
    {
        matchValue("(");
        if (!panicMode)
        {
            localTree->addChild(TypedVarList());
            if (!panicMode)
            {
                matchValue(")");
                if (!panicMode)
                {
                    localTree->addChild(Return());
                    if (!panicMode)
                    {
                        matchValue(":");
                    }
                    panicMode = false;
                }
            }
        }
        else
        {
            panicMode = false;
        }
        localTree->addChild(Block());
    }
    else
    {
        panicMode = false;
        matchValue("NEWLINE");
    }
    if (debug)
        cout << "(CLOSE) Def\n";
    return localTree;
}

AST Parser::TypedVarList()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");
    if (debug)
        cout << "(OPEN) TypedVarList\n";

    if (currentToken.type == "ID")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "args");
        localTree->addChild(TypedVar());
        localTree->addChild(TypedVarListTail());
    }
    if (!panicMode)
    {
        if (currentToken.value != ")")
        {
            localTree = ASTNode::GetNewInstance(ASTNodeType::Error, "Error");
            msgGen.buildMsg(MessageType::SYNTAX_ERROR, currentToken.type, "Expected a valid id, returned '" + currentToken.value + "'", currentToken.line, 0);
            if (debugConsume)
                msgGen.printMessage();
            errors.push_back(msgGen.getMessage());
            errorCounter++;

            while (currentToken.value != "NEWLINE")
                currentToken = scanner.getToken();

            panicMode = true;
        }
    }
    else
    {
    }
    if (debug)
        cout << "(CLOSE) TypedVarList\n";
    return localTree;
}

AST Parser::TypedVarListTail()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");
    if (debug)
        cout << "(OPEN) TypedVarListTail\n";

    if (currentToken.value == ",")
    {
        matchValue(",");
        localTree->addChild(TypedVar());
        localTree->addChild(TypedVarListTail());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) TypedVarListTail\n";
    return localTree;
}

AST Parser::TypedVar()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, ":");
    if (debug)
        cout << "(OPEN) TypedVar\n";

    localTree->addChild(ASTNode::GetNewInstance(ASTNodeType::Id, currentToken.value));

    matchType("ID");

    matchValue(":");

    if (!panicMode)
    {
        localTree->addChild(Type());
    }
    if (debug)
        cout << "(CLOSE) TypedVar\n";
    return localTree;
}

AST Parser::Type()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");
    if (debug)
        cout << "(OPEN) Type\n";

    if (currentToken.value == "int" || currentToken.value == "str")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::DataType, currentToken.value);
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

        panicMode = true;
        // currentToken = scanner.getToken();

        // error("Error de sintaxis. Se esperaba un tipo válido.");
    }
    if (debug)
        cout << "(CLOSE) Type\n";
    return localTree;
}

AST Parser::Return()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");
    if (debug)
        cout << "(OPEN) Return\n";

    if (currentToken.value != ":")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, "->");
        matchValue("->");
        if (!panicMode)
            localTree->addChild(Type());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) Return\n";
    return localTree;
}

AST Parser::Block()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) Block\n";

    matchType("NEWLINE");
    matchType("INDENT");

    localTree->addChild(Statement());
    localTree->addChild(StatementList());

    matchType("DEDENT");
    if (debug)
        cout << "(CLOSE) Block\n";
    return localTree;
}

AST Parser::StatementList()
{
    // ", if, while, for, pass, return, -, (, ID, None, True, False, INTEGER, STRING, [
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

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
        localTree->addChild(Statement());
        localTree->addChild(StatementList());
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
    return localTree;
}

AST Parser::Statement()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");
    if (debug)
        cout << "(OPEN) Statement\n";

    if (currentToken.value == "pass")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "pass");
        matchValue("pass");
    }
    else if (currentToken.value == "return")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "return");
        matchValue("return");
        localTree->addChild(ReturnExpr());
    }
    else if (currentToken.value == "if")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "if");
        matchValue("if");
        localTree->addChild(Expr());
        matchValue(":");
        localTree->addChild(Block());
        localTree->addChild(ElifList());
        localTree->addChild(Else());
    }
    else if (currentToken.value == "while")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "while");
        matchValue("while");
        localTree->addChild(Expr());
        matchValue(":");
        localTree->addChild(Block());
    }
    else if (currentToken.value == "for")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "for");
        matchValue("for");
        localTree->addChild(ASTNode::GetNewInstance(ASTNodeType::Id, currentToken.value));
        matchType("ID");
        matchValue("in");
        localTree->addChild(Expr());
        matchValue(":");
        localTree->addChild(Block());
    }
    else
    {
        localTree->addChild(SimpleStatement());
        matchType("NEWLINE");
    }
    if (debug)
        cout << "(CLOSE) Statement\n";
    return localTree;
}

AST Parser::ElifList()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) ElifList\n";

    if (currentToken.value == "elif")
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "elif");
        localTree->addChild(Elif());
        localTree->addChild(ElifList());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) ElifList\n";
    return localTree;
}

AST Parser::Elif()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) Elif\n";

    localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "elif");

    matchValue("elif");
    localTree->addChild(Expr());
    matchValue(":");
    localTree->addChild(Block());
    if (debug)
        cout << "(CLOSE) Elif\n";
    return localTree;
}

AST Parser::Else()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) Else\n";

    if (currentToken.value == "else")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "else");
        matchValue("else");
        matchValue(":");
        localTree->addChild(Block());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) Else\n";
    return localTree;
}

AST Parser::SimpleStatement()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) SimpleStatement\n";
    if ((currentToken.value == "(") || (currentToken.value == "[") ||
        (currentToken.value == "pass") || (currentToken.value == "return") ||
        (currentToken.value == "-") || (currentToken.value == "None") ||
        (currentToken.value == "True") || (currentToken.value == "False"))
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, currentToken.value);
        localTree->addChild(Expr());
        auto SSTailNode = SSTail();
        SSTailNode->addChild(localTree);
        return SSTailNode;
    }

    if ((currentToken.type == "ID") || (currentToken.type == "LITNUM") || (currentToken.type == "LITSTR"))
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, currentToken.type);
        localTree->addChild(Expr());
        auto SSTailNode = SSTail();
        SSTailNode->addChild(localTree);
        return SSTailNode;
    }
    if (debug)
        cout << "(CLOSE) SimpleStatement\n";
    return localTree;
}

AST Parser::SSTail()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) SSTail\n";

    if (currentToken.value == "=")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, "=");

        matchValue("=");
        localTree->addChild(Expr());
    }
    if (debug)
        cout << "(CLOSE) SSTail\n";
    return localTree;
}

AST Parser::ReturnExpr()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");
    if (debug)
        cout << "(OPEN) ReturnExpr\n";

    if (
        (currentToken.value == "(") || (currentToken.value == "False") ||
        (currentToken.value == "-") || (currentToken.value == "None") ||
        (currentToken.value == "True"))
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Default, currentToken.value);

        localTree->addChild(Expr());
    }
    if ((currentToken.type == "ID") || (currentToken.type == "LITNUM") || (currentToken.type == "LITSTR"))
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Default, currentToken.value);
        localTree->addChild(Expr());
    }
    if (debug)
        cout << "(CLOSE) ReturnExpr\n";
    return localTree;
}

AST Parser::Expr()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");
    if (debug)
        cout << "(OPEN) Expr\n";

    localTree->addChild(orExpr());
    localTree->addChild(ExprPrime());

    if (debug)
        cout << "(CLOSE) Expr\n";
    return localTree;
}

AST Parser::ExprPrime()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) ExprPrime\n";

    if (currentToken.value == "if")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "if");
        matchValue("if");
        localTree->addChild(andExpr());
        matchValue("else");
        localTree->addChild(andExpr());
        localTree->addChild(ExprPrime());
    }
    if (debug)
        cout << "(CLOSE) ExprPrime\n";
    return localTree;
}

AST Parser::orExpr()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) orExpr\n";

    localTree->addChild(andExpr());
    localTree->addChild(orExprPrime());

    if (debug)
        cout << "(CLOSE) orExpr\n";
    return localTree;
}

AST Parser::orExprPrime()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) orExprPrime\n";

    if (currentToken.value == "or")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "or");

        matchValue("or");
        localTree->addChild(andExpr());
        localTree->addChild(orExprPrime());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) orExprPrime\n";
    return localTree;
}

AST Parser::andExpr()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) andExpr\n";

    localTree->addChild(notExpr());
    localTree->addChild(andExprPrime());

    if (debug)
        cout << "(CLOSE) andExpr\n";
    return localTree;
}

AST Parser::andExprPrime()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) andExprPrime\n";

    if (currentToken.value == "and")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "and");

        matchValue("and");
        localTree->addChild(notExpr());
        localTree->addChild(andExprPrime());
    }
    if (debug)
        cout << "(CLOSE) andExprPrime\n";
    return localTree;
}

AST Parser::notExpr()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) notExpr\n";

    if (currentToken.value == "not")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "not");

        matchValue("not");
    }
    localTree->addChild(CompExpr());
    localTree->addChild(notExprPrime());

    if (debug)
        cout << "(CLOSE) notExpr\n";
    return localTree;
}

AST Parser::notExprPrime()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) notExprPrime\n";

    // Regla epsilon
    if (currentToken.value == "not")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, "not");

        matchValue("not");
        localTree->addChild(CompExpr());
        localTree->addChild(notExprPrime());
    }
    if (debug)
        cout << "(CLOSE) notExprPrime\n";
    return localTree;
}

AST Parser::CompExpr()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) CompExpr\n";

    auto IntExprNode = IntExpr();
    auto CompExprPrimeNode = CompExprPrime();

    CompExprPrimeNode->addChild(IntExprNode);
    localTree = CompExprPrimeNode;

    if (debug)
        cout << "(CLOSE) CompExpr\n";
    return localTree;
}

AST Parser::CompExprPrime()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) CompExprPrime\n";

    // Regla epsilon
    if (currentToken.value == "==" || currentToken.value == "!=" ||
        currentToken.value == "<" || currentToken.value == ">" ||
        currentToken.value == "<=" || currentToken.value == ">=" ||
        currentToken.value == "is")
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);
        localTree = CompOp();
        localTree->addChild(IntExpr());
        localTree->addChild(CompExprPrime());
    }
    if (debug)
        cout << "(CLOSE) CompExprPrime\n";
    return localTree;
}

AST Parser::IntExpr()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) IntExpr\n";

    auto TermNode = Term();
    auto IntExprPrimeNode = IntExprPrime();

    IntExprPrimeNode->addChild(TermNode);
    localTree = IntExprPrimeNode;

    if (debug)
        cout << "(CLOSE) IntExpr\n";
    return localTree;
}
AST Parser::IntExprPrime()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) IntExprPrime\n";

    if (currentToken.value == "+")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);

        matchValue("+");
        localTree->addChild(Term());
        localTree->addChild(IntExprPrime());
    }
    else if (currentToken.value == "-")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);

        matchValue("-");
        localTree->addChild(Term());
        localTree->addChild(IntExprPrime());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) IntExprPrime\n";
    return localTree;
}

AST Parser::Term()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) Term\n";
    auto FactorNode = Factor();
    auto TermPrimeNode = TermPrime();

    TermPrimeNode->addChild(FactorNode);
    localTree = TermPrimeNode;

    if (debug)
        cout << "(CLOSE) Term\n";
    return localTree;
}

AST Parser::TermPrime()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) TermPrime\n";

    if (currentToken.value == "*")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);

        matchValue("*");
        localTree->addChild(Factor());
        localTree->addChild(TermPrime());
    }
    else if (currentToken.value == "//")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);

        matchValue("//");
        localTree->addChild(Factor());
        localTree->addChild(TermPrime());
    }
    else if (currentToken.value == "%")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);

        matchValue("%");
        localTree->addChild(Factor());
        localTree->addChild(TermPrime());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) TermPrime\n";
    return localTree;
}

AST Parser::Factor()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)

        cout << "(OPEN) Factor\n";

    if (currentToken.value == "-")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);

        matchValue("-");
        localTree->addChild(Factor());
    }
    else if (currentToken.type == "ID")
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Id, currentToken.value);
        localTree->addChild(Name());
    }
    else if (currentToken.value == "None" || currentToken.value == "True" || currentToken.value == "False")
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, currentToken.value);
        localTree->addChild(Literal());
    }
    else if (currentToken.type == "LITNUM" || currentToken.type == "LITSTR")
    {
        // localTree = ASTNode::GetNewInstance(ASTNodeType::DataType, currentToken.type);
        localTree->addChild(Literal());
    }
    else if (currentToken.value == "[")
    {
        localTree->addChild(List());
    }
    else if (currentToken.value == "(")
    {
        matchValue("(");
        localTree->addChild(Expr());
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
    return localTree;
}

AST Parser::Name()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) Name\n";

    localTree = ASTNode::GetNewInstance(ASTNodeType::Id, currentToken.value);
    matchType("ID");
    localTree->addChild(NameTail());

    if (debug)
        cout << "(CLOSE) Name\n";
    return localTree;
}

AST Parser::NameTail()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) NameTail\n";

    if (currentToken.value == "(")
    {
        matchValue("(");
        localTree->addChild(ExprList());
        matchValue(")");
    }
    else if (currentToken.value == "[")
    {
        localTree->addChild(List());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) NameTail\n";
    return localTree;
}

AST Parser::Literal()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) Literal\n";

    if (currentToken.value == "None" || currentToken.value == "True" || currentToken.value == "False")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, currentToken.value);
        consume();
    }
    else if (currentToken.type == "LITNUM" || currentToken.type == "LITSTR")
    {

        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, currentToken.value);
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
    return localTree;
}

AST Parser::List()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) List\n";

    matchValue("[");
    localTree->addChild(ExprList());
    matchValue("]");
    if (debug)
        cout << "(CLOSE) List\n";
    return localTree;
}

AST Parser::ExprList()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) ExprList\n";

    // if (currentToken.value != "]")
    if (
        (currentToken.value == "(") || (currentToken.value == "False") ||
        (currentToken.value == "-") || (currentToken.value == "None") ||
        (currentToken.value == "True"))
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Keyword, currentToken.value);
        localTree->addChild(Expr());
        localTree->addChild(ExprListTail());
    }
    else if ((currentToken.type == "ID") || (currentToken.type == "LITNUM") || (currentToken.type == "LITSTR"))
    {

        // localTree = ASTNode::GetNewInstance(ASTNodeType::DataType, currentToken.value);
        localTree->addChild(Expr());
        localTree->addChild(ExprListTail());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) ExprList\n";
    return localTree;
}

AST Parser::ExprListTail()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) ExprListTail\n";

    if (currentToken.value == ",")
    {
        matchValue(",");
        localTree->addChild(Expr());
        localTree->addChild(ExprListTail());
    }
    else
    {
        // Regla epsilon
    }
    if (debug)
        cout << "(CLOSE) ExprListTail\n";
    return localTree;
}

AST Parser::CompOp()
{
    auto localTree = ASTNode::GetNewInstance(ASTNodeType::Default, "[empty]");

    if (debug)
        cout << "(OPEN) CompOp\n";

    if (currentToken.value == "==" || currentToken.value == "!=" || currentToken.value == "<" ||
        currentToken.value == ">" || currentToken.value == "<=" || currentToken.value == ">=" || currentToken.value == "is")
    {
        localTree = ASTNode::GetNewInstance(ASTNodeType::Symbol, currentToken.value);
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
    return localTree;
}

// Función principal para iniciar el análisis sintáctico
void Parser::parse()
{
    if (scanner.hasErrors())
    {
        msgGen.buildMsg(MessageType::INFO, "", "It's impossible to parse, the scanner threw errors :(.", 0, 0);
        msgGen.printMessage();
        return;
    }
    msgGen.buildMsg(MessageType::INFO, "", "Start parsing...", 0, 0);
    msgGen.printMessage();
    // if (debug)
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