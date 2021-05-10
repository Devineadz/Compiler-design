#include "EST.h"
#include "Visitor.h"
#include "Prog_node.h"




// rightmost sibling
void EST::makeSiblings(EST* y)
{
	if (this->right_sibling == NULL) {
		this->right_sibling = y;
		if (this->parent != NULL)
			y->parent = this->parent;
	}
	else {
		EST* placeholder = this;
		EST* right_siblingholder = this->right_sibling;
		while (right_siblingholder->right_sibling != NULL) {
			right_siblingholder = right_siblingholder->right_sibling;
			placeholder = placeholder->right_sibling;
		}
		placeholder->right_sibling->right_sibling = y;
	}
	if (y->leftmost_sibling == NULL) {
		y->leftmost_sibling = this;
	}

}

void EST::adoptChildren(EST* y)
{

	if (this->left_child != NULL) {
		if (y->leftmost_sibling == NULL)
			this->left_child = y;
		else
			this->left_child = y->leftmost_sibling;
	}
	y->parent = this;
}

void EST::makeFamily(EST* y)
{
	if (!this->left_child) {
		if (y->leftmost_sibling != NULL) {
			this->left_child = y->leftmost_sibling;
		}
		else
			this->left_child = y;
	}
	else {
		y->right_sibling = this->left_child;
		this->left_child = y;
	}

	y->parent = this;
}

vector<EST*> EST::getChildren()
{
	vector<EST*>children;
	if (this->left_child != NULL) {
		EST* nextChild = this->left_child;
		children.push_back(nextChild);
		while (nextChild->right_sibling != NULL) {
			nextChild = nextChild->right_sibling;
			children.push_back(nextChild);
		}
	}
	return children;
}

void EST::accept(Visitor* visitor)
{
	visitor->visit(this);
}



EST* ESTmaker::makeNode(string node_type)
{
	if (node_type == "class_decl") {
		return new Class_decl;
	}
	if (node_type == "prog") {
		return new Prog_node;
	}
	if (node_type == "func_def") {
		return new Func_def;
	}
	if (node_type == "func_body") {
		return new Func_body;
	}
	if (node_type == "methodbodyvar") {
		return new MethodBody;
	}
	if (node_type == "statementlist") {
		return new StatementList;
	}
	if (node_type == "inherit") {
		return new Inherit;
	}
	if (node_type == "classdeclbody") {
		return new Classdeclbody;
	}
	if (node_type == "classdecl") {
		return new Classdecl;
	}
	if (node_type == "nestedid") {
		return new NestedID;
	}
	if (node_type == "memberdecl") {
		return new Memberdecl;
	}
	if (node_type == "visibility") {
		return new Visibility;
	}
	if (node_type == "funcdecl") {
		return new Funcdecl;
	}
	if (node_type == "vardecl") {
		return new Vardecl;
	}
	if (node_type == "fparams") {
		return new Fparams;
	}
	if (node_type == "funcdecltail") {
		return new Funcdecltail;
	}
	if (node_type == "fparamstail") {
		return new Fparamstail;
	}
	if (node_type == "arraysizerept") {
		return new Arraysizerept;
	}
	if (node_type == "type") {
		return new Type;
	}
	if (node_type == "intnum") {
		return new Intnum;
	}
	if (node_type == "classlist") {
		return new Classlist;
	}
	if (node_type == "void") {
		return new Void;
	}
	if (node_type == "vardeclrep") {
		return new Vardeclrep;
	}
	if (node_type == "varlist") {
		return new Varlist;
	}
	if (node_type == "statement") {
		return new Statement;
	}
	if (node_type == "funcorassignstat") {
		return new Funcorassignstat;
	}
	if (node_type == "funcorassignstatidnest") {
		return new FuncOrAssignStatIdnest;
	}
	if (node_type == "indicerep") {
		return new Indicerep;
	}
	if (node_type == "funcorassignstatsdnestvartail") {
		return new FuncOrAssignStatIdnestVarTail;
	}
	if (node_type == "assignstattail") {
		return new Assignstattail;
	}
	if (node_type == "assignop") {
		return new Assignop;
	}
	if (node_type == "expr") {
		return new Expr;
	}
	if (node_type == "arithexpr") {
		return new Arithexpr;
	}
	if (node_type == "exprtail") {
		return new Exprtail;
	}
	if (node_type == "term") {
		return new Term;
	}
	if (node_type == "factor") {
		return new Factor;
	}
	if (node_type == "write") {
		return new Write;
	}
	if (node_type == "termtail") {
		return new Termtail;
	}
	if (node_type == "arithexprtail") {
		return new Arithexprtail;
	}
	if (node_type == "funcorvar") {
		return new Funcorvar;
	}
	if (node_type == "funcorvaridnest") {
		return new Funcorvaridnest;
	}
	if (node_type == "addop") {
		return new Addop;
	}
	else
		return NULL;
}

EST* ESTmaker::makeNode() {
	return new EST;
}

EST* ESTmaker::makeNode(string node_type, string node_val)
{
	if (node_type == "id") {
		return new ID(node_val);
	}
	if (node_type == "class") {
		return new Class(node_val);
	}
	if (node_type == "intnum") {
		return new Intnumber(node_val);
	}
	if (node_type == "type_id") {
		return new Type_ID(node_val);
	}
}
