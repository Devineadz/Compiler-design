#include "CodeGenerationVisitor.h"


CodeGenerationVisitor::CodeGenerationVisitor(string path) {
	initializeFile(path);
	tempvarnum = 1;
	moonExecCode = "";
	moonDataCode = "";

	// creating pool of registers to the stack
	for (int i = 12; i >= 1; i--) {
		registerPool.push("r" + to_string(i));
	}

}

void CodeGenerationVisitor::initializeFile(string filename)
{
	string newFileName;
	for (int count = 0; filename[count] != '.'; count++) { // gets the name from the original file name
		newFileName.push_back(filename[count]);
	}
	outputfileName = newFileName + ".moon";

	outputfile.open(outputfileName, ofstream::out | ofstream::trunc); // clear contents from files
	outputfile.close();
}

void CodeGenerationVisitor::visit(EST* node)
{
	
	if (node->getType() == "PROG_NODE") {
		visitP(node);
	}
	if (node->getType() == "FUNC BODY") {
		visitFB(node);
	}
	if (node->getType() == "METHOD BODY") {
		visitMB(node);
	}
	if (node->getType() == "VARLIST") {
		visitVL(node);
	}
	if (node->getType() == "VARDECL") {
		visitV(node);
	}
	if (node->getType() == "STATEMENTLIST") {
		visitSL(node);
	}
	if (node->getType() == "STATEMENT") {
		visitST(node);
	}
	if (node->getType() == "ASSIGNOP") {
		visitAP(node);
	}
	if (node->getType() == "EXPR") {
		visitEX(node);
	}
	if (node->getType() == "ARITHEXPR") {
		visitAE(node);
	}
	if (node->getType() == "WRITE") {
		visitWR(node);
	}
	if (node->getType() == "ADDOP") {
		visitAD(node);
	}

//}
/*
void CodeGenerationVisitor::visit(Vardecl* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
*/
}

void CodeGenerationVisitor::visitP(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
	outputfile.open(outputfileName, ios::app);
	if (outputfile.is_open()) {
		outputfile << moonExecCode << "\n" << moonDataCode << "\n";
	}
	outputfile.close();
}

void CodeGenerationVisitor::visitFB(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
}

void CodeGenerationVisitor::visitMB(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
}

void CodeGenerationVisitor::visitVL(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
}

void CodeGenerationVisitor::visitV(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
	EST* typen = children.at(1);
	if (typen != NULL) {
		vector<EST*>typec = typen->getChildren();
		string type=typec.at(0)->getType();
		if (type == "integer") {
			moonDataCode += moonCodeIndent + "% space for variable " + children.at(0)->getType() + "\n";
			moonDataCode += children.at(0)->getType() + moonCodeIndent2 + "res 4\n";
		}
	}
}

void CodeGenerationVisitor::visitSL(EST* node)
{
	moonExecCode += moonCodeIndent + "entry\n";
	moonExecCode += moonCodeIndent + "addi r14,r0,topaddr\n";
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
	moonDataCode += moonCodeIndent + "% buffer space used for console output\n";
	moonDataCode += "buf        res 20\n";
	moonExecCode += moonCodeIndent + "hlt\n";
}

void CodeGenerationVisitor::visitST(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
}

void CodeGenerationVisitor::visitAP(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
	string val;
	if (children[0] != NULL) {
		val = children[0]->getType();
	}
	string varName = "t" + to_string(tempvarnum);
	tempvarnum++;
	string localRegister = this->registerPool.top();
	registerPool.pop();
	moonExecCode += moonCodeIndent + "% processing: " + val + " := " + varName +"\n";
	moonExecCode += moonCodeIndent + "lw " + localRegister + "," + varName+"(r0)\n";
	moonExecCode += moonCodeIndent + "sw " + val + "(r0)," + localRegister + "\n";
	this->registerPool.push(localRegister);
}

void CodeGenerationVisitor::visitEX(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
}

void CodeGenerationVisitor::visitAE(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
	string val = "";
	if (children[0] != NULL) {
		val = children[0]->getType();
	}
	if (dynamic_cast<Addop*>(children[0]) == nullptr) {
		string localRegister = this->registerPool.top();
		registerPool.pop();
		string varName = "t" + to_string(tempvarnum);
		moonDataCode += moonCodeIndent + "% space for constant " + val + "\n";
		moonDataCode += varName + moonCodeIndent2 + "res 4\n";
		moonExecCode += moonCodeIndent + "% processing: " + varName + " := " + val + "\n";
		moonExecCode += moonCodeIndent + "addi " + localRegister + ",r0," + val + "\n";
		moonExecCode += moonCodeIndent + "sw " + varName + "(r0)," + localRegister + "\n";
		// deallocate register
		this->registerPool.push(localRegister);
	}
}

