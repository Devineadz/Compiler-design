#include "EST.h"




// rightmost sibling
void EST::makeSiblings(EST* y)
{
	if (this->right_sibling == NULL) {
		this->right_sibling = y;
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
	if (this->left_child == NULL) {
		if (y->leftmost_sibling == NULL) {
			this->left_child = y;
		}
		else
			this->left_child = y->leftmost_sibling;	
	}
	y->parent = this;
}


EST* ESTmaker::makeNode(string node_type)
{
	if (node_type == "class_decl") {
		return new Class_decl;
	}
	if (node_type == "prog") {
		return new Prog;
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
}
