
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

#include"user-options.hh"
#include"error-display.hh"
#include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"
Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast(int line)
{
	report_internal_error("Should not reach, Ast : check_ast");
}

Data_Type Ast::get_data_type()
{
	report_internal_error("Should not reach, Ast : get_data_type");
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	report_internal_error("Should not reach, Ast : print_value");
}

int Ast::get_successor()
{
	report_internal_error("Should not reach, Ast : get_successor");
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	report_internal_error("Should not reach, Ast : get_value_of_evaluation");
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	report_internal_error("Should not reach, Ast : set_value_of_evaluation");
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs)
{
	lhs = temp_lhs;
	rhs = temp_rhs;
	successor = -1;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

Data_Type Assignment_Ast::get_data_type()
{
	return node_data_type;
}

bool Assignment_Ast::check_ast(int line)
{
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	report_error("Assignment statement data type not compatible", line);
}

void Assignment_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Asgn:\n";

	file_buffer << AST_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")\n";

	file_buffer << AST_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);

	file_buffer << ")";
}

int Assignment_Ast::get_successor(){
	return successor;
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	if (result.is_variable_defined() == false)
		report_error("Variable should be defined to be on rhs", NOLINE);

	lhs->set_value_of_evaluation(eval_env, result);
	// Print the result
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Asgn:\n";

	file_buffer << AST_NODE_SPACE << "LHS (";
	lhs->print_ast(file_buffer);
	file_buffer << ")\n";

	file_buffer << AST_NODE_SPACE << "RHS (";
	rhs->print_ast(file_buffer);
	file_buffer << ")";

	lhs->print_value(eval_env, file_buffer);

	return result;
}
/////////////////////////////////////////////////////////////////
Goto_Ast::Goto_Ast(int temp_bb)
{
	bb = temp_bb;
	successor = temp_bb;
}

Goto_Ast::~Goto_Ast()
{
}

Data_Type Goto_Ast::get_data_type()
{
	return node_data_type;
}

bool Goto_Ast::check_ast(int line)
{
	if (bb > 1)
	{
		return true;
	}

	report_error("Basic block should be greater than one.", line);
}

void Goto_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Goto statement:\n";

	file_buffer << AST_NODE_SPACE << "Successor: " << bb;
	
}

int Goto_Ast::get_successor(){
	return successor;
}

Eval_Result & Goto_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// Print the result
	print_ast(file_buffer);
	Eval_Result & result = *new Eval_Result_Value_Int();
	result.set_value(bb);
	file_buffer << "\n";
	file_buffer << AST_SPACE << "GOTO (BB " << successor << ")\n";
	successor = bb;
	return result;
}
/////////////////////////////////////////////////////////////////
If_Else_Ast::If_Else_Ast(Ast * temp_gotoTrue, Ast * temp_condition, Ast * temp_gotoFalse)
{
	gotoTrue = temp_gotoTrue;
	condition = temp_condition;
	gotoFalse = temp_gotoFalse;
	successor = -3;
}

If_Else_Ast::~If_Else_Ast()
{
	delete gotoTrue;
	delete gotoFalse;
	delete condition;
}

Data_Type If_Else_Ast::get_data_type()
{
	return node_data_type;
}

bool If_Else_Ast::check_ast(int line)
{
	if ((gotoTrue->get_successor() > 1) && (gotoFalse->get_successor() > 1))
	{
		return true;
	}

	report_error("Basic block should be greater than one.", line);
}

void If_Else_Ast::print_ast(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "If_Else statement:";
	condition->print_ast(file_buffer);
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "True Successor: " << gotoTrue->get_successor();
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "False Successor: " << gotoFalse ->get_successor();
	
}

int If_Else_Ast::get_successor(){
	return successor;
}

