#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>


using namespace std;

class EST {
protected:
	EST* parent;
	EST* right_sibling;
	EST* leftmost_sibling;
	EST* left_child;
	string type;
public: 

	string getType() const { return type; }
	void makeSiblings(EST* y);
	void adoptChildren(EST* y);
	void makeFamily(EST* y);

};

class Prog : public EST {
public:
	Prog(){
		parent =NULL ;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "PROG";
	}
};

class Class_decl : public EST {
public:
	Class_decl() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "CLASS_DECL";
	}

};

class Func_def : public EST {
public:
	Func_def() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNC DEF";
	}


};

class Func_body : public EST {
public:
	Func_body() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNC BODY";
	}


};

class MethodBody : public EST {
public:
	MethodBody() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "METHOD BODY";
	}

};

class StatementList : public EST {
public:
	StatementList() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "STATEMENTLIST";
	}

};

class Inherit : public EST {
public:
	Inherit() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "INHERIT";
	}

};

class Classdecl : public EST {
public:
	Classdecl() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "CLASSDECL";
	}

};

class Classdeclbody : public EST {
public:
	Classdeclbody() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "CLASSDECLBODY";
	}

};

class ID :public EST {
public:
	ID(string id_name) {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "CLASSDECLBODY";
		id = id_name;
	}
	string id;
};

class Empty_nody : public EST {

};

class ESTmaker {
public:
	static EST* makeNode();
	static EST* makeNode(string node_type);
	static EST* makeNode(string node_type, string node_val);
};