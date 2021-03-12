#include "parser.h"

parser::parser(string path)
{
	token_path = path;
	tokenizer = new lexer(token_path);
	initializeErrorfile(path);
}

parser::~parser()
{
}

string* parser::nextToken()
{
	static string nextToken[3];
	string* placeHolder = tokenizer->getNextToken();
	nextToken[0] = placeHolder[0];
	nextToken[1] = placeHolder[1];
	nextToken[2] = placeHolder[2];
	return placeHolder;
}

bool parser::skipErrors(vector<string>firsts, vector<string>follows)
{
	for (vector<string>::iterator it = firsts.begin(); it < firsts.end(); ++it)
	{
		if (lookahead[0] == *it) {
			return true;
		}
		else {
			for (vector<string>::iterator it2 = follows.begin(); it2 < follows.end(); ++it) {
				if (*it == "epsilon" && lookahead[0] == *it2)
					return true;
			}
		}
	}
	// write syntax error
	errorFile.open(errorName, ios::app);
	if (errorFile.is_open()) {
		errorFile << "Syntax error for: " << lookahead[0] << " - " << lookahead[1] << " at location: " << lookahead[2] << ".\n";
	}
	errorFile.close();
	bool lookaheadCheck = false;
	while (lookaheadCheck == false) {
		lookahead = nextToken();
		for (vector<string>::iterator it = firsts.begin(); it < firsts.end(); ++it)
		{
			if (lookahead[0] == *it) {
				lookaheadCheck = true;
				return true;
			}
			else {
				for (vector<string>::iterator it2 = follows.begin(); it2 < follows.end(); ++it)
					if (*it == "epsilon" && lookahead[0] == *it2)
						lookaheadCheck = true;
				return false;
			}
		}
	}
	return false;
}

void parser::initializeErrorfile(string fileName)
{
	string newFileName;
	for (int count = 0; fileName[count] != '.'; count++) { // gets the name from the original file name
		newFileName.push_back(fileName[count]);
	}
	errorName = newFileName + ".outsytanxerrors";

	errorFile.open(errorName, ofstream::out | ofstream::trunc); // clear contents from files
	errorFile.close();
}

bool parser::parse()
{
	lookahead = nextToken();
	if (start() & match("$"))
		return true;
	else
		return false;
}

bool parser::start()
{
	firsts.push_back("main");
	firsts.push_back("class");
	firsts.push_back("func");
	follows.push_back("NULL");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();
	if (lookahead[0] == "main" || lookahead[0] == "class" || lookahead[0] == "func") {
		if (prog()) {
			return(true);
		}
	}
	return false;
}

bool parser::prog()
{
	firsts.push_back("main");
	firsts.push_back("class");
	firsts.push_back("func");
	follows.push_back("NULL");//figure out
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "main" || lookahead[0] == "class" || lookahead[0] == "func") {
		if (classdecl() && funcDef() && match("main") && funcbody())
			return true;
	}
	else
		return false;
}

bool parser::classdecl()
{
	firsts.push_back("class");
	follows.push_back("func");
	follows.push_back("main");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "class") {
		if (match("class") && match("id") && inherit() && match("opencubr") && classdeclbody() && match("closecubr") && match("semi") && classdecl()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "func" || lookahead[0] == "main")
		return true;
	else
		return false;
}

bool parser::funcDef()
{
	firsts.push_back("func");
	follows.push_back("main");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "func") {
		if (function() && funcDef())
			return true;
		else
			return false;
	}
	if (lookahead[0] == "main")
		return true;
	else
		return false;
}

bool parser::match(string token)
{
	if (lookahead[0] == token) {
		lookahead = nextToken();
		return true;
	}
	else {
		lookahead = nextToken();
			return false;
	}
}


bool parser::inherit()
{
	firsts.push_back("inherits");
	follows.push_back("opencubr");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "inherits") {
		if (match("inherits") && match("id") && nestedId()) {
			return true;
		}
		else
			return false;
	}
	else 
		return true;
}

bool parser::nestedId()
{
	firsts.push_back("comma");
	follows.push_back("opencubr");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "comma") {
		if (match("comma") && match("id") && nestedId()) {
			return true;
		}
		else
			return false;
	}
	else
		return true;
}

