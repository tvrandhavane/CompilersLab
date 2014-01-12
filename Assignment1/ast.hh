/*********************************************************************************

	            Abstract Syntax Trees for cs306 2013-14
                    Implemented by 
                         - Tanu Kanvar (tanu@cse.iitb.ac.in), and
                         - Uday Khedker (uday@cse.iitb.ac.in) 

*********************************************************************************/

#ifndef AST_HH
#define AST_HH

#include<string>

using namespace std;

class Ast;
typedef Ast * ast_Ptr;
typedef list <ast_Ptr> * ast_List_Ptr;
void print_Ast_List (ast_List_Ptr alist);

enum relational_operators{LE, LT, GT, GE};
enum boolean_operators{AND, OR, NOT};

class Ast
{
public:
	Ast() {}
	~Ast() {}

	virtual void print(ostream & myStream) = 0;

};


class Assignment_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;

public:
	Assignment_Ast(Ast * arg_lhs, Ast * arg_rhs);
	~Assignment_Ast();

	void print(ostream & myStream);

};

class Name_Ast: public Ast
{
	string variable_name;

public:
	Name_Ast(string & name);
	~Name_Ast();

	void print(ostream & myStream);

};


class Number_Ast:public Ast
{
	int constant;

public:
	Number_Ast(int number);
	~Number_Ast();

	void print(ostream &);

};

class Relational_Expr_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;
	relational_operators oper;

public:
	Relational_Expr_Ast(Ast * arg_lhs, relational_operators arg_oper, Ast * arg_rhs);
	~Relational_Expr_Ast();

	void print(ostream & myStream);

};

class Boolean_Expr_Ast:public Ast
{
	Ast * lhs;
	Ast * rhs;
	boolean_operators oper;

public:
	Boolean_Expr_Ast(Ast * arg_lhs, boolean_operators arg_oper, Ast * arg_rhs);
	~Boolean_Expr_Ast();

	void print(ostream & myStream);

};


#endif
