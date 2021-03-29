#include "EST.h"




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
