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
}