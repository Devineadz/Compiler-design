#include "Prog_node.h"
#include "Visitor.h"

void Prog_node::accept(Visitor& visitor)
{
	cout << "Visiting node";
	visitor.visit(this);
}
