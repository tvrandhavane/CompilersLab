/*********************************************************************************

	            Abstract Syntax Trees for cs306 2013-14
                    Implemented by 
                         - Tanu Kanvar (tanu@cse.iitb.ac.in), and
                         - Uday Khedker (uday@cse.iitb.ac.in) 

*********************************************************************************/

#include<iostream>
#include<fstream>
#include <cstdlib>
#include <stdlib.h>
#include<list>

using namespace std;

#include"ast.hh"

Assignment_Ast::Assignment_Ast(Ast * arg_lhs, Ast * arg_rhs)
{
	lhs = arg_lhs;
	rhs = arg_rhs;
}

void Assignment_Ast::print(ostream & myStream){
	lhs->print(myStream);
	myStream << " = ";
	rhs->print(myStream);	
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name)
{
	variable_name = name;
}

Name_Ast::~Name_Ast()
{
	
}

void Name_Ast::print(ostream & myStream){
	myStream << variable_name;
}


///////////////////////////////////////////////////////////////////////////////

Number_Ast::Number_Ast(int number)
{
	constant = number;
}

Number_Ast::~Number_Ast()
{
}

void Number_Ast::print(ostream & myStream){
	myStream << constant;
}
///////////////////////////////////////////////////////////////////////////////


Relational_Expr_Ast::Relational_Expr_Ast(Ast * arg_lhs, relational_operators arg_oper, Ast * arg_rhs)
{
	lhs = arg_lhs;
	rhs = arg_rhs;
	oper = arg_oper;
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	delete lhs;
	delete rhs;
}

void Relational_Expr_Ast::print(ostream & myStream){
	myStream << "(";
	lhs->print(myStream);
	if (oper == LT) {
		myStream << " < ";	
	}
	else if (oper == LE) {
		myStream << " <= ";	
	}
	else if (oper == GT) {
		myStream << " > ";	
	}
	else {
		myStream << " >= ";	
	}
	rhs->print(myStream);
	myStream << ")";
}
///////////////////////////////////////////////////////////////////////////////
Boolean_Expr_Ast::Boolean_Expr_Ast(Ast * arg_lhs, boolean_operators arg_oper, Ast * arg_rhs)
{
	lhs = arg_lhs;
	rhs = arg_rhs;
	oper = arg_oper;
}

Boolean_Expr_Ast::~Boolean_Expr_Ast()
{
	delete lhs;
	delete rhs;
}

void Boolean_Expr_Ast::print(ostream & myStream){
	myStream << "(";	
	if (oper == AND) {
		lhs->print(myStream);
		myStream << "&&";
		rhs->print(myStream);	
	}
	else if (oper == OR) {
		lhs->print(myStream);
		myStream << "||";
		rhs->print(myStream);
	}
	else{
		myStream << "!";
		lhs->print(myStream);
	}	
	myStream << ")";
}
///////////////////////////////////////////////////////////////////////////////

void print_Ast_List(ast_List_Ptr alist)
{
	list<ast_Ptr>::iterator i;

	int line=1;

	for (i=alist->begin(); i != alist->end(); i++)
        {
		cout << line++ << ": ";
        	(*i)->print(cout);
		cout << "\n";
        }

	cout << "\n";
}

