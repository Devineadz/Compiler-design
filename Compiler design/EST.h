#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

class Visitor;
class Prog_node;


using namespace std;

// AST class, EST is parent class, all other nodes are child classes
class EST {
	
public: 
	EST *parent = NULL;
	EST* right_sibling = NULL;
	EST* leftmost_sibling = NULL;
	EST* left_child = NULL;
	string type = "Node";

	string getType() const { return type; }
	void makeSiblings(EST* y);
	void adoptChildren(EST* y);
	void makeFamily(EST* y);
	vector<EST*> getChildren();
	virtual void accept(Visitor* visit);
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
	void accept(Visitor &visitor) {
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
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
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
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}

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
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}

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
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
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
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
};

class Statement : public EST {
public:
	Statement() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "STATEMENT";
	}
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
};

class Funcorassignstat : public EST {
public:
	Funcorassignstat() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNCORASSIGNSTAT";
	}

}; 

class FuncOrAssignStatIdnest : public EST {
public:
	FuncOrAssignStatIdnest() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNCORASSIGNSTATIDNEST";
	}

};

class Indicerep : public EST {
public:
	Indicerep() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "INDICEREP";
	}

};

class Assignstattail : public EST {
public:
	Assignstattail() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "ASSIGNSTATTAIL";
	}

};

class Assignop : public EST {
public:
	Assignop() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "ASSIGNOP";
	}
//	void accept(Visitor* visitor) {
	//	visitor->visit(this);
//	}
};

class Addop : public EST {
public:
	Addop() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "ADDOP";
	}
	//	void accept(Visitor* visitor) {
		//	visitor->visit(this);
	//	}
};

class Expr : public EST {
public:
	Expr() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "EXPR";
	}
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
};

class Exprtail : public EST {
public:
	Exprtail() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "EXPRTAIL";
	}

};

class Term : public EST {
public:
	Term() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "TERM";
	}

};

class Arithexprtail : public EST {
public:
	Arithexprtail() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "ARITHEXPRTAIL";
	}

};

class Termtail : public EST {
public:
	Termtail() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "TERMTAIL";
	}

};

class Factor : public EST {
public:
	Factor() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FACTOR";
	}

};

class Write : public EST {
public:
	Write() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "WRITE";
	}
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
};

class Funcorvar : public EST {
public:
	Funcorvar() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNCORVAR";
	}

};

class Funcorvaridnest : public EST {
public:
	Funcorvaridnest() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNCORVARIDNEST";
	}

};

class Arithexpr : public EST {
public:
	Arithexpr() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "ARITHEXPR";
	}
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}

};

class FuncOrAssignStatIdnestVarTail : public EST {
public:
	FuncOrAssignStatIdnestVarTail() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "FUNCORASSIGNSTATIDNESTVARTAIL";
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
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}

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

class Void : public EST {
public:
	Void() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "VOID";
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

class Vardeclrep : public EST {
public:
	Vardeclrep() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "VARDECLREP";
	}

};

class Varlist : public EST {
public:
	Varlist() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "VARLIST";
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

	string getID() const {  return id; }

//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
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
//	void accept(Visitor* visitor) {
//		visitor->visit(this);
//	}
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