bool parser::classdeclbody()
{
	firsts.push_back("public");
	firsts.push_back("private");
	firsts.push_back("func");
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("id");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "public" || lookahead[0] == "private" || lookahead[0] == "func" || lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id" || lookahead[0] == "closecubr") {
		if (visibility() && memberDecl() && classdeclbody())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closecubr")
		return true;
	else
		return false;
}

bool parser::visibility()
{
	firsts.push_back("public");
	firsts.push_back("private");
	follows.push_back("func");
	follows.push_back("integer");
	follows.push_back("float");
	follows.push_back("string");
	follows.push_back("id");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "public") {
		if (match("public"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "private") {
		if (match("private"))
			return true;
		else
			return false;
	}
	else
		return true;
}

bool parser::memberDecl()
{
	firsts.push_back("func");
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("id");
	follows.push_back("public");
	follows.push_back("private");
	follows.push_back("func");
	follows.push_back("integer");
	follows.push_back("float");
	follows.push_back("string");
	follows.push_back("id");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "func") {
		if (funcDecl())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		if (varDecl())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::funcDecl()
{
	firsts.push_back("func");
	follows.push_back("public");
	follows.push_back("private");
	follows.push_back("integer");
	follows.push_back("float");
	follows.push_back("string");
	follows.push_back("func");
	follows.push_back("id");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "func") {
		if (match("func") && match("id") && match("openpar") && fParams() && match("closepar") && match("colon") && funcDeclTail() && match("semi"))
			return true;
		else
			return false;
	}

	return false;
}

bool parser::varDecl()
{
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("id");
	follows.push_back("rcurbr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		if (type() && match("id") && arraySizeRept() && match(";"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::type()
{
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("id");
	follows.push_back("opencubr");
	follows.push_back("semi");
	follows.push_back("id");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "integer") {
		if (match("integer"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "float") {
		if (match("float"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "string") {
		if (match("string"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "id") {
		if (match("id"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::arraySizeRept()
{
	if (lookahead[0] == "opensqbr") {
		if (match("opensqbr") && intNum() && match("closesqbr") && arraySizeRept())
			return true;
		else
			return false;
	}
	else
		return true;
}

bool parser::intNum()
{
	if (lookahead[0] == "intNum") {
		if (match("intNum")) {
			return true;
		}
	}
	else
		return true;
}

bool parser::fParams()
{
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("id");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		if (type() && match("id") && arraySizeRept() && fParamsTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::funcDeclTail()
{
	firsts.push_back("void");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("integer");
	firsts.push_back("id");
	follows.push_back("closepar");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "void") {
		if (match("void"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "float" || (lookahead[0] == "string") || (lookahead[0] == "integer") || (lookahead[0] == "id")) {
		if (type())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::fParamsTail()
{
	firsts.push_back("comma");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "comma") {
		if (match("comma") && type() && match("id") && arraySizeRept() && fParamsTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::function()
{
	firsts.push_back("func");
	follows.push_back("main");
	follows.push_back("func");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "func") {
		if (funcHead() && funcBody())
			return true;
		else
			return false;
	}
	return false;
}

bool parser::funcHead()
{
	firsts.push_back("func");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "func") {
		if (match("func") && match("id") && classMethod() && match("openpar") && fParams() && match("closepar") && match("colon") && funcDeclTail())
			return true;
		else return false;
	}
	else
		return false;
}

bool parser::funcBody()
{
	firsts.push_back("opencubr");
	follows.push_back("main");
	follows.push_back("func");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "opencubr") {
		if (match("opencubr") && methodBodyVar() && statementList() && match("closecubr")) {
			return true;
		}
		else
			return false;
	}
	return false;
}

bool parser::classMethod()
{
	firsts.push_back("sr");
	follows.push_back("openpar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "sr") {
		if (match("sr") && match("id"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "openpar")
		return true;
	else
		return false;
}

bool parser::methodBodyVar()
{
	firsts.push_back("var");
	follows.push_back("if");
	follows.push_back("while");
	follows.push_back("read");
	follows.push_back("write");
	follows.push_back("return");
	follows.push_back("break");
	follows.push_back("continue");
	follows.push_back("id");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "var") {
		if (match("var") && match("opencubr") && varDeclRep() && match("closecubr")) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "if" || lookahead[0] == "while" || lookahead[0] == "read" || lookahead[0] == "write" || lookahead[0] == "return" || lookahead[0] == "break" || lookahead[0] == "continue" | lookahead[0] == "id" || lookahead[0] == "closecubr") {
		return true;
	}
	else
		return false;
}

bool parser::statementList()
{
	firsts.push_back("if");
	firsts.push_back("while");
	firsts.push_back("read");
	firsts.push_back("write");
	firsts.push_back("return");
	firsts.push_back("break");
	firsts.push_back("cotinue");
	firsts.push_back("id");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "if" || (lookahead[0] == "while") || (lookahead[0] == "read") || (lookahead[0] == "write") || (lookahead[0] == "return") || (lookahead[0] == "break") || (lookahead[0] == "continue") || (lookahead[0] == "id")) {
		if (statement() && statementList())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closecubr")
		return true;
	else	
		return false;
}

bool parser::varDeclRep()
{
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("id");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "float" || (lookahead[0] == "string") || (lookahead[0] == "integer") || (lookahead[0] == "id")) {
		if (varDecl() && varDeclRep())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closecubr")
		return true;
	return false;
}

bool parser::statement()
{
	firsts.push_back("if");
	firsts.push_back("while");
	firsts.push_back("read");
	firsts.push_back("write");
	firsts.push_back("return");
	firsts.push_back("break");
	firsts.push_back("continue");
	firsts.push_back("id");
	follows.push_back("if");
	follows.push_back("while");
	follows.push_back("read");
	follows.push_back("write");
	follows.push_back("return");
	follows.push_back("break");
	follows.push_back("continue");
	follows.push_back("id");
	follows.push_back("else");
	follows.push_back("semi");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "id") {
		if (funcOrAssignStat() && match("semi"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "if") {
		if (match("if") && match("openpar") && expr() && match("closepar") && match("then") && statBlock() && match("else") && statBlock() && match("semi"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "while") {
		if (match("while") && match("openpar") && expr() && match("closepar") && statBlock() && match("semi"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "read") {
		if (match("read") && match("openpar") && variable() && match("closepar") && match("semi"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "write") {
		if (match("write") && match("openpar") && expr() && match("closepar") && match("semi"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "return") {
		if (match("return") && match("openpar") && expr() && match("closepar") && match("semi"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "break") {
		if (match("break") && match("semi"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "continue") {
		if (match("continue") && match("semi"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::funcOrAssignStat()
{
	firsts.push_back("id");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "id") {
		if (match("id") && funcOrAssignStatIdnest())
			return true;
		else return false;
	}
	else
		return false;
}

bool parser::expr()
{
	firsts.push_back("intnum");
	firsts.push_back("floatnum");
	firsts.push_back("stringlit");
	firsts.push_back("openpar");
	firsts.push_back("not");
	firsts.push_back("qmark");
	firsts.push_back("id");
	firsts.push_back("plus");
	firsts.push_back("minus");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "intnum" || lookahead[0] == "floatnum" || lookahead[0] == "stringlit" || lookahead[0] == "openpar" || lookahead[0] == "not" || lookahead[0] == "qmark" || lookahead[0] == "id" || lookahead[0] == "plus" || lookahead[0] == "minus") {
		if (arithExpr() && exprTail()) {
			return true;
		}
		else
			return false;
	}
	return false;
}

bool parser::funcOrAssignStatIdnest()
{
	firsts.push_back("openpar");
	firsts.push_back("opensqbr");
	firsts.push_back("dot");
	firsts.push_back("assign");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "opensqbr" || lookahead[0] == "dot" || lookahead[0] == "assign") {
		if (indiceRep() && funcOrAssignStatIdnestVarTail()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		if (match("openpar") && aParams() && match("closepar") && funcOrAssignStatIdnestFuncTail())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::indiceRep()
{
	firsts.push_back("opensqbr");
	follows.push_back("mult");
	follows.push_back("div");
	follows.push_back("and");
	follows.push_back("semi");
	follows.push_back("assign");
	follows.push_back("dot");
	follows.push_back("eq");
	follows.push_back("noteq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("plus");
	follows.push_back("minus");
	follows.push_back("leq");
	follows.push_back("or");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("closesqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "opensqbr") {
		if (match("opensqbr") && expr() && match("closesqbr") && indiceRep())
			return true;
	}
	else if (lookahead[0] == "mult" || lookahead[0] == "div" || lookahead[0] == "and" || lookahead[0] == "semi" || lookahead[0] == "assign" || lookahead[0] == "dot" || lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "or" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar") {
		return true;
	}
	else
		return false;
}

bool parser::aParams()
{
	firsts.push_back("intnum");
	firsts.push_back("floatnum");
	firsts.push_back("stringlit");
	firsts.push_back("openpar");
	firsts.push_back("not");
	firsts.push_back("qmark");
	firsts.push_back("id");
	firsts.push_back("plus");
	firsts.push_back("minus");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "intnum" || lookahead[0] == "floatnum" || lookahead[0] == "stringlit" || lookahead[0] == "openpar" || lookahead[0] == "not" || lookahead[0] == "qmark" || lookahead[0] == "id" || lookahead[0] == "plus" || lookahead[0] == "minus") {
		if (expr() && aParamsTail()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::aParamsTail()
{
	firsts.push_back("comma");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "comma") {
		if (match("comma") && expr() && aParamsTail())
			return true;
	}
	else if (lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::funcOrAssignStatIdnestFuncTail()
{
	firsts.push_back("dot");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "dot") {
		if (match("dot") && match("id") && funcStatTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi")
		return true;
	else
		return false;
}

bool parser::funcStatTail()
{
	firsts.push_back("dot");
	firsts.push_back("openpar");
	firsts.push_back("opensqbr");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "dot" || lookahead[0] == "opensqbr") {
		if (indiceRep() && match("dot") && match("id") && funcStatTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		if (match("openpar") && aParams() && match("closepar") && funcStatTailIdnest())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::funcStatTailIdnest()
{

	firsts.push_back("dot");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "dot") {
		if (match("dot") && match("id") && funcStatTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi")
		return true;
	else
		return false;
}

bool parser::arithExpr()
{
	firsts.push_back("intnum");
	firsts.push_back("floatnum");
	firsts.push_back("stringlit");
	firsts.push_back("openpar");
	firsts.push_back("not");
	firsts.push_back("qmark");
	firsts.push_back("id");
	firsts.push_back("plus");
	firsts.push_back("minus");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("neq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("closesqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();
	
	if (lookahead[0] == "intnum" || lookahead[0] == "floatnum" || lookahead[0] == "stringlit" || lookahead[0] == "openpar" || lookahead[0] == "not" || lookahead[0] == "qmark" || lookahead[0] == "id" || lookahead[0] == "plus" || lookahead[0] == "minus") {
		if (term() && arithExprTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "neq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::exprTail()
{

	firsts.push_back("eq");
	firsts.push_back("neq");
	firsts.push_back("lt");
	firsts.push_back("gt");
	firsts.push_back("leq");
	firsts.push_back("geq");
	follows.push_back("semi");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("closesqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "eq" || lookahead[0] == "neq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq") {
		if (relOp() && arithExpr())
			return true;
		else
			return false;
	}

	return false;
}

bool parser::term()
{
	firsts.push_back("intnum");
	firsts.push_back("floatnum");
	firsts.push_back("stringlit");
	firsts.push_back("openpar");
	firsts.push_back("not");
	firsts.push_back("qmark");
	firsts.push_back("id");
	firsts.push_back("plus");
	firsts.push_back("minus");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("neq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("plus");
	follows.push_back("minus");
	follows.push_back("comma");
	follows.push_back("or");
	follows.push_back("colon");
	follows.push_back("closesqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "intnum" || lookahead[0] == "floatnum" || lookahead[0] == "stringlit" || lookahead[0] == "openpar" || lookahead[0] == "not" || lookahead[0] == "qmark" || lookahead[0] == "id" || lookahead[0] == "plus" || lookahead[0] == "minus") {
		if (factor() && varDeclRep()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "neq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "comma" || lookahead[0] == "colon" ||  lookahead[0] == "or" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::arithExprTail()
{
	firsts.push_back("or");
	firsts.push_back("plus");
	firsts.push_back("minus");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("neq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("closeqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "or") {
		if (addOp() && term() && arithExprTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "neq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::factor()
{
	firsts.push_back("intnum");
	firsts.push_back("floatnum");
	firsts.push_back("stringlit");
	firsts.push_back("openpar");
	firsts.push_back("not");
	firsts.push_back("qmark");
	firsts.push_back("id");
	firsts.push_back("plus");
	firsts.push_back("minus");
	follows.push_back("mult");
	follows.push_back("div");
	follows.push_back("and");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("neq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("plus");
	follows.push_back("minus");
	follows.push_back("or");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("opensqbr");
	follows.push_back("openpar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "id") {
		if (funcOrVar()) {
			return true;
		}
		return false;
	}
	else if (lookahead[0] == "intnum") {
		if (match("intnum"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "floatnum") {
		if (match("floatnum"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "stringlit") {
		if (match("stringlit"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		if (match("openpar") && expr() && match("closepar"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "not") {
		if (match("not") && factor())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "plus" || lookahead[0] == "minus") {
		if (sign() && factor())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "qmark") {
		if (match("qmark") && match("opensqbr") && expr() && match("colon") && expr() && match("colon") && expr() && match("closesqbr"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::funcOrVar()
{
	firsts.push_back("id");
	follows.push_back("mult");
	follows.push_back("div");
	follows.push_back("and");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("neq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("plus");
	follows.push_back("minus");
	follows.push_back("or");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("leftsqbr");
	follows.push_back("leftpar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "id") {
		if (funcOrVarIdnest())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::sign()
{
	firsts.push_back("plus");
	firsts.push_back("minus");
	follows.push_back("intnum");
	follows.push_back("floatnum");
	follows.push_back("stringlit");
	follows.push_back("openpar");
	follows.push_back("not");
	follows.push_back("qmark");
	follows.push_back("id");
	follows.push_back("plus");
	follows.push_back("minus");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "plus") {
		if (match("plus"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "minus") {
		if (match("minus"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::funcOrVarIdnest()
{
	firsts.push_back("openpar");
	firsts.push_back("opensqbr");
	firsts.push_back("dot");
	follows.push_back("mult");
	follows.push_back("div");
	follows.push_back("and");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("neq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("plus");
	follows.push_back("minus");
	follows.push_back("or");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("leftsqbr");
	follows.push_back("leftpar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "leftsqbr" || "dot") {
		if (indiceRep() && funcOrVarIdnestTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "leftpar") {
		if (match("leftpar") && aParams() && match("rightpar") && funcOrVarIdnestTail())
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::funcOrVarIdnestTail()
{
	firsts.push_back("dot");
	follows.push_back("mult");
	follows.push_back("div");
	follows.push_back("and");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("neq");
	follows.push_back("lt");
	follows.push_back("gt");
	follows.push_back("leq");
	follows.push_back("geq");
	follows.push_back("plus");
	follows.push_back("minus");
	follows.push_back("or");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("closesqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "dot") {
		if (match("dot") && match("id") && funcOrVarIdnest())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "mult" || lookahead[0] == "div" || lookahead[0] == "and" || lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "neq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "or" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar")
		return true;
	else
		return false;
}

bool parser::addOp()
{
	firsts.push_back("plus");
	firsts.push_back("minus");
	firsts.push_back("or");
	follows.push_back("intnum");
	follows.push_back("floatnum");
	follows.push_back("stringlit");
	follows.push_back("openpar");
	follows.push_back("not");
	follows.push_back("qmark");
	follows.push_back("id");
	follows.push_back("plus");
	follows.push_back("minus");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "plus") {
		if (match("plus"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "minus") {
		if (match("minus")) {
			return true;
		}
		else
			return false;
	}
	if (lookahead[0] == "or") {
		if (match("or")) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool parser::relOp()
{
	firsts.push_back("eq");
	firsts.push_back("neq");
	firsts.push_back("lt");
	firsts.push_back("gt");
	firsts.push_back("leq");
	firsts.push_back("geq");
	follows.push_back("intnum");
	follows.push_back("floatnum");
	follows.push_back("stringlit");
	follows.push_back("openpar");
	follows.push_back("not");
	follows.push_back("qmark");
	follows.push_back("id");
	follows.push_back("plus");
	follows.push_back("minus");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "eq") {
		if (match("eq"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "neq") {
		if (match("neq"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "lt") {
		if (match("lt"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "gt") {
		if (match("gt"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "leq") {
		if (match("leq"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "geq") {
		if (match("geq"))
			return true;
		else
			return false;
	}
	else
		return false;
}
