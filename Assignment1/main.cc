/*********************************************************************************

	            Abstract Syntax Trees for cs306 2013-14
                    Implemented by 
                         - Tanu Kanvar (tanu@cse.iitb.ac.in), and
                         - Uday Khedker (uday@cse.iitb.ac.in) 

*********************************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>

using namespace std;

#include "ast.hh"


void print_Ast_List(ast_List_Ptr alist);

string & c_to_cpp_string(const char * name)
{
	
	string * str = new string(name); 
	return * str;
}
	

int main() 
{

	ast_List_Ptr ast_list = new list<ast_Ptr>;

/**** Assignment Statement Number 1  ******/
	ast_Ptr t0 = new Name_Ast(c_to_cpp_string("a"));
	ast_Ptr t1 = new Name_Ast(c_to_cpp_string("b"));
	ast_Ptr a1 = new Assignment_Ast(t0, t1);
	ast_list->push_back(a1);


/**** Assignment Statement Number 2  ******/
	ast_Ptr t2 = new Number_Ast(10);
	ast_Ptr a2 = new Assignment_Ast(t1, t2);
	ast_list->push_back(a2);


	
/**** Assignment Statement Number 3  ******/
        ast_Ptr t3 = new Relational_Expr_Ast(t0, LE, t2);
	ast_Ptr a3 = new Assignment_Ast(t1, t3);
	ast_list->push_back(a3);


/**** Assignment Statement Number 4  ******/
        ast_Ptr t4 = new Relational_Expr_Ast(t0, LT, t1);
	ast_Ptr a4 = new Assignment_Ast(t1, t4);
	ast_list->push_back(a4);

/**** Assignment Statement Number 5  ******/
        ast_Ptr t5 = new Relational_Expr_Ast(t1, GE, t2);
	ast_Ptr a5 = new Assignment_Ast(t0, t5);
	ast_list->push_back(a5);

/**** Assignment Statement Number 6  ******/
        ast_Ptr t6 = new Relational_Expr_Ast(t2, GT, t1);
	ast_Ptr a6 = new Assignment_Ast(t0, t6);
	ast_list->push_back(a6);

   
/**** Assignment Statement Number 7  ******/
        ast_Ptr t7 = new Boolean_Expr_Ast(t5, AND, t6);
	ast_Ptr a7 = new Assignment_Ast(t0, t7);
	ast_list->push_back(a7);
     
/**** Assignment Statement Number 8  ******/
        ast_Ptr t8 = new Boolean_Expr_Ast(t7, OR, t3);
	ast_Ptr a8 = new Assignment_Ast(t0, t8);
	ast_list->push_back(a8);

/**** Assignment Statement Number 9  ******/
        ast_Ptr t9 = new Boolean_Expr_Ast(t6, OR, t5);
	ast_Ptr a9 = new Assignment_Ast(t1, t9);
	ast_list->push_back(a9);
     
/**** Assignment Statement Number 9  ******/
        ast_Ptr t10 = new Boolean_Expr_Ast(t6, NOT, NULL);
	ast_Ptr a10 = new Assignment_Ast(t0, t10);
	ast_list->push_back(a10);
 
/************ Print the list *************/
     print_Ast_List(ast_list);
     return 0;
}

