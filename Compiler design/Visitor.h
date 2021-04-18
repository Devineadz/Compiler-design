#pragma once
#include "EST.h"
#include "Prog_node.h"

class Visitor {
public:
	virtual void visit(EST* node) =0;
//	virtual void visit(Class_decl* node) {};
//	virtual void visit(Func_def* node) {};
//	virtual void visit(Func_body* node) {};
//	virtual void visit(MethodBody* node) {};
//	virtual void visit(StatementList* node) {};
//	virtual void visit(Vardecl* node) {};
//	virtual void visit(Varlist* node) {};
//	virtual void visit(ID* node) {};
//	virtual void visit(Statement* node) {};
//	virtual void visit(Assignop* node) {};
//	virtual void visit(Expr* node) {};
//	virtual void visit(Intnumber* node) {};
//	virtual void visit(Arithexpr* node) {};
//	virtual void visit(Write* node) {};
};