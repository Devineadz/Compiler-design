#include <iostream>
#include <string>
using namespace std;

string tokenHolder = "";
bool finalState = false;
bool needsBacking = false;

int main() {
	
	// loop until end of file
	// currentpos = currentCharPos
	// new_token = nextToken() + currentpos
	// if new_token = error
	//	write to errorlex
	// else
	//	write to lex 

	// table(state, lookup)
	// switch? 
	// state: 1-42 states
	// case n:
	//	if lookup == n char
	//		state = new state
	//	else
	//		error or finalstate
	//		if finalstate
	//			backup = true/false
	// last case: error
	
	return 0;
}

string nextToken() {
	int state = 1;
	string token = "";
	do {
		char lookup = nextChar();
		int state = table(state, lookup);
		if (isFinalState())
			token = createToken();
		if (backUp())
			backUpChar();
	} 
	while (token != "");
	return token;
}

int table(int state, char lookup) {
	switch (state) {
	case 1:
		if ((lookup >= 'a' && lookup <= 'z') || (lookup >= 'A' && lookup <= 'Z'))
			state = 4;
			break;
		if (lookup > 0 && lookup < 10)
			state = 2;
			break;
		if (lookup == 0)
			state = 3;
			break;
		if (lookup == '"')
			state = 5;
			break;
		if (lookup == '+')
			state = 13;
			break;
		if (lookup == '-')
			state = 13;
			break;
		if (lookup == '_')
			state = 42;
			break;
		if (lookup == '.')
			state = 27;
			break;
		if (lookup == '=')
			state = 28;
			break;
		if (lookup == '_')
			state = 42;
			break;
	case 2:
		break;
		return state;
	}
	case 3:
}

char nextChar() {
	// if not in vocab - return non valid vocab error
	//	return nextchar
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