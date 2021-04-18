#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <typeinfo>
#include "lexer.h"
#include "EST.h"
#include "Prog_node.h"

using namespace std;

class parser {
public:
	parser(string path);
	~parser();
	EST* parse();
	string token_path;
	stack<EST*> est_stack;
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
	fstream diagramFile;
	string errorName;
	string derivationName;
	string diagramName;
	void replace(string deriv_func, string deriv_repl);
	void writeToDerivation();
	void initializeFile(string fileName);
	ESTmaker* est;
	EST* ast;
	void createFamilyDiagram(EST* parent, int labelNum);
	void createParentDiagram(int labelNum, int child_label);
	void createLabelChildDiagram(int labelNum, EST* child);
	void createTreeDiagram(EST* parent);
	void diagramToFile(string parent, string child);
	void labelToFile(string child, string child_label);
	int labelcounter = 1;
	int idlabelcounter = 100;
	vector<int>inheritCounts;
	vector<int>inheritClassCounts;
	vector<int>classBodyCounts;
	vector<int>visibilityCount;
	vector<int>memberCount;

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

	int class_num = 0;

};
