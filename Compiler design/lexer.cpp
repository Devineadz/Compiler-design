#include "lexer.h"

using namespace std;

lexer::lexer(string path)
{
	vector<string> fileRows = tokenVector(path);
	string lexeme = "";
	string tokenHolder = "";
	bool finalState = false;
	bool needsBacking = false;
	int lineCounter = 0;
	string nextLine = "";
	bool isLast = false;
	bool endFile = false;
}

lexer::~lexer()
{
	delete &fileRows;
}

vector<string> lexer::tokenVector(string path)
{
	fstream srcFile;
	vector<string>newRows;
	int currentRow = 0;
	srcFile.open(path, ios::in);
	if (srcFile.is_open()) {
		string textLine;
		while (getline(srcFile, textLine)) { // stores the source file into a vector
			newRows.push_back(textLine);
		}
		srcFile.close();
	}
	else {
		cout << "Can't open file" << endl;
	}

	return newRows;
}

// Get the next character
char lexer::nextChar() {
		while (nextLine == "") { // loops until it gets a string with characters from the vector
			if (fileRows.empty()) {
				return '+';
				endFile = true;
			}
			lineCounter++;
			nextLine = fileRows.front(); // gets the first string from vector
			fileRows.erase(fileRows.begin()); // removes the first element from vector after consuming
		}
		char c = nextLine.at(0); // gets the first character from string
		lexeme = lexeme + c; // saves the character in a lexeme
		nextLine.erase(0, 1); // erase first character from string after consuming
		if (nextLine == "") { // checks if string is now empty and if the character was last in the string
			isLast = true;
		}
		return c;
}

bool lexer::isFinalState() { // checks if the state is a final state
	if (finalState == true) {
		isLast = false;
		return true;
	}
	else
		return false;
}

string lexer::createToken() { 
	return tokenHolder;
}

bool lexer::backUp() { // checks if backup needed
	if (needsBacking == true)
		return true;
	else
		return false;
}

void lexer::backUpChar() { // removes the last character from lexeme and returns it back to the line string
	nextLine = lexeme.at(lexeme.length() - 1) + nextLine;
	lexeme.erase(lexeme.length() - 1);
}



int lexer::table(int state, char lookup) {
	switch (state) {
	case 1: //A
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z')) { // letters
			if (isLast == true) {
				finalState = true;
				tokenHolder = "id";
			}
			else
				state = 4;
		}
		else if (lookup == '1' || lookup == '2' || lookup == '3' || lookup == '4' || lookup == '5' || lookup == '6' || lookup == '7' || lookup == '8' || lookup == '9') { // non zero
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
		// single character operators
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
		// other operators
		else if (lookup == '=') {
			if (isLast == true) {
				finalState = true;
				tokenHolder = "eq";
			}
			else
				state = 28;
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
	case 2: // B int or float
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
		else { // if next char not an integer or ., returns int
			finalState = true;
			needsBacking = true;
			tokenHolder = "intnum";
			state = 1;
		}
		return state;
		break;
	case 3: // C continues to check for float or int
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
	case 4: // D id 
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
	case 5: // E string literals
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
	case 6: // F float
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
	case 8: // H continue float
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
	case 9: // I float
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
	case 10: // J float
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
	case 11: // K float final
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
			tokenHolder = "geq"; 
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
		else if (isLast == true && fileRows.empty()) {
			finalState = true;
			needsBacking = false;
			tokenHolder = "error";
			state = 1;
		}
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

string lexer::nextToken() { // gets the next token
	int state = 1;
	string token = "";
	do {
		char lookup = nextChar();
		if (endFile == true) {
			token = "";
			return token;
		}
		state = table(state, lookup);
		if (isFinalState())
			token = createToken();
		if (backUp()) {
			backUpChar();
		}
	} while (token == "");
	return token;
}

string lexer::checkReservedWords(string token) { // checks if an id is a reserved word and changes token if it is
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

string lexer::getNextToken() { // reads file and gets the token
	tokenHolder = "";
	finalState = false;
	needsBacking = false;
	lexeme = "";

	while (!fileRows.empty()) {
		string token = nextToken();
		if (token == "error" || token == "invalid character") { // returns error string
			return "[" + token + ", " + lexeme + ", " + to_string(lineCounter) + "]\n" ;
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
				return "[" + token + ", " + lexeme + ", " + to_string(lineCounter) + "]\n";
			}
		}
	}


}

