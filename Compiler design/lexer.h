#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class lexer
{
public:
	lexer(string path);
	~lexer();

	string checkReservedWords(string token);
	string getNextToken();
	string getLexeme();
	string getRow();
private:
	string nextToken();
	vector<string> fileRows;
	string lexeme;
	string tokenHolder;
	bool finalState;
	bool needsBacking;
	int lineCounter;
	string nextLine;
	bool isLast;
	bool endFile;
	vector<string> tokenVector(string newpath);
	char nextChar();
	bool isFinalState();
	string createToken();
	bool backUp();
	void backUpChar();
	int table(int state, char lookup);
};