Eval_Result & If_Else_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// Print the result
	file_buffer << "\n";
	file_buffer << AST_SPACE << "If_Else statement:";
	condition->print_ast(file_buffer);
	Eval_Result & conditionResult = condition->evaluate(eval_env, file_buffer);
	int value = conditionResult.get_value();
	Eval_Result & result = *new Eval_Result_Value_Int();
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "True Successor: " << gotoTrue->get_successor();
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "False Successor: " << gotoFalse ->get_successor();
	file_buffer << "\n";
	if(value == 1){

		file_buffer << AST_SPACE << "Condition True : Goto (BB " << gotoTrue->get_successor() << ")\n";
		successor = gotoTrue->get_successor();
	}
	else{
		file_buffer << AST_SPACE << "Condition False : Goto (BB " << gotoFalse->get_successor() << ")\n";
		successor = gotoFalse->get_successor();
	}
	result.set_value(successor);
	return result;
}
/////////////////////////////////////////////////////////////////
Relational_Expr_Ast::Relational_Expr_Ast(Ast * temp_lhs, relational_operators temp_oper, Ast * temp_rhs)
{
	lhs = temp_lhs;
	rhs = temp_rhs;
	oper = temp_oper;
	node_data_type = int_data_type;
	successor = -1;
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	delete lhs;
	delete rhs;
}

Data_Type Relational_Expr_Ast::get_data_type()
{
	return node_data_type;
}

bool Relational_Expr_Ast::check_ast(int line)
{
	if (lhs->get_data_type() == rhs->get_data_type())
	{

		//change here
		node_data_type = int_data_type;
		return true;
	}

	report_error("Relational expression data type not compatible", line);
}

void Relational_Expr_Ast::print_ast(ostream & file_buffer)
{
		file_buffer << "\n";
		file_buffer << AST_NODE_SPACE << "Condition: ";
		if (oper == LT) {
			file_buffer << "LT\n";
		}
		else if (oper == LE) {
			file_buffer << "LE\n";
		}
		else if (oper == GT) {
			file_buffer << "GT\n";
		}
		else if (oper == GE){
			file_buffer << "GE\n";
		}
		else if (oper == EQ){
			file_buffer << "EQ\n";
		}
		else if (oper == NE){
			file_buffer << "NE\n";
		}
		else if (oper == AND){
			file_buffer << "AND\n";
		}
		else if (oper == OR){
			file_buffer << "OR\n";
		}
		else if (oper == NOT){
			file_buffer << "NOT\n";
		}

		file_buffer << AST_SMALL_SPACE << AST_NODE_SPACE << "LHS (";
		lhs->print_ast(file_buffer);
		file_buffer << ")\n";
		if (oper != NOT){
			file_buffer << AST_SMALL_SPACE << AST_NODE_SPACE << "RHS (";
			rhs->print_ast(file_buffer);
			file_buffer << ")";
		}
}

