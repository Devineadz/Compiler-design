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
	string* getNextToken();
private:
	string nextToken();
	vector<string> fileRows;
	string lexeme = "";
	string tokenHolder = "";
	bool finalState = false;
	bool needsBacking = false;
	int lineCounter = 0;
	string nextLine = "";
	bool isLast = false;
	bool endFile = false;
	vector<string> tokenVector(string path);
	char nextChar();
	bool isFinalState();
	string createToken();
	bool backUp();
	void backUpChar();
	int table(int state, char lookup);
};