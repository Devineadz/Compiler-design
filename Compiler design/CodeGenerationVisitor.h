#pragma once

// Code generation based on tags

#include <iostream>
#include <string>
#include <fstream>
#include <stack>

#include "Visitor.h"
#include "EST.h"
#include "Prog_node.h"

using namespace std;

class CodeGenerationVisitor : public Visitor {
	public:
		CodeGenerationVisitor(string filename);
		stack<string> registerPool;
		int tempvarnum;
		string moonExecCode;
		string moonDataCode;
		string moonCodeIndent = "           ";
		string moonCodeIndent2 = "          ";
		fstream outputfile;
		string outputfileName;
		void initializeFile(string filename);

		void visit(EST* node);
		void visitP(EST* node);
		void visitFB(EST* node);
		void visitMB(EST* node);
		void visitVL(EST* node);
		void visitV(EST* node);
		void visitSL(EST* node);
		void visitST(EST* node);
		void visitAP(EST* node);
		void visitEX(EST* node);
		void visitAE(EST* node);
		void visitWR(EST* node);
		void visitAD(EST* node);

		
	//	void visit(Classdecl* node) {};
	//	void visit(Func_def* node) {};
	//	void visit(Func_body* node) {};
	//	void visit(MethodBody* node) {};
	//	void visit(StatementList* node) {};
	//	void visit(Vardecl* node);
	//	void visit(Varlist* node) {};
	//	void visit(ID* node) {};
	//	void visit(Statement* node) {};
	//	void visit(Assignop* node) {};
	//	void visit(Expr* node) {};
	//	void visit(Intnum* node) {};
	//	void visit(Arithexpr* node) {};
	//	void visit(Write* node) {};
};