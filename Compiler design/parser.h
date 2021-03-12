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
private:
	string* nextToken(); // array
	string token;
	string token_path;
	lexer* tokenizer = new lexer(token_path);
	string* lookahead;
	vector<string>firsts;
	vector<string>follows;
	bool skipErrors(vector<string>firsts, vector<string>follows);
	fstream errorFile;
	string errorName;
	void initializeErrorfile(string fileName);


	bool parse();
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
	bool arithExprTail();
	bool factor();
	bool funcOrVar();
	bool sign();
	bool funcOrVarIdnest();
	bool funcOrVarIdnestTail();
	bool addOp();
	bool relOp();

};
