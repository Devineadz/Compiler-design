#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "parser.h"

int main(int argc, char* argv[])
{
	string file = "polynomial.src";
	parser* fileparser = new parser(file);
	fileparser->parse();

	string file2 = "bubblesort.src";
	parser* fileparser2 = new parser(file2);
	fileparser2->parse();

	string file3 = "test.src";
	parser* fileparser3 = new parser(file3);
	fileparser3->parse();

	string file4 = "test2.src";
	parser* fileparser4 = new parser(file4);
	fileparser4->parse();

	string file5 = "test3.src";
	parser* fileparser5 = new parser(file5);
	fileparser5->parse();

	string file6 = "Test_error.src";
	parser* fileparser6 = new parser(file6);
	fileparser6->parse();

	string file7 = "test_error2.src";
	parser* fileparser7 = new parser(file7);
	fileparser7->parse();

}