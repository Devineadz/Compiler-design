#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

vector<string> fileRows;
string lexeme = "";
string tokenHolder = "";
bool finalState = false;
bool needsBacking = false;
int lineCounter = 0;

int main() {
	fstream srcFile;
	srcFile.open("test.text", ios::in);
	if (srcFile.is_open()) {
		string textLine;
		while (getline(srcFile, textLine)) {
			fileRows.push_back(textLine);
		}
	}

	while (!fileRows.empty()) {
		string token = nextToken();
		if (token == "error") {
			// write to errorlex
		}
		else {
			// write to lex
		}
	}
	
	return 0;
}

char nextChar() {
	// if not in vocab - return non valid vocab error
	//	return nextchar
	// lexeme = lexeme + newchar;
	return 'c';
}

bool isFinalState() {
	if (finalState == true)
		return true;
	else
		return false;
}

string createToken() {
	return tokenHolder;
}

bool backUp() {
	if (needsBacking == true)
		return true;
	else
		return false;
}

void backUpChar() {
	// backup one char
}



int table(int state, char lookup) {
	switch (state) {
	case 1: //A
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z')) {
			state = 4;
		}
		if (lookup > 0 && lookup < 10)
			state = 2;
		if (lookup == 0)
			state = 3;
		if (lookup == '"')
			state = 5;
		if (lookup == '+') {
			finalState = true;
			tokenHolder = "plus"; 
		}
		if (lookup == '-') {
			finalState = true;
			tokenHolder = "minus"; 
		}
		if (lookup == '_') { // error, cannot start with underscore
			finalState = true;
			tokenHolder = "error";
		}
		if (lookup == '.') {
			finalState = true;
			tokenHolder = "dot"; 
		}
		if (lookup == '=')
			state = 28;
		if (lookup == '(') {
			finalState = true;
			tokenHolder = "openpar"; 
		}
		if (lookup == ')') {
			finalState = true;
			tokenHolder = "closepar"; 
		}
		if (lookup == '{') {
			finalState = true;
			tokenHolder = "opencubr"; 
		}
		if (lookup == '}') {
			finalState = true;
			tokenHolder = "closecubr"; 
		}
		if (lookup == '[') {
			finalState = true;
			tokenHolder = "opensqbr"; 
		}
		if (lookup == ']') {
			finalState = true;
			tokenHolder = "closesqbr"; 
		}
		if (lookup == ';') {
			finalState = true;
			tokenHolder = "semi"; 
		}
		if (lookup == ',') {
			finalState = true;
			tokenHolder = "comma";
		}
		if (lookup == '|') {
			finalState = true;
			tokenHolder = "or"; 
		}
		if (lookup == '&') {
			finalState = true;
			tokenHolder = "and";
		}
		if (lookup == '!') {
			finalState = true;
			tokenHolder = "not"; 
		}
		if (lookup == '?') {
			finalState = true;
			tokenHolder = "qmark";
		}
		if (lookup == '=') {
			state = 28;
		}
		if (lookup == '<') {
			state = 29;
		}
		if (lookup == '>') {
			state = 30;
		}
		if (lookup == '*') {
			state = 31;
		}
		if (lookup == '/') {
			state = 32;
		}
		if (lookup == ':') {
			state = 33;
		}
		return state;
		break;
	case 2: // B
		if (lookup > -1 || lookup < 10) {}
		if (lookup == '.') {
			state = 6;
		}
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "intnum"; 
			state = 1;
		}
		return state;
		break;
	case 3: // C
		if (lookup == '.') {
			state = 6;
		}
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "intnum"; 
			state = 1;
		}
		return state;
		break;
	case 4: // D
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z') || (lookup > -1 || lookup < 10) || (lookup == '_')) {}
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "id"; 
			state = 1;
		}
		return state;
		break;
	case 5: // E
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z') || (lookup > -1 || lookup < 10) || (lookup == ' ')) {}
		if (lookup == '"') {
			finalState = true;
			tokenHolder = "stringlit"; 
			state = 1;
		}
		else {
			finalState = true;
			tokenHolder = "error";
			state = 1;
		}
		return state;
		break;
	case 6: // F
		if (lookup > -1 || lookup < 10) {
			state = 8;
		}
		else {
			finalState = true;
			tokenHolder = "error";
			state = 1;
		}
		return state;
		break;
	case 8: // H
		if (lookup > 0 || lookup < 10) {}
		if (lookup == 0) {
			state = 9;
		}
		if (lookup == 'e') {
			state = 10;
		}
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "floatnum"; 
			state = 1;
		}
		return state;
		break;
	case 9: // I
		if (lookup > 0 || lookup < 10) {
			state = 8;
		}
		if (lookup == 0) {}
		else {
			finalState = true;
			tokenHolder = "error";
			state = 1;
		}
		return state;
		break;
	case 10: // J
		if (lookup == '+' || lookup == '-') {
			state = 11;
		}
		else {
			finalState = true;
			tokenHolder = "error";
			state = 1;
		}
		return state;
		break;
	case 11: // K
		if (lookup > -1 || lookup < 10) {
			finalState = true;
			needsBacking = false;
			tokenHolder = "floatnum";
			state = 1;
			return state;
			break;
		}
		else {
			finalState = true;
			tokenHolder = "error";
			state = 1;
		}
		return state;
		break;
	case 28: // N1
		// ==
		if (lookup == '=') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "eq"; 
			state = 1;
		}
		// =
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "assign"; 
			state = 1;
		}
		return state;
		break;
	case 29: // N2
		// <=
		if (lookup == '=') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "leq"; 
			state = 1;
		}
		// <>
		if (lookup == '>') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "noteq"; 
			state = 1;
		}
		// <
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "lt"; 
			state = 1;
		}
		return state;
		break;
	case 30: // N3
		// >=
		if (lookup == '=') {
			finalState = true;
			needsBacking = false;
			tokenHolder = " geq"; 
			state = 1;
		}
		// >
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "gt"; 
			state = 1;
		}
		return state;
		break;
	case 31: // N4
		// */
		if (lookup == '/') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "blockcmt"; 
			state = 1;
		}
		// *
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "mult"; 
			state = 1;
		}
		return state;
		break;
	case 32: // N5
		// /*
		if (lookup == '*') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "blockcmt"; 
			state = 1;
		}
		// //
		if (lookup == '/') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "inlinecmt"; 
			state = 1;
		}
		// /
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "div"; 
			state = 1;
		}
		return state;
		break;
	case 33: // N6
		// ::
		if (lookup == ':') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "coloncolon"; 
			state = 1;
		}
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "colon"; 
			state = 1;
		}
		return state;
		break;
	default:
		cout << "No such case";
	}
}

string nextToken() {
	int state = 1;
	string token = "";
	do {
		char lookup = nextChar();
		state = table(state, lookup);
		if (isFinalState())
			token = createToken();
		if (backUp())
			backUpChar();
	} while (token != "");
	return token;
}