void CodeGenerationVisitor::visitWR(EST* node)
{
	vector<EST*> children = node->getChildren();
	string val = "";
	if (children[0] != NULL) {
		EST* expr = children[0];
		vector<EST*> expr_c = expr->getChildren();
		if (expr_c[0] != NULL) {
			EST* arithexpr = expr_c[0];
			vector<EST*> arithexpr_c = arithexpr->getChildren();
			val = arithexpr_c[0]->getType();
		}
	}
	string localRegister = registerPool.top();
	registerPool.pop();
	moonExecCode += moonCodeIndent + "% processing: put(" + val + ")\n";
	moonExecCode += moonCodeIndent + "lw " + localRegister + "," + val + "(r0)\n";
	moonExecCode += moonCodeIndent + "% put value on stack\n";
	moonExecCode += moonCodeIndent + "sw -8(r14)," + localRegister + "\n";
	moonExecCode += moonCodeIndent + "% link buffer to stack\n";
	moonExecCode += moonCodeIndent + "addi " + localRegister + ",r0, buf\n";
	moonExecCode += moonCodeIndent + "sw -12(r14)," + localRegister + "\n";
	moonExecCode += moonCodeIndent + "% convert int to string for output\n";
	moonExecCode += moonCodeIndent + "jl r15, intstr\n";
	moonExecCode += moonCodeIndent + "sw -8(r14),r13\n";
	moonExecCode += moonCodeIndent + "% output to console\n";
	moonExecCode += moonCodeIndent + "jl r15, putstr\n";
	this->registerPool.push(localRegister);
}

void CodeGenerationVisitor::visitAD(EST* node)
{
	vector<EST*> children = node->getChildren();
	for (vector<EST*>::iterator it = begin(children); it != end(children); ++it) {
		(*it)->accept(this);
	}
	// Check if vals are id's or ints
	string val;
	string val2;
	if (dynamic_cast<Intnumber*>(children[0]) == nullptr) {
		if (children[0] != NULL) {
			val = children[0]->getType();
		}
	}
	if (dynamic_cast<Intnumber*>(children[1]) == nullptr) {
	}
	else {
		if (children[1] != NULL) {
			val2 = children[1]->getType();
		}
		string localRegister = this->registerPool.top();
		registerPool.pop();
		string varName = "t" + to_string(tempvarnum);
		tempvarnum++;
		moonDataCode += moonCodeIndent + "% space for constant " + val2 + "\n";
		moonDataCode += varName + moonCodeIndent2 + "res 4\n";
		moonExecCode += moonCodeIndent + "% processing: " + varName + " := " + val2 + "\n";
		moonExecCode += moonCodeIndent + "addi " + localRegister + ",r0," + val2 + "\n";
		moonExecCode += moonCodeIndent + "sw " + varName + "(r0)," + localRegister + "\n";
		// deallocate register
		this->registerPool.push(localRegister);
		val2 = varName;
	}
	string newVar = "t" + to_string(tempvarnum);
	string localReg = registerPool.top();
	registerPool.pop();
	string leftChildreg = registerPool.top();
	registerPool.pop();
	string rightChildreg = registerPool.top();
	registerPool.pop();
	moonExecCode += moonCodeIndent + "% processing: " + newVar + " := " + val + " + " + val2 + "\n";
	moonExecCode += moonCodeIndent + "lw " + leftChildreg + "," + val + +"(r0)\n";
	moonExecCode += moonCodeIndent + "lw " + rightChildreg + "," + val2 + +"(r0)\n";
	moonExecCode += moonCodeIndent + "add " + localReg + "," + leftChildreg + "," + rightChildreg + "\n";
	moonDataCode += moonCodeIndent + "% space for " + val + " + " + val2 + "\n";
	moonDataCode += newVar + moonCodeIndent2 + "res 4\n";
	moonExecCode += moonCodeIndent + "sw " + newVar + "(r0)," + localReg + "\n";
	registerPool.push(rightChildreg);
	registerPool.push(leftChildreg);
	registerPool.push(leftChildreg);
}

