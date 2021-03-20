/*
Compiler Design
Diana Zitting-Rioux
Recursive-descent predictive parse using stack
*/

#include "parser.h"

parser::parser(string path)
{
	token_path = path;
	tokenizer = new lexer(token_path);
	initializeFile(path);
	eof = false;
	ast = new Prog();
}

parser::~parser()
{
}

// extract next token from lexer.
void parser::nextToken()
{	
	if (lookahead[0] == "$") {
		return;
	}
	if (eof == true) {
		errorFile.open(errorName, ios::app);
		if (errorFile.is_open()) {
			errorFile << "No more tokens to process.\n";
		}
		errorFile.close();
		exit(0);
	}
	string placeHolder = tokenizer->getNextToken();
	while (placeHolder == "blockcmt" || placeHolder =="" || placeHolder == "inlinecmt") {
		placeHolder = tokenizer->getNextToken();
	}
	if (placeHolder == "$") {
		eof = true;
	}
	string placeHolder2 = tokenizer->getLexeme();
	string placeHolder3 = tokenizer->getRow();
	lookahead[0] = placeHolder;
	lookahead[1] = placeHolder2;
	lookahead[2] = placeHolder3;
}

// checks 2 vectors of first and follow sets
bool parser::skipErrors(vector<string>firsts, vector<string>follows)
{
	for (vector<string>::iterator it = firsts.begin(); it < firsts.end(); it++)
	{
		if (lookahead[0] == *it) {
			return true;
		}
		else {
			for (vector<string>::iterator it2 = follows.begin(); it2 < follows.end(); it2++) {
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
		nextToken();
		for (vector<string>::iterator it = firsts.begin(); it < firsts.end(); ++it)
		{
			if (lookahead[0] == *it) {
				lookaheadCheck = true;
				return true;
			}
			else {
				for (vector<string>::iterator it2 = follows.begin(); it2 < follows.end(); it2++)
					if (*it == "epsilon" && lookahead[0] == *it2)
						lookaheadCheck = true;
				return false;
			}
		}
	}
	return false;
}

// initializes files with the file name
void parser::initializeFile(string fileName)
{
	string newFileName;
	for (int count = 0; fileName[count] != '.'; count++) { // gets the name from the original file name
		newFileName.push_back(fileName[count]);
	}
	errorName = newFileName + ".outsyntaxerrors";
	derivationName = newFileName + ".outderivation";
	diagramName = newFileName + ".svg";

	errorFile.open(errorName, ofstream::out | ofstream::trunc); // clear contents from files
	errorFile.close();

	derivationFile.open(derivationName, ofstream::out | ofstream::trunc); // clear contents from files
	derivationFile.close();

	diagramFile.open(diagramName, ofstream::out | ofstream::trunc); // clear contents from files
	if (diagramFile .is_open()) {
		diagramFile << "digraph EST{" << "\n";
	}
	diagramFile.close();
}

// Parsing starter
bool parser::parse()
{
	nextToken();
	if (start() & match("$")) {
		cout << "Parsing ready";
		return true;
	}
	else
		return false;
}

// Recursive descent beginning
bool parser::start()
{

	derivation = "START";
	writeToDerivation();
	firsts.push_back("main");
	firsts.push_back("class");
	firsts.push_back("func");
	follows.push_back("$");	
	ESTmaker* est = new ESTmaker();
	est_stack.push(ast);
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();
	if (lookahead[0] == "main" || lookahead[0] == "class" || lookahead[0] == "func") {
		replace("START", "PROG");
		if (prog()) {
			diagramFile.open(diagramName, ios::app);
			if (diagramFile.is_open()) {
				diagramFile << "}";
			}
			diagramFile.close();
			return(true);
		}
	}
	return false;
}

// writes to derivation file
void parser::writeToDerivation()
{
	derivationFile.open(derivationName, ios::app);
	if (derivationFile.is_open()) {
		derivationFile << derivation << "\n";
	}
	derivationFile.close();
}

// writes to diagram file
void parser::diagramToFile(string parent, string child)
{
	diagramFile.open(diagramName, ios::app);
	if (diagramFile.is_open()) {
		diagramFile << parent << " -> " << child << "\n";
	}
	diagramFile.close();
}

// function for derivation file, replaces a string with another
void parser::replace(string deriv_func, string deriv_repl)
{
	size_t found = derivation.find(deriv_func);
	if (found != string::npos) {
		derivation.erase(found, deriv_func.length());
		derivation.insert(found, deriv_repl);
		writeToDerivation();
	}
}

bool parser::prog()
{
	EST* prog_ast = est_stack.top();
	est_stack.pop();
	firsts.push_back("main");
	firsts.push_back("class");
	firsts.push_back("func");
	follows.push_back("$");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();


	if (lookahead[0] == "main" || lookahead[0] == "class" || lookahead[0] == "func") {
		replace("PROG", "CLASSDECL FUNCDEF main FUNCBODY");
		EST* class_decl_est = est->makeNode("class_decl");
		EST* func_def_est = est->makeNode("func_def");
		EST* func_body_est = est->makeNode("func_body");
		diagramToFile("PROG", "CLASSDECL");
		diagramToFile("PROG", "FUNCDEF");
		diagramToFile("PROG", "FUNCBODY");
		est_stack.push(func_body_est);
		est_stack.push(func_def_est);
		est_stack.push(class_decl_est);


		if (classdecl() & funcDef() & match("main") & funcbody()){
			prog_ast = est_stack.top();
			est_stack.pop();
			est_stack.push(prog_ast);
			return true;
	}
		else
			return false;
	}
	else
		return false;
}

bool parser::classdecl()
{
	EST* classd_ast = est_stack.top();
	est_stack.pop();
	EST* func_def_est = est_stack.top();
	est_stack.pop();
	
	firsts.push_back("class");
	firsts.push_back("epsilon");
	follows.push_back("func");
	follows.push_back("main");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "class") {
		replace("CLASSDECL", "class id INHERIT opencubr CLASSDECLBODY closecubr semi CLASSDECL");
		EST* inherit_ast = est->makeNode("inherit");
		EST* classdeclbody_ast = est->makeNode("classdeclbody");
		EST* classdecl_ast = est->makeNode("classdecl");

		est_stack.push(classdecl_ast);
		est_stack.push(classdeclbody_ast);
		est_stack.push(inherit_ast);
		if (match("class")) {
			string token_id = lookahead[1];
			if (match("id") & inherit() & match("opencubr") & classdeclbody() & match("closecubr") & match("semi") & classdecl()) {
				// Make family of nodes under classdecl and push on stack
				EST* id_ast = est->makeNode("Class", token_id);
				classd_ast->makeFamily(id_ast);
				id_ast->makeSiblings(classdecl_ast);
				est_stack.push(classd_ast);
				string idToFile = "class" + token_id;
				diagramToFile(idToFile, "inherit");
				diagramToFile(idToFile, "classdeclbody");
				diagramToFile("CLASSDECL", idToFile);
				return true;
			}
			else
				return false;	
		}
		else
			return false;
	}
	else if (lookahead[0] == "func" || lookahead[0] == "main") {
		func_def_est = classd_ast;
		est_stack.push(func_def_est);
		replace("CLASSDECL ", "");
		return true;
	}
	else
		return false;
}

bool parser::funcDef()
{
	EST* funcdef_ast = est_stack.top();
	est_stack.pop();
	EST* func_body_est = est_stack.top();
	est_stack.pop();

	firsts.push_back("func");
	firsts.push_back("epsilon");
	follows.push_back("main");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();



	if (lookahead[0] == "func") {
		// Fix and make family
		EST* function_ast = est->makeNode("function");
		EST* funcdef_ast = est->makeNode("funcdef");

		est_stack.push(function_ast);
		est_stack.push(funcdef_ast);

		replace("FUNCDEF", "FUNCTION FUNCDEF");
		if (function() & funcDef()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "main") {
		func_body_est = funcdef_ast;
		est_stack.push(func_body_est);
		replace("FUNCDEF ", "");
		return true;
	}
	else
		return false;
}

// Does the token matching.
bool parser::match(string token)
{
	if (lookahead[0] == token) {
		nextToken();
		return true;
	}
	else {
		nextToken();
			return false;
	}
}

bool parser::funcbody()
{
	EST* funcbody_ast = est_stack.top();
	est_stack.pop();
	firsts.push_back("opencubr");
	follows.push_back("main");
	follows.push_back("func");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "opencubr") {
		replace("FUNCBODY", "opencubr METHODBODYVAR STATEMENTLIST closecubr");
		EST* methodBodyVar_est = est->makeNode("methodbodyvar");
		EST* statementList_est = est->makeNode("statementlist");
		methodBodyVar_est->makeSiblings(statementList_est);
		est_stack.push(statementList_est);
		est_stack.push(methodBodyVar_est);
		if (match("opencubr") & methodBodyVar() & statementList() & match("closecubr")) {
			funcbody_ast = statementList_est;
			est_stack.push(funcbody_ast);
			return true;
		}
		else
			return false;
	}
	else
		return true;
}


bool parser::inherit()
{
	firsts.push_back("inherits");
	firsts.push_back("epsilon");
	follows.push_back("opencubr");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "inherits") {
		replace("INHERIT", "inherits id NESTEDID");
		if (match("inherits") & match("id") & nestedId()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "opencubr") {
		replace("INHERIT ", "");
		return true;
	}
	else
		return false;
}

bool parser::nestedId()
{
	firsts.push_back("comma");
	firsts.push_back("epsilon");
	follows.push_back("opencubr");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "comma") {
		replace("NESTEDID", "comma id NESTEDID");
		if (match("comma") & match("id") & nestedId()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "opencubr") {
		replace("NESTEDID ", "");
		return true;
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
	firsts.push_back("epsilon");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "public" || lookahead[0] == "private" || lookahead[0] == "func" || lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		replace("CLASSDECLBODY", "VISIBILITY MEMBERDECL CLASSDECLBODY");
		if (visibility() & memberDecl() & classdeclbody())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closecubr") {
		replace("CLASSDECLBODY ", "");
		return true;
	}
	else
		return false;
}

bool parser::visibility()
{
	firsts.push_back("public");
	firsts.push_back("private");
	firsts.push_back("epsilon");
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
		replace("VISIBILITY", "public");
		if (match("public"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "private") {
		replace("VISIBILITY", "private");
		if (match("private"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "func" || lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		replace("VISIBILITY ", "");
		return true;
	}
	else
		return false;
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
		replace("MEMBERDECL", "FUNCDECL");
		if (funcDecl())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		replace("MEMBERDECL", "VARDECL");
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
		replace("FUNCDECL", "func id openpar FPARAMS closepar colon FUNCDECLTAIL semi");
		if (match("func") & match("id") & match("openpar") & fParams() & match("closepar") & match("colon") & funcDeclTail() & match("semi"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::varDecl()
{
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

	if (lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		replace("VARDECL", "TYPE id ARRAYSIZEREPT semi");
		if (type() & match("id") & arraySizeRept() & match("semi"))
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
		replace("TYPE", "integer");
		if (match("integer"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "float") {
		replace("TYPE", "float");
		if (match("float"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "string") {
		replace("TYPE", "string");
		if (match("string"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "id") {
		replace("TYPE", "id");
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
	firsts.push_back("opensqbr");
	firsts.push_back("epsilon");
	follows.push_back("closepar");
	follows.push_back("comma");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();
	if (lookahead[0] == "opensqbr") {
		replace("ARRAYSIZEREPT", "opensqbr INTNUM closesqbr ARRAYSIZEREPT");
		if (match("opensqbr") & intNum() & match("closesqbr") & arraySizeRept())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "comma" || lookahead[0] == "closepar") {
		replace("ARRAYSIZEREPT ", "");
		return true;
	}
	else
		return false;
}

bool parser::intNum()
{
	firsts.push_back("intnum");
	firsts.push_back("epsilon");
	follows.push_back("closesqbr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();
	if (lookahead[0] == "intnum") {
		replace("INTNUM ", "intnum");
		if (match("intnum")) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "closesqbr") {
		replace("INTNUM ", "");
		return true;
	}
	else
		return true;
}

bool parser::fParams()
{
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("epsilon");
	firsts.push_back("id");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "integer" || lookahead[0] == "float" || lookahead[0] == "string" || lookahead[0] == "id") {
		replace("FPARAMS", "TYPE id ARRAYSIZEREPT FPARAMSTAIL");
		if (type() & match("id") & arraySizeRept() & fParamsTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closepar") {
		replace("FPARAMS ", "");
		return true;
	}
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
		replace("FUNCDECLTAIL", "void");
		if (match("void"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "float" || (lookahead[0] == "string") || (lookahead[0] == "integer") || (lookahead[0] == "id")) {
		replace("FUNCDECLTAIL", "TYPE");
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
	firsts.push_back("epsilon");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "comma") {
		replace("FPARAMSTAIL", "comma TYPE id ARRAYSIZEREPT FPARAMSTAIL");
		if (match("comma") & type() & match("id") & arraySizeRept() & fParamsTail()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "closepar") {
		replace("FPARAMSTAIL ", "");
		return true;
	}
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
		replace("FUNCTION", "FUNCHEAD FUNCBODY");
		if (funcHead() & funcBody())
			return true;
		else
			return false;
	}
	return false;
}

bool parser::funcHead()
{
	firsts.push_back("func");
	follows.push_back("opencubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "func") {
		replace("FUNCHEAD", "func id CLASSMETHOD openpar FPARAMS closepar colon FUNCDECLTAIL");
		if (match("func") & match("id") & classMethod() & match("openpar") & fParams() & match("closepar") & match("colon") & funcDeclTail())
			return true;
		else 
			return false;
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
		replace("FUNCBODY", "opencubr METHODBODYVAR STATEMENTLIST closecubr");
		if (match("opencubr") & methodBodyVar() & statementList() & match("closecubr")) {
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
	firsts.push_back("epsilon");
	follows.push_back("openpar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "sr") {
		replace("CLASSMETHOD", "sr id");
		if (match("sr") & match("id"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		replace("CLASSMETHOD ", "");
		return true;
	}
	else
		return false;
}

bool parser::methodBodyVar()
{
	firsts.push_back("var");
	firsts.push_back("epsilon");
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
		replace("METHODBODYVAR", "var opencubr VARDECLREP closecubr");
		if (match("var") & match("opencubr") & varDeclRep() & match("closecubr")) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "if" || lookahead[0] == "while" || lookahead[0] == "read" || lookahead[0] == "write" || lookahead[0] == "return" || lookahead[0] == "break" || lookahead[0] == "continue" | lookahead[0] == "id" || lookahead[0] == "closecubr") {
		replace("METHODBODYVAR ", "");
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
	firsts.push_back("continue");
	firsts.push_back("id");
	firsts.push_back("epsilon");
	follows.push_back("closecubr");


	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "if" || (lookahead[0] == "while") || (lookahead[0] == "read") || (lookahead[0] == "write") || (lookahead[0] == "return") || (lookahead[0] == "break") || (lookahead[0] == "continue") || (lookahead[0] == "id")) {
		replace("STATEMENTLIST", "STATEMENT STATEMENTLIST");
		if (statement() & statementList())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closecubr") {
		replace("STATEMENTLIST ", "");
		return true;
	}
	else	
		return false;
}

bool parser::varDeclRep()
{
	firsts.push_back("integer");
	firsts.push_back("float");
	firsts.push_back("string");
	firsts.push_back("id");
	firsts.push_back("epsilon");
	follows.push_back("closecubr");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "float" || (lookahead[0] == "string") || (lookahead[0] == "integer") || (lookahead[0] == "id")) {
		replace("VARDECLREP", "VARDECL VARDECLREP");
		if (varDecl() & varDeclRep())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closecubr") {
		replace("VARDECLREP ", "");
		return true;
	}
	else
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
		replace("STATEMENT", "FUNCORASSIGNSTAT semi");
		if (funcOrAssignStat() & match("semi"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "if") {
		replace("STATEMENT", "if openpar EXPR closepar then STATBLOCK else STATBLOCK semi");
		if (match("if") & match("openpar") & expr() & match("closepar") & match("then") & statBlock() & match("else") & statBlock() & match("semi"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "while") {
		replace("STATEMENT", "while openpar EXPR closepar STATBLOCK semi");
		if (match("while") & match("openpar") & expr() & match("closepar") & statBlock() & match("semi"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "read") {
		replace("STATEMENT", "read openpar VARIABLE closepar semi");
		if (match("read") & match("openpar") & variable() & match("closepar") & match("semi"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "write") {
		replace("STATEMENT", "write openpar EXPR closepar semi");
		if (match("write") & match("openpar") & expr() & match("closepar") & match("semi"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "return") {
		replace("STATEMENT", "return openpar EXPR closepar semi");
		if (match("return") & match("openpar") & expr() & match("closepar") & match("semi"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "break") {
		replace("STATEMENT", "break semi");
		if (match("break") & match("semi"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "continue") {
		replace("STATEMENT", "continue semi");
		if (match("continue") & match("semi"))
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
		replace("FUNCORASSIGNSTAT", "id FUNCORASSIGNSTATIDNEST");
		if (match("id") & funcOrAssignStatIdnest())
			return true;
		else 
			return false;
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
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("closesqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "intnum" || lookahead[0] == "floatnum" || lookahead[0] == "stringlit" || lookahead[0] == "openpar" || lookahead[0] == "not" || lookahead[0] == "qmark" || lookahead[0] == "id" || lookahead[0] == "plus" || lookahead[0] == "minus") {
		replace("EXPR", "ARITHEXPR EXPRTAIL");
		if (arithExpr() & exprTail()) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool parser::statBlock()
{
	firsts.push_back("opencubr");
	firsts.push_back("if");
	firsts.push_back("while");
	firsts.push_back("read");
	firsts.push_back("write");
	firsts.push_back("return");
	firsts.push_back("break");
	firsts.push_back("continue");
	firsts.push_back("id");
	firsts.push_back("epsilon");
	follows.push_back("else");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "opencubr") {
		replace("STATBLOCK", "opencubr STATEMENTLIST closecubr");
		if (match("opencubr") & statementList() & match("closecubr"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "if" || lookahead[0] == "while" || lookahead[0] == "read" || lookahead[0] == "write" || lookahead[0] == "return" || lookahead[0] == "break" || lookahead[0] == "continue" || lookahead[0] == "id") {
		replace("STATBLOCK", "STATEMENT");
		if (statement()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "else" || lookahead[0] == "semi") {
		replace("STATBLOCK ", "");
		return true;
	}
	else
		return false;
}

bool parser::variable()
{
	firsts.push_back("id");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "id") {
		replace("VARIABLE", "id VARIABLEIDNEST");
		if (match("id") & variableIdnest())
			return true;
		else
			return false;
	}
	else
		return false;	
}

bool parser::variableIdnest()
{
	firsts.push_back("opensqbr");
	firsts.push_back("dot");
	firsts.push_back("epsilon");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "opensqbr" || lookahead[0] == "dot") {
		replace("VARIABLEIDNEST", "INDICEREP VARIABLEIDNESTTAIL");
		if (indiceRep() & variableIdNestTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closepar") {
		replace("VARIABLEIDNEST ", "");
		return true;
	}
	else
		return false;
}

bool parser::variableIdNestTail()
{
	firsts.push_back("dot");
	firsts.push_back("epsilon");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "dot") {
		replace("VARIABLEIDNESTTAIL", "dot id VARIABLEIDNEST");
		if (match("dot") & match("id") & variableIdnest())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closepar") {
		replace("VARIABLEIDNESTTAIL ", "");
		return true;
	}
		else
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
		replace("FUNCORASSIGNSTATIDNEST", "INDICEREP FUNCORASSIGNSTATIDNESTVARTAIL");
		if (indiceRep() & funcOrAssignStatIdnestVarTail()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		replace("FUNCORASSIGNSTATIDNEST", "openpar APARAMS closepar FUNCORASSIGNSTATIDNESTFUNCTAIL");
		if (match("openpar") & aParams() & match("closepar") & funcOrAssignStatIdnestFuncTail())
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
	firsts.push_back("epsilon");
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
		replace("INDICEREP", "opensqbr EXPR closesqbr INDICEREP");
		if (match("opensqbr") & expr() & match("closesqbr") & indiceRep())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "mult" || lookahead[0] == "div" || lookahead[0] == "and" || lookahead[0] == "semi" || lookahead[0] == "assign" || lookahead[0] == "dot" || lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "or" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar") {
		replace("INDICEREP ", "");
		return true;
	}
	else
		return false;
}

bool parser::funcOrAssignStatIdnestVarTail()
{
	firsts.push_back("dot");
	firsts.push_back("assign");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();
	
	if (lookahead[0] == "dot") {
		replace("FUNCORASSIGNSTATIDNESTVARTAIL", "dot id FUNCORASSIGNSTATIDNEST");
		if (match("dot") & match("id") & funcOrAssignStatIdnest())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "assign") {
		replace("FUNCORASSIGNSTATIDNESTVARTAIL", "ASSIGNSTATTAIL");
		if (assignStatTail())
			return true;
		else
			return false;
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
	firsts.push_back("epsilon");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "intnum" || lookahead[0] == "floatnum" || lookahead[0] == "stringlit" || lookahead[0] == "openpar" || lookahead[0] == "not" || lookahead[0] == "qmark" || lookahead[0] == "id" || lookahead[0] == "plus" || lookahead[0] == "minus") {
		replace("APARAMS", "EXPR APARAMSTAIL");
		if (expr() && aParamsTail()) {
			return true;
		}
		else
			return false;
	}
	else if (lookahead[0] == "closepar") {
		replace("APARAMS ", "");
		return true;
	}
	else
		return false;
}

bool parser::aParamsTail()
{
	firsts.push_back("comma");
	firsts.push_back("epsilon");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "comma") {
		replace("APARAMSTAIL", "comma EXPR APARAMSTAIL");
		if (match("comma") & expr() & aParamsTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closepar"){
		replace("APARAMSTAIL ", "");
		return true;
	}
	else
		return false;
}

bool parser::funcOrAssignStatIdnestFuncTail()
{
	firsts.push_back("dot");
	firsts.push_back("epsilon");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "dot") {
		replace("FUNCORASSIGNSTATIDNESTFUNCTAIL", "dot id FUNCSTATTAIL");
		if (match("dot") & match("id") & funcStatTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi") {
		replace("FUNCORASSIGNSTATIDNESTFUNCTAIL ", "");
		return true;
	}	
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
		replace("FUNCSTATTAIL", "INDICEREP dot id FUNCSTATTAIL");
		if (indiceRep() & match("dot") & match("id") & funcStatTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		replace("FUNCSTATTAIL", "openpar APARAMS closepar FUNCSTATTAILIDNEST");
		if (match("openpar") & aParams() & match("closepar") & funcStatTailIdnest())
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
	firsts.push_back("epsilon");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "dot") {
		replace("FUNCSTATTAILIDNEST", "dot id FUNCSTATTAIL");
		if (match("dot") && match("id") && funcStatTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi") {
		replace("FUNCSTATTAILIDNEST ", "");
		return true;
	}
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
	firsts.push_back("epsilon");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("noteq");
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
		replace("ARITHEXPR", "TERM ARITHEXPRTAIL");
		if (term() & arithExprTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar") {
		replace("ARITHEXPR ", "");
		return true;
	}
	else
		return false;
}

bool parser::exprTail()
{

	firsts.push_back("eq");
	firsts.push_back("noteq");
	firsts.push_back("lt");
	firsts.push_back("gt");
	firsts.push_back("leq");
	firsts.push_back("geq");
	firsts.push_back("epsilon");
	follows.push_back("semi");
	follows.push_back("comma");
	follows.push_back("colon");
	follows.push_back("closesqbr");
	follows.push_back("closepar");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq") {
		replace("EXPRTAIL", "RELOP ARITHEXPR");
		if (relOp() & arithExpr())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar") {
		replace("EXPRTAIL ", "");
		return true;
	}
	else
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
	follows.push_back("noteq");
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
		replace("TERM", "FACTOR TERMTAIL");
		if (factor() & termTail()) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool parser::termTail()
{
	firsts.push_back("mult");
	firsts.push_back("div");
	firsts.push_back("and");
	firsts.push_back("epsilon");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("noteq");
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

	if (lookahead[0] == "mult" || lookahead[0] == "div" || lookahead[0] == "and") {
		replace("TERMTAIL", "MULTOP FACTOR TERMTAIL");
		if (multOp() & factor() & termTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "or" || lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar") {
		replace("TERMTAIL ", "");
		return true;
	}
	else
		return false;
}

bool parser::arithExprTail()
{
	firsts.push_back("or");
	firsts.push_back("plus");
	firsts.push_back("minus");
	firsts.push_back("epsilon");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("noteq");
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

	if (lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "or") {
		replace("ARITHEXPRTAIL", "ADDOP TERM ARITHEXPRTAIL");
		if (addOp() & term() & arithExprTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar") {
		replace("ARITHEXPRTAIL ", "");
		return true;
	}
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
	follows.push_back("noteq");
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

	if (lookahead[0] == "id") {
		replace("FACTOR", "FUNCORVAR");
		if (funcOrVar()) {
			return true;
		}
		return false;
	}
	else if (lookahead[0] == "intnum") {
		replace("FACTOR", "intnum");
		if (match("intnum"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "floatnum") {
		replace("FACTOR", "floatnum");
		if (match("floatnum"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "stringlit") {
		replace("FACTOR", "stringlit");
		if (match("stringlit"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		replace("FACTOR", "openpar EXPR closepar");
		if (match("openpar") & expr() & match("closepar"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "not") {
		replace("FACTOR", "not FACTOR");
		if (match("not") & factor())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "plus" || lookahead[0] == "minus") {
		replace("FACTOR", "sign FACTOR");
		if (sign() & factor())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "qmark") {
		replace("FACTOR", "qmark opensqbr EXPR colon EXPR colon EXPR closesqbr");
		if (match("qmark") & match("opensqbr") & expr() & match("colon") & expr() & match("colon") & expr() & match("closesqbr"))
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
	follows.push_back("noteq");
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

	if (lookahead[0] == "id") {
		replace("FUNCORVAR", "id FUNCORVARIDNEST");
		if (match("id") & funcOrVarIdnest())
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
		replace("SIGN", "plus");
		if (match("plus"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "minus") {
		replace("SIGN", "minus");
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
	firsts.push_back("epsilon");
	follows.push_back("mult");
	follows.push_back("div");
	follows.push_back("and");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("noteq");
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

	if (lookahead[0] == "opensqbr" || lookahead[0] == "dot") {
		replace("FUNCORVARIDNEST", "INDICEREP FUNCORVARIDNESTTAIL");
		if (indiceRep() & funcOrVarIdnestTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "openpar") {
		replace("FUNCORVARIDNEST", "openpar APARAMS closepar FUNCORVARIDNESTTAIL");
		if (match("openpar") & aParams() & match("closepar") & funcOrVarIdnestTail())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "closesqbr" || lookahead[0] == "closepar" || lookahead[0] == "semi" || lookahead[0] == "mult" || lookahead[0] == "div" || lookahead[0] == "and" || lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "or" || lookahead[0] == "comma" || lookahead[0] == "colon") {
		replace("FUNCORVARIDNEST ", "");
		return true;
	}
	else
		return false;
}

bool parser::funcOrVarIdnestTail()
{
	firsts.push_back("dot");
	firsts.push_back("epsilon");
	follows.push_back("mult");
	follows.push_back("div");
	follows.push_back("and");
	follows.push_back("semi");
	follows.push_back("eq");
	follows.push_back("noteq");
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
		replace("FUNCORVARIDNESTTAIL", "dot id FUNCORVARIDNEST");
		if (match("dot") & match("id") & funcOrVarIdnest())
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "mult" || lookahead[0] == "div" || lookahead[0] == "and" || lookahead[0] == "plus" || lookahead[0] == "minus" || lookahead[0] == "semi" || lookahead[0] == "eq" || lookahead[0] == "noteq" || lookahead[0] == "lt" || lookahead[0] == "gt" || lookahead[0] == "leq" || lookahead[0] == "geq" || lookahead[0] == "comma" || lookahead[0] == "colon" || lookahead[0] == "or" || lookahead[0] == "closesqbr" || lookahead[0] == "closepar") {
		replace("FUNCORVARIDNESTTAIL ", "");
		return true;
	}
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
		replace("ADDOP", "plus");
		if (match("plus"))
			return true;
		else
			return false;
	}
	if (lookahead[0] == "minus") {
		replace("ADDOP", "minus");
		if (match("minus")) {
			return true;
		}
		else
			return false;
	}
	if (lookahead[0] == "or") {
		replace("ADDOP", "or");
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
	firsts.push_back("noteq");
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
		replace("RELOP", "eq");
		if (match("eq"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "noteq") {
		replace("RELOP", "noteq");
		if (match("noteq"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "lt") {
		replace("RELOP", "lt");
		if (match("lt"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "gt") {
		replace("RELOP", "gt");
		if (match("gt"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "leq") {
		replace("RELOP", "leq");
		if (match("leq"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "geq") {
		replace("RELOP", "geq");
		if (match("geq"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::assignStatTail()
{
	firsts.push_back("assign");
	follows.push_back("semi");

	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "assign") {
		replace("ASSIGNSTATTAIL", "ASSIGNOP EXPR");
		if (assignOp() & expr()) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool parser::assignOp()
{
	firsts.push_back("assign");
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
	
	if (lookahead[0] == "assign") {
		replace("ASSIGNOP", "assign");
		if (match("assign"))
			return true;
		else
			return false;
	}
	else
		return false;
}

bool parser::multOp()
{
	firsts.push_back("mult");
	firsts.push_back("div");
	firsts.push_back("and");
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
	
	if (lookahead[0] == "mult") {
		replace("MULTOP", "mult");
		if (match("mult"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "div") {
		replace("MULTOP", "div");
		if (match("div"))
			return true;
		else
			return false;
	}
	else if (lookahead[0] == "and") {
		replace("MULTOP", "and");
		if (match("and"))
			return true;
		else
			return false;
	}
	else
		return false;
}
