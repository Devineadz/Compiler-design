/*
Compiler Design
Diana Zitting-Rioux
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "parser.h"
#include "EST.h"
#include "CodeGenerationVisitor.h"

using namespace std;

int main(int argc, char* argv[])
{

	string file = "Test_first.src";
	// Start parsing
	parser* fileparser = new parser(file);
	parser* errorparser = new parser("Test_error.src");
	// Create visitor
	CodeGenerationVisitor* codegenVisitor = new CodeGenerationVisitor(file);
	cout << "Parsing Text_first.src\n";
	EST* prog =fileparser->parse();
	cout << "Parsing Test_error.src\n";
	EST* prog2 = errorparser->parse();
	cout << "Launching visits";
	// Create source code
	prog->accept(codegenVisitor);


}