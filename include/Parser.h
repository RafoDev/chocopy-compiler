#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include "ASTree.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum ASTNodeType
{
    Default,
    Program,
    Symbol,
    Literal,
    Id,
    Keyword,
    DataType,
    Error
};

using ASTNode = ASTree::ASTNode<ASTNodeType>;
using ASTNodeWalker = ASTree::ASTNodeWalker<ASTNodeType>;
typedef ASTNode::SharedPtr AST;

class Parser
{
private:
    bool panicMode;
    int errorCounter;
    MToken currentToken;
    Scanner scanner;
    bool debug;
    bool debugConsume;
    MessageGenerator msgGen;
    vector<string> errors;

    bool matchType(string);
    bool matchValue(string);
    void consume();
    void Program();
    AST DefList();
    AST Def();
    AST TypedVarList();
    AST TypedVarListTail();
    AST TypedVar();
    AST Type();
    AST Return();
    AST Block();
    AST StatementList();
    AST Statement();
    AST ElifList();
    AST Elif();
    AST Else();
    AST SimpleStatement();
    AST SSTail();
    AST ReturnExpr();
    AST Expr();
    AST ExprPrime();
    AST orExpr();
    AST orExprPrime();
    AST andExpr();
    AST andExprPrime();
    AST notExpr();
    AST notExprPrime();
    AST CompExpr();
    AST CompExprPrime();
    AST IntExpr();
    AST IntExprPrime();
    AST Term();
    AST TermPrime();
    AST Factor();
    AST Name();
    AST NameTail();
    AST Literal();
    AST List();
    AST ExprList();
    AST ExprListTail();
    AST CompOp();
    void printErrors();

    template <typename Func>
    void panic(Func matchFunction)
    {
        matchFunction();
        if (panicMode)
        {
            panicMode = false;
            matchValue("NEWLINE");
        }
    }

public:
    Parser(string filename, bool _debugConsume = false)
    {
        panicMode = false;
        debugConsume = _debugConsume;
        debug = false;
        scanner.init(filename, debugConsume);
        scanner.scan();
        currentToken = scanner.getToken();
        errorCounter = 0;
    }
    void parse();
};
#endif