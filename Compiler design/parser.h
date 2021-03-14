#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "lexer.h"

using namespace std;

class parser {
public:
	parser(string path);
	~parser();
	bool parse();
	string token_path;
private:
	void nextToken(); // array
	bool eof;
	string token;
	lexer* tokenizer;
	string lookahead[3];
	vector<string>firsts;
	vector<string>follows;
	string derivation;
	bool skipErrors(vector<string>firsts, vector<string>follows);
	fstream derivationFile;
	fstream errorFile;
	string errorName;
	string derivationName;
	void replace(string deriv_func, string deriv_repl);
	void writeToDerivation();
	void initializeFile(string fileName);


	bool start();
	bool prog();
	bool classdecl();
	bool funcDef();
	bool match(string token);
	bool funcbody();
	bool inherit();
	bool nestedId();
	bool classdeclbody();
	bool visibility();
	bool memberDecl();
	bool funcDecl();
	bool varDecl();
	bool type();
	bool arraySizeRept();
	bool intNum();
	bool fParams();
	bool funcDeclTail();
	bool fParamsTail();
	bool function();
	bool funcHead();
	bool funcBody();
	bool classMethod();
	bool methodBodyVar();
	bool statementList();
	bool varDeclRep();
	bool statement(); // continue here funcDef -> funcBody -> statmenetlist -> statement
	bool funcOrAssignStat();
	bool expr();
	bool statBlock();
	bool variable();
	bool variableIdnest();
	bool variableIdNestTail();
	bool funcOrAssignStatIdnest();
	bool indiceRep();
	bool funcOrAssignStatIdnestVarTail();
	bool aParams();
	bool aParamsTail();
	bool funcOrAssignStatIdnestFuncTail();
	bool funcStatTail();
	bool funcStatTailIdnest();
	bool arithExpr();
	bool exprTail();
	bool term();
	bool termTail();
	bool arithExprTail();
	bool factor();
	bool funcOrVar();
	bool sign();
	bool funcOrVarIdnest();
	bool funcOrVarIdnestTail();
	bool addOp();
	bool relOp();
	bool assignStatTail();
	bool assignOp();
	bool multOp();

};
