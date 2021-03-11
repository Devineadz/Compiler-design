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
	firsts.push_back("main");
	follows.push_back("epsilon");
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
	firsts.push_back("main");
	follows.push_back("epsilon");
	if (!skipErrors(firsts, follows))
		return false;
	firsts.clear();
	follows.clear();

	if (lookahead[0] == "class") {
		while (lookahead[0] == "class") {
			if (classdecl()) {}
			else
				return false();
		}
	}
	if (lookahead[0] == "func") {
		while (lookahead[0] == "func"){
			if (funcDef()) {}
			else
				return false();
}
	}
	if (lookahead[0] == "main") {
		if (match("main") && funcbody()) {
			return(true);
		}
	}
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
		if (match("class") && match("id")) {
			if (lookahead[0] == "inherits") {
				if (inherit()) {}
				else
					return false;
			}
			else if(lookahead[0] == "{")
				if (match("{")) {

				}
		}
	}
}

bool parser::funcDef()
{
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

bool parser::reptprog1()
{
	return false;
}

bool parser::funcbody()
{
	return false;
}
