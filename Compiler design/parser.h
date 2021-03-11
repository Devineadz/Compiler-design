#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "lexer.h"

using namespace std;

class parser {
public:
	parser(string path);
	~parser();
private:
	string* nextToken(); // array
	string token;
	string token_path;
	lexer* tokenizer = new lexer(token_path);
	string* lookahead;
	vector<string>firsts;
	vector<string>follows;
	bool skipErrors(vector<string>firsts, vector<string>follows);
	fstream errorFile;
	string errorName;
	void initializeErrorfile(string fileName);


	bool parse();
	bool start();
	bool prog();
	bool classdecl();
	bool funcDef();
	bool match(string token);
	bool reptprog0();
	bool reptprog1();
	bool funcbody();
	bool inherit();
};
