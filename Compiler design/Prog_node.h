#pragma once

#include <iostream>
#include "EST.h"
class Visitor;

using namespace std;

class Prog_node : public EST {
public:
	Prog_node() {
		parent = NULL;
		right_sibling = NULL;
		leftmost_sibling = NULL;
		left_child = NULL;
		type = "PROG_NODE";
	}
	void accept(Visitor& visitor);
};