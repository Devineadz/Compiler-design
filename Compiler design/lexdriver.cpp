#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

vector<string> fileRows;
string lexeme = "";
string tokenHolder = "";
bool finalState = false;
bool needsBacking = false;
int lineCounter = 0;
string nextLine = ""; 
bool isLast = false;

char nextChar() {
		while (nextLine == "") {
			lineCounter++;
			nextLine = fileRows.front();
			fileRows.erase(fileRows.begin()); 
		}
		char c = nextLine.at(0);
		lexeme = lexeme + c;
		nextLine.erase(0, 1);
		if (nextLine == "") {
			isLast = true;
		}
		return c;
}

bool isFinalState() {
	if (finalState == true) {
		isLast = false;
		return true;
	}
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
	nextLine = lexeme.at(lexeme.length() - 1) + nextLine;
	lexeme.erase(lexeme.length() - 1);
}



int table(int state, char lookup) {
	switch (state) {
	case 1: //A
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z')) {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "id";
			}
			else
				state = 4;
		}
		else if (lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "int";
			}
			else
				state = 2;
		}
		else if (lookup == '0') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "int";
			}
			state = 3;
		}
		else if (lookup == '"') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "stringlit";
			}
			else
				state = 5;
		}
		else if (lookup == '+') {
			finalState = true;
			tokenHolder = "plus";
		}
		else if (lookup == '-') {
			finalState = true;
			tokenHolder = "minus";
		}
		else if (lookup == '_') { // error, cannot start with underscore
			finalState = true;
			tokenHolder = "error";
		}
		else if (lookup == '.') {
			finalState = true;
			tokenHolder = "dot";
		}
		else if (lookup == '=') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "eq";
			}
			else
				state = 28;
		}
		else if (lookup == '(') {
			finalState = true;
			tokenHolder = "openpar";
		}
		else if (lookup == ')') {
			finalState = true;
			tokenHolder = "closepar";
		}
		else if (lookup == '{') {
			finalState = true;
			tokenHolder = "opencubr";
		}
		else if (lookup == '}') {
			finalState = true;
			tokenHolder = "closecubr";
		}
		else if (lookup == '[') {
			finalState = true;
			tokenHolder = "opensqbr";
		}
		else if (lookup == ']') {
			finalState = true;
			tokenHolder = "closesqbr";
		}
		else if (lookup == ';') {
			finalState = true;
			tokenHolder = "semi";
		}
		else if (lookup == ',') {
			finalState = true;
			tokenHolder = "comma";
		}
		else if (lookup == '|') {
			finalState = true;
			tokenHolder = "or";
		}
		else if (lookup == '&') {
			finalState = true;
			tokenHolder = "and";
		}
		else if (lookup == '!') {
			finalState = true;
			tokenHolder = "not";
		}
		else if (lookup == '?') {
			finalState = true;
			tokenHolder = "qmark";
		}
		else if (lookup == '<') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "lt";
			}
			else
				state = 29;
		}
		else if (lookup == '>') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "gt";
			}
			else
				state = 30;
		}
		else if (lookup == '*') {
			finalState = true;
			tokenHolder = "mult";
		}
		else if (lookup == '/') {
			state = 32;
		}
		else if (lookup == ':') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "colon";
			}
			else
				state = 33;
		}
		else if (lookup == ' ' || lookup == '\t') { // to ignore spaces and tabs
			finalState = true;
			tokenHolder = "empty";
		}
		else {
			finalState = true;
			tokenHolder = "invalid character";
		}
		return state;
		break;
	case 2: // B
		if (lookup == '0' || lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "intnum";
			}
			else
				state = 2;
		}
		else if (lookup == '.') {
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
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z') || lookup == '0' || lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9' || (lookup == '_')) {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "id";
			}
		}
		else {
			finalState = true;
			needsBacking = true;
			tokenHolder = "id";
			state = 1;
		}
		return state;
		break;
	case 5: // E
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z') || lookup == '0' || lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9' || (lookup == ' ')) {}
		else if (lookup == '"') {
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
		if (lookup == '0' || lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') {
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
		if (lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') {}
		else if (lookup == '0') {
			state = 9;
		}
		else if (lookup == 'e') {
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
		if (lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') {
			state = 8;
		}
		else if (lookup == '0') {}
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
		else if (lookup == '0' || lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') {
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
	case 11: // K
		if (lookup == '0' || lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') {
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
		else if (lookup == '>') {
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
			state = 39;
		}
		// //
		else if (lookup == '/') {
			state = 40;
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
	case 39: // block comment
		if (lookup == '*')
			state = 47;
		return state;
		break;
	case 40: //inline comment
		if (isLast == true) {
			finalState = true;
			needsBacking = false;
			tokenHolder = "inlinecmt";
			state = 1;
		}
		return state;
		break;
	case 47: // end comment
		if (lookup == '/') {
			finalState = true;
			needsBacking = false;
			tokenHolder = "blockcmt";
			state = 1;
		}
		else {
			finalState = true;
			needsBacking = false;
			tokenHolder = "error";
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
		if (backUp()) {
			backUpChar();
		}
	} while (token == "");
	return token;
}

string checkReservedWords(string token) {
	string newToken = token;
	if (lexeme == "if") {
		newToken = "if";
	}
	else if (lexeme == "private") {
		newToken = "private";
	}
	else if (lexeme == "public") {
		newToken = "public";
	}
	else if (lexeme == "func") {
		newToken = "func";
	}
	else if (lexeme == "then") {
		newToken = "then";
	}
	else if (lexeme == "read") {
		newToken = "read";
	}
	else if (lexeme == "write") {
		newToken = "write";
	}
	else if (lexeme == "class") {
		newToken = "class";
	}
	else if (lexeme == "var") {
		newToken = "var";
	}
	else if (lexeme == "else") {
		newToken = "else";
	}
	else if (lexeme == "integer") {
		newToken = "integer";
	}
	else if (lexeme == "main") {
		newToken = "main";
	}
	else if (lexeme == "inherits") {
		newToken = "inherits";
	}
	else if (lexeme == "string") {
		newToken = "string";
	}
	else if (lexeme == "while") {
		newToken = "while";
	}
	else if (lexeme == "void") {
		newToken = "void";
	}
	else if (lexeme == "continue") {
		newToken = "continue";
	}
	else if (lexeme == "float") {
		newToken = "float";
	}
	else if (lexeme == "break") {
		newToken = "break";
	}
	else if (lexeme == "return") {
		newToken = "return";
	}
	return newToken;
}

void createTokenFile(string fileName) {
	fstream srcFile;
	fstream targetFile;
	fstream errorFile;
	string newFileName;
	for (int count = 0; fileName[count] != '.'; count++){
		newFileName.push_back(fileName[count]);
		
	}
	
	int currentRow = 0;
	srcFile.open("test.text", ios::in);
	if (srcFile.is_open()) {
		string textLine;
		while (getline(srcFile, textLine)) { // 
			fileRows.push_back(textLine);
		}
		srcFile.close();
	}
	else {
		cout << "Can't open file" << endl;
	}

	string ErroName = newFileName + ".outlexerrors";
	newFileName = newFileName + ".outlextokens";

	while (!fileRows.empty()) {
		string token = nextToken();
		if (currentRow < lineCounter) {
			cout << "\n";
			currentRow++;
		}
		if (token == "error" || token == "invalid character") {
			cout << "[" << token << ", " << lexeme << ", " << lineCounter << "]";
		}
		else {
			if (token != "empty") { // check the list of reserved words and rename token if found in list
				if (token == "id") {
					token = checkReservedWords(token);
				}
				if (token == "stringlit") { //remove "" from strings
					lexeme.erase(0, 1);
					lexeme.erase(lexeme.length() - 1, lexeme.length());
				}
				if (token == "inlinecmt") { // remove // from inline comments
					lexeme.erase(0, 1);
					lexeme.erase(0, 1);
				}
				if (token == "blockcmt") { // remove /**/ from block comments
					lexeme.erase(0, 1);
					lexeme.erase(0, 1);
					lexeme.erase(lexeme.length() - 1, lexeme.length());
					lexeme.erase(lexeme.length() - 1, lexeme.length());
				}
				cout << "[" << token << ", " << lexeme << ", " << lineCounter << "]";

			}
		}
		tokenHolder = "";
		finalState = false;
		needsBacking = false;
		lexeme = "";
	}

}


int main() {
	string testFile1 = "test.text";
	createTokenFile(testFile1);

	return 0;
}
