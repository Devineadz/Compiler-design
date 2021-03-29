#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>


using namespace std;

// AST class, EST is parent class, all other nodes are child classes
class EST {
	
public: 
	EST* parent;
	EST* right_sibling;
	EST* leftmost_sibling;
	EST* left_child;
	string type;

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

class Visibility : public EST {
public:
	Visibility() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "VISIBILITY";
	}

};

class Memberdecl: public EST {
public:
	Memberdecl() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "MEMBERDECL";
	}

};

class Funcdecl : public EST {
public:
	Funcdecl() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "Funcdecl";
	}

};

class Vardecl : public EST {
public:
	Vardecl() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "VARDECL";
	}

};

class Fparams : public EST {
public:
	Fparams() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FPARAMS";
	}

};

class Fparamstail : public EST {
public:
	Fparamstail() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FPARAMSTAIL";
	}

};

class Arraysizerept : public EST {
public:
	Arraysizerept() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "ARRAYSIZEREPT";
	}

};

class Type : public EST {
public:
	Type() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "TYPE";
	}

};

class Intnum : public EST {
public:
	Intnum() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "INTNUM";
	}

};

class Funcdecltail : public EST {
public:
	Funcdecltail() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNCDECLTAIL";
	}

};

class Classlist : public EST {
public:
	Classlist() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "CLASSLIST";
	}

};

class NestedID : public EST {
public:
	NestedID() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "NESTEDID";
	}

};

class ID :public EST {
public:
	ID(string id_name) {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = id_name;
		id = "ID";
	}
	string id;
};

class Intnumber :public EST {
public:
	Intnumber(string id_name) {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = id_name;
		id = "Intnum";
	}
	string id;
};

class Class :public EST {
public:
	Class(string id_name) {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = id_name;
		id = "CLASS";
	}
	string id;
};

class Type_ID :public EST {
public:
	Type_ID(string id_name) {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = id_name;
		id = "TYPE_ID";
	}
	string id;
};


class Empty_nody : public EST {

};

// Factory that makes the nodes
class ESTmaker {
public:
	static EST* makeNode();
	static EST* makeNode(string node_type);
	static EST* makeNode(string node_type, string node_val);
};