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
	parser* fileparser = new parser(file);
	CodeGenerationVisitor* codegenVisitor = new CodeGenerationVisitor(file);
	EST* prog =fileparser->parse();
	cout << "Launching visits";
	prog->accept(codegenVisitor);


}