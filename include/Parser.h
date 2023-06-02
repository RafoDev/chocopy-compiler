#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

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

    bool matchType(string expectedType);
    bool matchValue(string expectedValue);
    void consume();
    void Program();
    void DefList();
    void Def();
    void TypedVarList();
    void TypedVarListTail();
    void TypedVar();
    void Type();
    void Return();
    void Block();
    void StatementList();
    void Statement();
    void ElifList();
    void Elif();
    void Else();
    void SimpleStatement();
    void SSTail();
    void ReturnExpr();
    void Expr();
    void ExprPrime();
    void orExpr();
    void orExprPrime();
    void andExpr();
    void andExprPrime();
    void notExpr();
    void notExprPrime();
    void CompExpr();
    void CompExprPrime();
    void IntExpr();
    void IntExprPrime();
    void Term();
    void TermPrime();
    void Factor();
    void Name();
    void NameTail();
    void Literal();
    void List();
    void ExprList();
    void ExprListTail();
    void CompOp();
    void printErrors();

public:
    Parser(string filename, bool _debugConsume = false)
    {
        panicMode=false;
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