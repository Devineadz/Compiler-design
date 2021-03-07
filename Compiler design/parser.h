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
	string getTokens();
	string token_path;
	lexer* tokenizer = new lexer(token_path);

};