int Relational_Expr_Ast::get_successor(){
	return successor;
}

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if(oper == GT){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on lhs of condition", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on lhs of condition", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if(lhsResult.get_value() > rhsResult.get_value()){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == LT){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if(lhsResult.get_value() < rhsResult.get_value()){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == GE){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if(lhsResult.get_value() >= rhsResult.get_value()){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == LE){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if(lhsResult.get_value() <= rhsResult.get_value()){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == EQ){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if(lhsResult.get_value() == rhsResult.get_value()){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == NE){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if(lhsResult.get_value() != rhsResult.get_value()){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == AND){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if((lhsResult.get_value()) && (rhsResult.get_value())){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == OR){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if (rhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);

		Eval_Result & result = *new Eval_Result_Value_Int();
		if((lhsResult.get_value()) || (rhsResult.get_value())){
			result.set_value(1);
		}
		else{
			result.set_value(0);
		}
		return result;
	}
	else if(oper == NOT){
		Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
		if (lhsResult.is_variable_defined() == false)
			report_error("Variable should be defined to be on rhs", NOLINE);
		Eval_Result & result = *new Eval_Result_Value_Int();
		if(lhsResult.get_value()){
			result.set_value(0);
		}
		else{
			result.set_value(1);
		}
		return result;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
Arithmetic_Expr_Ast::Arithmetic_Expr_Ast(Ast * temp_lhs, arithmetic_operators temp_oper, Ast * temp_rhs){

	lhs = temp_lhs;
	rhs = temp_rhs;
	oper = temp_oper;
	successor = -1;


}

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast()
{
	delete lhs;
	delete rhs;
}

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	return node_data_type;
}

bool Arithmetic_Expr_Ast::check_ast(int line)
{

	if(rhs != NULL){
		if (lhs->get_data_type() == rhs->get_data_type())
		{
			node_data_type = lhs->get_data_type();
			return true;
		}
	}
	else{
		if(oper == UMINUS){
			node_data_type = lhs->get_data_type();
		}
		else if(oper == I_NUM){
			node_data_type = int_data_type;
		}
		else if(oper == F_NUM){
			node_data_type = float_data_type;
		}
		else if(oper == VAR){
			node_data_type = lhs->get_data_type();
		}
		return true;
	}

	report_error("Arithmetic expression data type not compatible", line);
}

void Arithmetic_Expr_Ast::print_ast(ostream & file_buffer)
{

	if (oper == VAR || oper == F_NUM || oper == I_NUM){
		lhs->print_ast(file_buffer);
	}
	else{
		file_buffer << "\n";
		file_buffer << AST_NODE_SPACE << "Arith: ";
		if (oper == PLUS) {
			file_buffer << "PLUS\n";
		}
		else if (oper == MINUS) {
			file_buffer << "MINUS\n";
		}
		else if (oper == MULT) {
			file_buffer << "MULT\n";
		}
		else if (oper == DIV){
			file_buffer << "DIV\n";
		}
		else if (oper == UMINUS){
			file_buffer << "UMINUS\n";
		}

		file_buffer << AST_SMALL_SPACE << AST_NODE_SPACE << "LHS (";
		lhs->print_ast(file_buffer);
		file_buffer << ")";
		if (oper != I_NUM && oper != F_NUM && oper != UMINUS){
			file_buffer << "\n" << AST_SMALL_SPACE << AST_NODE_SPACE << "RHS (";
			rhs->print_ast(file_buffer);
			file_buffer << ")";
		}
	}
}

int Arithmetic_Expr_Ast::get_successor(){
	return successor;
}

Eval_Result & Arithmetic_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
	if(rhs != NULL){
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		if(lhs->get_data_type() == int_data_type && rhs->get_data_type() == int_data_type){

			Eval_Result & result = *new Eval_Result_Value_Int();
			if(oper == PLUS){

				result.set_value((int) (lhsResult.get_value() + rhsResult.get_value()) );
			}
			if(oper == MINUS){
				result.set_value((int) (lhsResult.get_value() - rhsResult.get_value()) );
			}
			if(oper == DIV){
				if((int) rhsResult.get_value() != 0){
					result.set_value((int) (lhsResult.get_value() / rhsResult.get_value()));
				}
				else{
					report_error("Divided by zero", NOLINE);
				}
			}
			if(oper == MULT){
				result.set_value((int) (lhsResult.get_value() * rhsResult.get_value()) );
			}
			return result;
		}
		else if(lhs->get_data_type() == float_data_type && rhs->get_data_type() == float_data_type){

			Eval_Result & result = *new Eval_Result_Value_Float();
			if(oper == PLUS){
				result.set_value((float) (lhsResult.get_value() + rhsResult.get_value()) );
			}
			if(oper == MINUS){
				result.set_value((float) (lhsResult.get_value() - rhsResult.get_value()) );
			}
			if(oper == DIV){
				if(rhsResult.get_value() != 0){
					result.set_value((float) (lhsResult.get_value() / rhsResult.get_value()) );
				}
				else{
					report_error("Divided by zero", NOLINE);
				}
			}
			if(oper == MULT){
				result.set_value((float) (lhsResult.get_value() * rhsResult.get_value()) );
			}
			return result;
		}
	}
	else{
		if(oper == F_NUM){
			Eval_Result_Value_Float & result = *new Eval_Result_Value_Float();
			//file_buffer << fixed << setprecision(2) << lhsResult.get_value();
			result.set_value((float) lhsResult.get_value());
			return result;
		}
		else if(lhs->get_data_type() == int_data_type){
			Eval_Result_Value_Int & result = *new Eval_Result_Value_Int();
			if(oper ==  UMINUS){
				result.set_value((int) -1*lhsResult.get_value());
			}
			if(oper == VAR){
				result.set_value((int) lhsResult.get_value());
			}
			if(oper == I_NUM){
				result.set_value((int) lhsResult.get_value());
			}
			return result;
		}
		else{
			Eval_Result_Value_Float & result = *new Eval_Result_Value_Float();
			if(oper ==  UMINUS){
				result.set_value((float) -1*lhsResult.get_value());
			}
			if(oper == VAR){
				result.set_value((float) lhsResult.get_value());
			}
			if(oper == I_NUM){
				result.set_value((float) lhsResult.get_value());
			}
			return result;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////


Function_Call_Ast::Function_Call_Ast(string name , Data_Type return_data_type, list <Ast *> * input_list){

	func_name = name;
	input_argument_list = input_list;
	successor = -1;
	node_data_type = return_data_type;

}

Function_Call_Ast::~Function_Call_Ast()
{

}

Data_Type Function_Call_Ast::get_data_type()
{
	return node_data_type;
}

bool Function_Call_Ast::check_ast(int line)
{

}

void Function_Call_Ast::print_ast(ostream & file_buffer)
{

		file_buffer<<"\n";
		file_buffer << AST_SPACE << "FN CALL: ";
		list<Ast *>::iterator i;
		if(input_argument_list != NULL){
			//file_buffer << AST_NODE_SPACE 	;
			file_buffer <<func_name << "(";
			for(i = input_argument_list->begin(); i != input_argument_list->end(); i++){
				file_buffer<<"\n";
				file_buffer << AST_NODE_SPACE 	;
				(*i)->print_ast(file_buffer);

			}
		}
		else
			file_buffer <<func_name << "(";
		file_buffer << ")";

}

int Function_Call_Ast::get_successor(){
	return successor;
}
Eval_Result & Function_Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

	
	Procedure *funct_call = program_object.get_procedure(func_name);
	Argument_Table & arg = funct_call->get_argument_symbol_table();
	Local_Environment & func_eval_env = *new Local_Environment();

	//arg.create(func_eval_env);
	list<Ast *>::iterator i;
	if(input_argument_list!=NULL){
		list<Symbol_Table_Entry *>::iterator j;
		i = input_argument_list->begin();
		j = arg.variable_table.begin();
		for(;i != input_argument_list->end();i++,j++){
			Eval_Result_Value * k;
			string variable_name = (*j)->get_variable_name();
			Eval_Result & result = (*i)->evaluate(eval_env, file_buffer);
			if (result.get_result_enum() == int_result && (*j)->get_data_type() == int_data_type)
			{
				k = new Eval_Result_Value_Int();
			 	k->set_value((int)result.get_value());
				func_eval_env.put_variable_value(*k, variable_name);

			}
			else if(result.get_result_enum() == float_result && (*j)->get_data_type() == float_data_type){
				k = new Eval_Result_Value_Float();
			 	k->set_value(result.get_value());
					func_eval_env.put_variable_value(*k, variable_name);

			}
			else{
				report_error("Passed argument is not compatible",NOLINE);
			}
		}
	}
	Eval_Result & return_result = funct_call->evaluate(func_eval_env,file_buffer);
	return return_result;
}
















/////////////////////////////////////////////////////////////////
Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry)
{
	variable_name = name;
	variable_symbol_entry = var_entry;
	successor = -1;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return variable_symbol_entry.get_data_type();
}

void Name_Ast::print_ast(ostream & file_buffer)
{
	//printf("arith_ast  ");
	file_buffer << "Name : " << variable_name;
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result_Value * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result_Value * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << "\n" << AST_SPACE << variable_name << " : ";

	if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
		file_buffer << "undefined";

	else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
		if (loc_var_val->get_result_enum() == int_result)
			file_buffer << (int) loc_var_val->get_value() << "\n";
		else if(loc_var_val->get_result_enum() == float_result)
			file_buffer << fixed << setprecision(2) << loc_var_val->get_value() << "\n";
		else
			report_internal_error("Result type can only be int and float");
	}

	else
	{
		if (glob_var_val->get_result_enum() == int_result)
		{
			if (glob_var_val == NULL)
				file_buffer << "0\n";
			else
				file_buffer << (int) glob_var_val->get_value() << "\n";
		}
		else if(glob_var_val->get_result_enum() == float_result){
			if (glob_var_val == NULL)
				file_buffer << "0\n";
			else
				file_buffer << fixed << setprecision(2) << glob_var_val->get_value() << "\n";
		}
		else
			report_internal_error("Result type can only be int and float");
	}
	file_buffer << "\n";
}

int Name_Ast::get_successor(){
	return successor;
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	if (eval_env.does_variable_exist(variable_name))
	{
		Eval_Result * result = eval_env.get_variable_value(variable_name);
		return *result;
	}

	Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
	return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	Eval_Result_Value * i;
	if (result.get_result_enum() == int_result)
	{
		i = new Eval_Result_Value_Int();
	 	i->set_value((int)result.get_value());

		if (eval_env.does_variable_exist(variable_name))
			eval_env.put_variable_value(*i, variable_name);
		else
			interpreter_global_table.put_variable_value(*i, variable_name);
	}
	else if(result.get_result_enum() == float_result){
		i = new Eval_Result_Value_Float();
	 	i->set_value(result.get_value());

		if (eval_env.does_variable_exist(variable_name))
			eval_env.put_variable_value(*i, variable_name);
		else
			interpreter_global_table.put_variable_value(*i, variable_name);

	}
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	return get_value_of_evaluation(eval_env);
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type)
{
	constant = number;
	node_data_type = constant_data_type;
	successor = -1;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print_ast(ostream & file_buffer)
{
	if(node_data_type == int_data_type)
		file_buffer << "Num : " << constant;
	else
		file_buffer << "Num : " << fixed << setprecision(2) << constant;
}

template <class DATA_TYPE>
int Number_Ast<DATA_TYPE>::get_successor(){
	return successor;
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_value(constant);

		return result;
	}
	if(node_data_type == float_data_type){

		Eval_Result & result = *new Eval_Result_Value_Float();
		result.set_value(constant);

		return result;

	}
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast * to_return)
{
	return_Ast = to_return;
	successor = -2;
	if(return_Ast == NULL){
		node_data_type = void_data_type;
	}
}

Return_Ast::~Return_Ast()
{}

int Return_Ast::get_successor(){
	return successor;
}

Data_Type Return_Ast::get_data_type()
{
	return node_data_type;
}
bool Return_Ast::check_ast(int line){
	if(return_Ast == NULL){
		node_data_type = void_data_type;
	}
}
void Return_Ast::print_ast(ostream & file_buffer)
{
	if(return_Ast == NULL){
		file_buffer <<"\n";
		file_buffer << AST_SPACE << "RETURN <NOTHING>\n";
	}
	else{
		file_buffer <<"\n";
		file_buffer << AST_SPACE << "RETURN ";
		return_Ast->print_ast(file_buffer);
		file_buffer<<"\n";
	}
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	
	if(return_Ast!=NULL){
		Eval_Result & result = return_Ast->evaluate(eval_env,file_buffer);
		print_ast(file_buffer);
		return result;
	}
	else{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_result_enum(void_result);
		print_ast(file_buffer);
		return result;
	}
}

template class Number_Ast<int>;

template class Number_Ast<float>;
