
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

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_value() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

int Ast::get_successor()
{
	stringstream msg;
	msg << "No get_successor() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	stringstream msg;
	msg << "No get_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	stringstream msg;
	msg << "No set_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}
/////////////////////////////////////////////////////////////////
Relational_Expr_Ast::Relational_Expr_Ast(Ast * temp_lhs, relational_operators temp_oper, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;
	oper = temp_oper;
	node_data_type = int_data_type;
	successor = -1;

	ast_num_child = binary_arity;

	lineno = line;
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

bool Relational_Expr_Ast::check_ast()
{
	if (lhs->get_data_type() == rhs->get_data_type())
	{

		node_data_type = int_data_type;
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Relational expression statement data type not compatible");
}

void Relational_Expr_Ast::print(ostream & file_buffer)
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
		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		lhs->print(file_buffer);
		file_buffer << ")\n";
		file_buffer << AST_SUB_NODE_SPACE << "RHS (";
		rhs->print(file_buffer);
		file_buffer << ")";
}

int Relational_Expr_Ast::get_successor(){
	return successor;
}

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Relational_Expr_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Relational_Expr_Ast cannot be null");

	Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
	Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(lhsResult.is_variable_defined(), "Variable should be defined to be on lhs of Relational_Expr_Ast", lineno);
	CHECK_INPUT_AND_ABORT(rhsResult.is_variable_defined(), "Variable should be defined to be on rhs of Relational_Expr_Ast", lineno);
	Eval_Result & result = *new Eval_Result_Value_Int();
	float lhs_val, rhs_val;
	if(lhsResult.get_result_enum() == int_result)
		lhs_val = lhsResult.get_int_value();
	else
		lhs_val = lhsResult.get_float_value();

	if(rhsResult.get_result_enum() == int_result)
		rhs_val = rhsResult.get_int_value();
	else
		rhs_val = rhsResult.get_float_value();
	if(oper == GT){
		if(lhs_val > rhs_val){
			result.set_value_int(1);
		}
		else{
			result.set_value_int(0);
		}
		return result;
	}
	else if(oper == LT){
		if(lhs_val < rhs_val){
			result.set_value_int(1);
		}
		else{
			result.set_value_int(0);
		}
		return result;
	}
	else if(oper == GE){
		if(lhs_val >= rhs_val){
			result.set_value_int(1);
		}
		else{
			result.set_value_int(0);
		}
		return result;
	}
	else if(oper == LE){
		if(lhs_val <= rhs_val){
			result.set_value_int(1);
		}
		else{
			result.set_value_int(0);
		}
		return result;
	}
	else if(oper == EQ){
		if(lhs_val == rhs_val){
			result.set_value_int(1);
		}
		else{
			result.set_value_int(0);
		}
		return result;
	}
	else if(oper == NE){
		if(lhs_val != rhs_val){
			result.set_value_int(1);
		}
		else{
			result.set_value_int(0);
		}
		return result;
	}
}
Code_For_Ast & Relational_Expr_Ast::compile()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Relational_Expr_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Relational_Expr_Ast cannot be null");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->used_for_expr_result = true;

	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	rhs_register->used_for_expr_result = true;

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Compute_IC_Stmt *compute_stmt;
	if(oper == GT){
		compute_stmt = new Compute_IC_Stmt(sgt, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == LT){
		compute_stmt = new Compute_IC_Stmt(slt, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == GE){
		compute_stmt = new Compute_IC_Stmt(sge, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == LE){
		compute_stmt = new Compute_IC_Stmt(sle, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == EQ){
		compute_stmt = new Compute_IC_Stmt(seq, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == NE){
		compute_stmt = new Compute_IC_Stmt(sne, lhs_opd, rhs_opd, register_opd);
	}

	// Store the statement in ic_list
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	ic_list.push_back(compute_stmt);

	Code_For_Ast * relational_stmt;
	if (ic_list.empty() == false)
		relational_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->used_for_expr_result = false;
	rhs_register->used_for_expr_result = false;
	return *relational_stmt;
}

Code_For_Ast & Relational_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Relational_Expr_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Relational_Expr_Ast cannot be null");


	if((typeid(*lhs) != typeid(Relational_Expr_Ast)) && 
		(typeid(*lhs) != typeid(Arithmetic_Expr_Ast)) &&
		(typeid(*lhs) != typeid(Function_Call_Ast))){
		lra.optimize_lra(mc_2r, NULL, lhs);
	}

	Code_For_Ast & lhs_stmt = lhs->compile_and_optimize_ast(lra);
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->used_for_expr_result = true;

	if((typeid(*rhs) != typeid(Relational_Expr_Ast)) && 
		(typeid(*rhs) != typeid(Arithmetic_Expr_Ast)) &&
		(typeid(*rhs) != typeid(Function_Call_Ast))){
		lra.optimize_lra(mc_2r, NULL, rhs);
	}

	Code_For_Ast & rhs_stmt = rhs->compile_and_optimize_ast(lra);
	Register_Descriptor * rhs_register = rhs_stmt.get_reg();
	rhs_register->used_for_expr_result = true;

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();

	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Compute_IC_Stmt *compute_stmt;
	if(oper == GT){
		compute_stmt = new Compute_IC_Stmt(sgt, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == LT){
		compute_stmt = new Compute_IC_Stmt(slt, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == GE){
		compute_stmt = new Compute_IC_Stmt(sge, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == LE){
		compute_stmt = new Compute_IC_Stmt(sle, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == EQ){
		compute_stmt = new Compute_IC_Stmt(seq, lhs_opd, rhs_opd, register_opd);
	}
	else if(oper == NE){
		compute_stmt = new Compute_IC_Stmt(sne, lhs_opd, rhs_opd, register_opd);
	}

	// Store the statement in ic_list
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

	ic_list.push_back(compute_stmt);

	Code_For_Ast * relational_stmt;
	if (ic_list.empty() == false)
		relational_stmt = new Code_For_Ast(ic_list, result_register);

	lhs_register->used_for_expr_result = false;
	rhs_register->used_for_expr_result = false;
	return *relational_stmt;
}
////////////////////////////////////////////////////////////////
Arithmetic_Expr_Ast::Arithmetic_Expr_Ast(Ast * temp_lhs, arithmetic_operators temp_oper, Ast * temp_rhs, int line){

	lhs = temp_lhs;
	rhs = temp_rhs;
	oper = temp_oper;
	successor = -1;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;


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

bool Arithmetic_Expr_Ast::check_ast()
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
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,"Arithmetic expression data type not compatible");
}

void Arithmetic_Expr_Ast::print(ostream & file_buffer)
{
	if (oper == F_NUM || oper == I_NUM){
		CHECK_INVARIANT((lhs != NULL), "Lhs of Arithmetic_Expr_Ast cannot be null");
		lhs->print(file_buffer);
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

		file_buffer << AST_SUB_NODE_SPACE << "LHS (";
		CHECK_INVARIANT((lhs != NULL), "Lhsof Arithmetic_Expr_Ast cannot be null");
		lhs->print(file_buffer);
		file_buffer << ")";
		if (oper != I_NUM && oper != F_NUM && oper != UMINUS){
			file_buffer << "\n" << AST_SUB_NODE_SPACE << "RHS (";
			CHECK_INVARIANT((rhs != NULL), "Rhs of Arithmetic_Expr_Ast cannot be null");
			rhs->print(file_buffer);
			file_buffer << ")";
		}
	}
}

int Arithmetic_Expr_Ast::get_successor(){
	return successor;
}

Eval_Result & Arithmetic_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	CHECK_INVARIANT((lhs != NULL), "Lhs of Arithmetic_Expr_Ast cannot be null");
	Eval_Result & lhsResult = lhs->evaluate(eval_env, file_buffer);
	if(rhs != NULL){
		Eval_Result & rhsResult = rhs->evaluate(eval_env, file_buffer);
		CHECK_INVARIANT((rhs != NULL), "Rhs of Arithmetic_Expr_Ast cannot be null");
		if(lhs->get_data_type() == int_data_type && rhs->get_data_type() == int_data_type){

			Eval_Result & result = *new Eval_Result_Value_Int();
			if(oper == PLUS){
				int lh_result = lhsResult.get_int_value();
				int rh_result = rhsResult.get_int_value();
				int res = lh_result + rh_result;
				result.set_value_int(res);
			}
			if(oper == MINUS){
				int lh_result = lhsResult.get_int_value();
				int rh_result = rhsResult.get_int_value();
				int res = lh_result - rh_result;
				result.set_value_int(res);
			}
			if(oper == DIV){
				if(rhsResult.get_int_value() != 0){
					int lh_result = lhsResult.get_int_value();
					int rh_result = rhsResult.get_int_value();
					int res = lh_result / rh_result;
					result.set_value_int(res);
				}
				else{
					CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Division by zero");
				}
			}
			if(oper == MULT){
				int lh_result = lhsResult.get_int_value();
				int rh_result = rhsResult.get_int_value();
				int res = lh_result * rh_result;
				result.set_value_int(res);
			}
			return result;
		}
		else if(lhs->get_data_type() == float_data_type && rhs->get_data_type() == float_data_type){

			Eval_Result & result = *new Eval_Result_Value_Float();
			if(oper == PLUS){
				result.set_value_float(lhsResult.get_float_value() + rhsResult.get_float_value() );
			}
			if(oper == MINUS){
				result.set_value_float(lhsResult.get_float_value() - rhsResult.get_float_value() );
			}
			if(oper == DIV){
				if(rhsResult.get_float_value() != 0){
					result.set_value_float(lhsResult.get_float_value() / rhsResult.get_float_value());
				}
				else{
					CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Division by zero");
				}
			}
			if(oper == MULT){
				result.set_value_float(lhsResult.get_float_value() * rhsResult.get_float_value());
			}
			return result;
		}
	}
	else{
		if(oper == F_NUM){
			if(lhs->get_data_type() == int_data_type){
				Eval_Result_Value_Float & result = *new Eval_Result_Value_Float();
				//file_buffer << fixed << setprecision(2) << lhsResult.get_value();
				result.set_value_float((float)lhsResult.get_int_value());
				return result;
			}
			else{
				return lhsResult;
			}
		}
		else if(oper == I_NUM){
			if(lhs->get_data_type() == float_data_type){
				Eval_Result_Value_Int & result = *new Eval_Result_Value_Int();
				result.set_value_int((int)lhsResult.get_float_value());
				return result;
			}
			else{
				return lhsResult;
			}
		}
		else if(lhs->get_data_type() == int_data_type){
			Eval_Result_Value_Int & result = *new Eval_Result_Value_Int();
			if(oper ==  UMINUS){
				result.set_value_int(-1*lhsResult.get_int_value());
			}
			return result;
		}
		else{
			Eval_Result_Value_Float & result = *new Eval_Result_Value_Float();
			if(oper ==  UMINUS){
				result.set_value_float(-1*lhsResult.get_float_value());
			}
			return result;
		}
	}
}

Code_For_Ast & Arithmetic_Expr_Ast::compile()
{
	CHECK_INVARIANT((lhs != NULL), "Lhs of Arithmetic_Expr_Ast cannot be null");

	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->used_for_expr_result = true;

	if(rhs != NULL){
		CHECK_INVARIANT((rhs != NULL), "Rhs of Arithmetic_Expr_Ast cannot be null");

		Code_For_Ast & rhs_stmt = rhs->compile();
		Register_Descriptor * rhs_register = rhs_stmt.get_reg();
		rhs_register->used_for_expr_result = true;

		Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
		Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);

		if(lhs->get_data_type() == int_data_type && rhs->get_data_type() == int_data_type){
			Register_Descriptor * result_register = machine_dscr_object.get_new_register();
			Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

			Compute_IC_Stmt *compute_stmt;
			if(oper == PLUS){
				compute_stmt = new Compute_IC_Stmt(add, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MINUS){
				compute_stmt = new Compute_IC_Stmt(sub, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == DIV){
				compute_stmt = new Compute_IC_Stmt(div_op, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MULT){
				compute_stmt = new Compute_IC_Stmt(mul, lhs_opd, rhs_opd, register_opd);
			}

			list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

			if (lhs_stmt.get_icode_list().empty() == false)
				ic_list = lhs_stmt.get_icode_list();

			if (rhs_stmt.get_icode_list().empty() == false)
				ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

			ic_list.push_back(compute_stmt);

			Code_For_Ast * arithmetic_stmt;
			if (ic_list.empty() == false)
				arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

			lhs_register->used_for_expr_result = false;
			rhs_register->used_for_expr_result = false;
			return *arithmetic_stmt;
		}
		else if(lhs->get_data_type() == float_data_type && rhs->get_data_type() == float_data_type){
			Register_Descriptor * result_register = machine_dscr_object.get_new_float_register();
			Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

			Compute_IC_Stmt *compute_stmt;
			if(oper == PLUS){
				compute_stmt = new Compute_IC_Stmt(add, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MINUS){
				compute_stmt = new Compute_IC_Stmt(sub, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == DIV){
				compute_stmt = new Compute_IC_Stmt(div_op, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MULT){
				compute_stmt = new Compute_IC_Stmt(mul, lhs_opd, rhs_opd, register_opd);
			}

			list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

			if (lhs_stmt.get_icode_list().empty() == false)
				ic_list = lhs_stmt.get_icode_list();

			if (rhs_stmt.get_icode_list().empty() == false)
				ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

			ic_list.push_back(compute_stmt);

			Code_For_Ast * arithmetic_stmt;
			if (ic_list.empty() == false)
				arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

			lhs_register->used_for_expr_result = false;
			rhs_register->used_for_expr_result = false;
			return *arithmetic_stmt;
		}
	}
	else{
		if(oper == F_NUM){
			if(lhs_register->get_value_type() == float_num){
				lhs_register->used_for_expr_result = false;
				return lhs_stmt;
			}
			else{
				Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);

				Register_Descriptor * result_register = machine_dscr_object.get_new_float_register();
				Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

				Compute_IC_Stmt *compute_stmt;
				compute_stmt = new Compute_IC_Stmt(mtc1, lhs_opd, NULL, register_opd);

				list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

				if (lhs_stmt.get_icode_list().empty() == false)
					ic_list = lhs_stmt.get_icode_list();

				ic_list.push_back(compute_stmt);

				Code_For_Ast * arithmetic_stmt;
				if (ic_list.empty() == false)
					arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

				lhs_register->used_for_expr_result = false;
				return *arithmetic_stmt;
			}
		}
		else if(oper == I_NUM){
			if(lhs_register->get_value_type() == int_num){
				lhs_register->used_for_expr_result = false;
				return lhs_stmt;
			}
			else{
				Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);

				Register_Descriptor * result_register = machine_dscr_object.get_new_register();
				Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

				Compute_IC_Stmt *compute_stmt;
				compute_stmt = new Compute_IC_Stmt(mfc1, lhs_opd, NULL, register_opd);

				list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

				if (lhs_stmt.get_icode_list().empty() == false)
					ic_list = lhs_stmt.get_icode_list();

				ic_list.push_back(compute_stmt);

				Code_For_Ast * arithmetic_stmt;
				if (ic_list.empty() == false)
					arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

				lhs_register->used_for_expr_result = false;
				return *arithmetic_stmt;
			}
		}
		else {
			Register_Descriptor * result_register;
			if(lhs->get_data_type() == int_data_type)
				result_register = machine_dscr_object.get_new_register();
			else
				result_register = machine_dscr_object.get_new_float_register();

			Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
			Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

			Compute_IC_Stmt *compute_stmt;
			compute_stmt = new Compute_IC_Stmt(uminus, lhs_opd, NULL, register_opd);

			list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

			if (lhs_stmt.get_icode_list().empty() == false)
				ic_list = lhs_stmt.get_icode_list();

			if(compute_stmt != NULL)
				ic_list.push_back(compute_stmt);

			Code_For_Ast * arithmetic_stmt;
			if (ic_list.empty() == false)
				arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

			lhs_register->used_for_expr_result = false;
			return *arithmetic_stmt;
		}
	}
}

Code_For_Ast & Arithmetic_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs of Arithmetic_Expr_Ast cannot be null");

	if((typeid(*lhs) != typeid(Relational_Expr_Ast)) &&
		(typeid(*lhs) != typeid(Arithmetic_Expr_Ast)) &&
		(typeid(*lhs) != typeid(Function_Call_Ast))){
		lra.optimize_lra(mc_2r, NULL, lhs);
	}

	Code_For_Ast & lhs_stmt = lhs->compile_and_optimize_ast(lra);
	Register_Descriptor * lhs_register = lhs_stmt.get_reg();
	lhs_register->used_for_expr_result = true;

	if(rhs != NULL){
		CHECK_INVARIANT((rhs != NULL), "Rhs of Arithmetic_Expr_Ast cannot be null");

		if((typeid(*rhs) != typeid(Relational_Expr_Ast)) &&
			(typeid(*rhs) != typeid(Arithmetic_Expr_Ast)) &&
			(typeid(*rhs) != typeid(Function_Call_Ast))){
			lra.optimize_lra(mc_2r, NULL, rhs);
		}

		Code_For_Ast & rhs_stmt = rhs->compile_and_optimize_ast(lra);
		Register_Descriptor * rhs_register = rhs_stmt.get_reg();
		rhs_register->used_for_expr_result = true;

		Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
		Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_register);

		if(lhs->get_data_type() == int_data_type && rhs->get_data_type() == int_data_type){
			Register_Descriptor * result_register = machine_dscr_object.get_new_register();
			Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

			Compute_IC_Stmt *compute_stmt;
			if(oper == PLUS){
				compute_stmt = new Compute_IC_Stmt(add, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MINUS){
				compute_stmt = new Compute_IC_Stmt(sub, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == DIV){
				compute_stmt = new Compute_IC_Stmt(div_op, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MULT){
				compute_stmt = new Compute_IC_Stmt(mul, lhs_opd, rhs_opd, register_opd);
			}

			list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

			if (lhs_stmt.get_icode_list().empty() == false)
				ic_list = lhs_stmt.get_icode_list();

			if (rhs_stmt.get_icode_list().empty() == false)
				ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

			ic_list.push_back(compute_stmt);

			Code_For_Ast * arithmetic_stmt;
			if (ic_list.empty() == false)
				arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

			lhs_register->used_for_expr_result = false;
			rhs_register->used_for_expr_result = false;
			return *arithmetic_stmt;
		}
		else if(lhs->get_data_type() == float_data_type && rhs->get_data_type() == float_data_type){
			Register_Descriptor * result_register = machine_dscr_object.get_new_float_register();
			Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

			Compute_IC_Stmt *compute_stmt;
			if(oper == PLUS){
				compute_stmt = new Compute_IC_Stmt(add, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MINUS){
				compute_stmt = new Compute_IC_Stmt(sub, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == DIV){
				compute_stmt = new Compute_IC_Stmt(div_op, lhs_opd, rhs_opd, register_opd);
			}
			else if(oper == MULT){
				compute_stmt = new Compute_IC_Stmt(mul, lhs_opd, rhs_opd, register_opd);
			}

			list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

			if (lhs_stmt.get_icode_list().empty() == false)
				ic_list = lhs_stmt.get_icode_list();

			if (rhs_stmt.get_icode_list().empty() == false)
				ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());

			ic_list.push_back(compute_stmt);

			Code_For_Ast * arithmetic_stmt;
			if (ic_list.empty() == false)
				arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

			lhs_register->used_for_expr_result = false;
			rhs_register->used_for_expr_result = false;
			return *arithmetic_stmt;
		}
	}
	else{
		if(oper == F_NUM){
			if(lhs_register->get_value_type() == float_num){
				lhs_register->used_for_expr_result = false;
				return lhs_stmt;
			}
			else{
				Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);

				Register_Descriptor * result_register = machine_dscr_object.get_new_float_register();
				Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

				Compute_IC_Stmt *compute_stmt;
				compute_stmt = new Compute_IC_Stmt(mtc1, lhs_opd, NULL, register_opd);

				list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

				if (lhs_stmt.get_icode_list().empty() == false)
					ic_list = lhs_stmt.get_icode_list();

				ic_list.push_back(compute_stmt);

				Code_For_Ast * arithmetic_stmt;
				if (ic_list.empty() == false)
					arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

				lhs_register->used_for_expr_result = false;
				return *arithmetic_stmt;
			}
		}
		else if(oper == I_NUM){
			if(lhs_register->get_value_type() == int_num){
				lhs_register->used_for_expr_result = false;
				return lhs_stmt;
			}
			else{
				Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);

				Register_Descriptor * result_register = machine_dscr_object.get_new_register();
				Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

				Compute_IC_Stmt *compute_stmt;
				compute_stmt = new Compute_IC_Stmt(mfc1, lhs_opd, NULL, register_opd);

				list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

				if (lhs_stmt.get_icode_list().empty() == false)
					ic_list = lhs_stmt.get_icode_list();

				ic_list.push_back(compute_stmt);

				Code_For_Ast * arithmetic_stmt;
				if (ic_list.empty() == false)
					arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

				lhs_register->used_for_expr_result = false;
				return *arithmetic_stmt;
			}
		}
		else {
			Register_Descriptor * result_register;
			if(lhs->get_data_type() == int_data_type)
				result_register = machine_dscr_object.get_new_register();
			else
				result_register = machine_dscr_object.get_new_float_register();

			Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_register);
			Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

			Compute_IC_Stmt *compute_stmt;
			compute_stmt = new Compute_IC_Stmt(uminus, lhs_opd, NULL, register_opd);

			list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

			if (lhs_stmt.get_icode_list().empty() == false)
				ic_list = lhs_stmt.get_icode_list();

			if(compute_stmt != NULL)
				ic_list.push_back(compute_stmt);

			Code_For_Ast * arithmetic_stmt;
			if (ic_list.empty() == false)
				arithmetic_stmt = new Code_For_Ast(ic_list, result_register);

			lhs_register->used_for_expr_result = false;
			return *arithmetic_stmt;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	successor = -1;

	lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

int Assignment_Ast::get_successor(){
	return successor;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH,
		"Assignment statement data type not compatible");
}

void Assignment_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Asgn:";

	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	Eval_Result & result = rhs->evaluate(eval_env, file_buffer);
	CHECK_INPUT_AND_ABORT(result.is_variable_defined(), "Variable should be defined to be on rhs of Assignment_Ast", lineno);

	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");
	lhs->set_value_of_evaluation(eval_env, result);

	// Print the result

	print(file_buffer);

	lhs->print_value(eval_env, file_buffer);
	return result;
}

Code_For_Ast & Assignment_Ast::compile()
{
	/*
		An assignment x = y where y is a variable is
		compiled as a combination of load and store statements:
		(load) R <- y
		(store) x <- R
		If y is a constant, the statement is compiled as:
		(imm_Load) R <- y
		(store) x <- R
		where imm_Load denotes the load immediate operation.
	*/

	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();
	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	if((typeid(*rhs) != typeid(Relational_Expr_Ast)) &&
		(typeid(*rhs) != typeid(Arithmetic_Expr_Ast)) &&
		(typeid(*rhs) != typeid(Function_Call_Ast))){
		lra.optimize_lra(mc_2m, lhs, rhs);
	}

	Code_For_Ast load_stmt = rhs->compile_and_optimize_ast(lra);

	Register_Descriptor * result_register = load_stmt.get_reg();

	Symbol_Table_Entry *new_entry = &(lhs->get_symbol_entry());

	Register_Descriptor * curr_register = new_entry->get_register();
	if(curr_register != NULL){
		new_entry->free_register(curr_register);
	}
	new_entry->update_register(result_register);

	Code_For_Ast store_stmt = lhs->create_store_stmt(result_register);

	list<Icode_Stmt *> ic_list;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);

	return *assign_stmt;
}

/////////////////////////////////////////////////////////////////
If_Else_Ast::If_Else_Ast(Ast * temp_gotoTrue, Ast * temp_condition, Ast * temp_gotoFalse, int line)
{
	gotoTrue = temp_gotoTrue;
	condition = temp_condition;
	gotoFalse = temp_gotoFalse;

	ast_num_child = binary_arity;
	successor = -3;
	lineno = line;
}

If_Else_Ast::~If_Else_Ast()
{
	delete gotoTrue;
	delete gotoFalse;
	delete condition;
}

int If_Else_Ast::get_successor(){
	return successor;
}

bool If_Else_Ast::check_ast()
{
	CHECK_INVARIANT((condition != NULL), "Condition of If_Else_Ast cannot be null");
	CHECK_INVARIANT((gotoTrue != NULL), "True Successor of If_Else_Ast cannot be null");
	CHECK_INVARIANT((gotoFalse != NULL), "False Successor of If_Else_Ast cannot be null");
}

void If_Else_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "If_Else statement:";
	condition->print(file_buffer);
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "True Successor: " << gotoTrue->get_successor();
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "False Successor: " << gotoFalse ->get_successor();
}

Eval_Result & If_Else_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// Print the result
	file_buffer << "\n";
	file_buffer << AST_SPACE << "If_Else statement:";
	CHECK_INVARIANT((condition != NULL), "Condition of If_Else_Ast cannot be null");
	condition->print(file_buffer);
	Eval_Result & conditionResult = condition->evaluate(eval_env, file_buffer);
	int value = conditionResult.get_int_value();
	Eval_Result & result = *new Eval_Result_Value_Int();
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "True Successor: " << gotoTrue->get_successor();
	file_buffer << "\n";
	file_buffer << AST_NODE_SPACE << "False Successor: " << gotoFalse ->get_successor();
	file_buffer << "\n";
	if(value == 1){
		CHECK_INVARIANT((gotoTrue != NULL), "True Successor of If_Else_Ast cannot be null");
		file_buffer << AST_SPACE << "Condition True : Goto (BB " << gotoTrue->get_successor() << ")\n";
		successor = gotoTrue->get_successor();
	}
	else{
		file_buffer << AST_SPACE << "Condition False : Goto (BB " << gotoFalse->get_successor() << ")\n";
		CHECK_INVARIANT((gotoFalse != NULL), "False Successor of If_Else_Ast cannot be null");
		successor = gotoFalse->get_successor();
	}
	result.set_value_int(successor);
	return result;
}

Code_For_Ast & If_Else_Ast::compile()
{
	CHECK_INVARIANT((condition != NULL), "Condition of If_Else_Ast cannot be null");
	CHECK_INVARIANT((gotoTrue != NULL), "True Successor of If_Else_Ast cannot be null");
	CHECK_INVARIANT((gotoFalse != NULL), "False Successor of If_Else_Ast cannot be null");

	Code_For_Ast & condition_code = condition->compile();
	Register_Descriptor * condition_result_register = condition_code.get_reg();
	condition_result_register->used_for_expr_result = true;

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();

	Ics_Opd * condition_opd = new Register_Addr_Opd(condition_result_register);
	Ics_Opd * zero_opd = new Register_Addr_Opd(machine_dscr_object.spim_register_table[zero]);
	Const_Opd<int> * goto_false_opd = new Const_Opd<int>(gotoFalse->get_successor());
	Const_Opd<int> * goto_true_opd = new Const_Opd<int>(gotoTrue->get_successor());

	Icode_Stmt * bne_stmt = new Control_Flow_IC_Stmt(bne, condition_opd, zero_opd, goto_true_opd);
	Icode_Stmt * goto_stmt = new Control_Flow_IC_Stmt(goto_op, goto_false_opd);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (condition_code.get_icode_list().empty() == false)
		ic_list = condition_code.get_icode_list();

	ic_list.push_back(bne_stmt);
	ic_list.push_back(goto_stmt);

	Code_For_Ast * if_else_stmt;
	if (ic_list.empty() == false)
		if_else_stmt = new Code_For_Ast(ic_list, result_register);

	condition_result_register->used_for_expr_result = false;
	return *if_else_stmt;
}

Code_For_Ast & If_Else_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((condition != NULL), "Condition of If_Else_Ast cannot be null");
	CHECK_INVARIANT((gotoTrue != NULL), "True Successor of If_Else_Ast cannot be null");
	CHECK_INVARIANT((gotoFalse != NULL), "False Successor of If_Else_Ast cannot be null");

	Code_For_Ast & condition_code = condition->compile_and_optimize_ast(lra);
	Register_Descriptor * condition_result_register = condition_code.get_reg();
	condition_result_register->used_for_expr_result = true;

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();

	Ics_Opd * condition_opd = new Register_Addr_Opd(condition_result_register);
	Ics_Opd * zero_opd = new Register_Addr_Opd(machine_dscr_object.spim_register_table[zero]);
	Const_Opd<int> * goto_false_opd = new Const_Opd<int>(gotoFalse->get_successor());
	Const_Opd<int> * goto_true_opd = new Const_Opd<int>(gotoTrue->get_successor());

	Icode_Stmt * bne_stmt = new Control_Flow_IC_Stmt(bne, condition_opd, zero_opd, goto_true_opd);
	Icode_Stmt * goto_stmt = new Control_Flow_IC_Stmt(goto_op, goto_false_opd);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	if (condition_code.get_icode_list().empty() == false)
		ic_list = condition_code.get_icode_list();

	ic_list.push_back(bne_stmt);
	ic_list.push_back(goto_stmt);

	Code_For_Ast * if_else_stmt;
	if (ic_list.empty() == false)
		if_else_stmt = new Code_For_Ast(ic_list, result_register);

	condition_result_register->used_for_expr_result = false;
	return *if_else_stmt;
}

/////////////////////////////////////////////////////////////////
Goto_Ast::Goto_Ast(int temp_bb, int line)
{
	bb = temp_bb;
	successor = temp_bb;

	ast_num_child = binary_arity;

	lineno = line;
}

Goto_Ast::~Goto_Ast()
{
}

int Goto_Ast::get_successor(){
	return successor;
}

bool Goto_Ast::check_ast()
{
	CHECK_INVARIANT((bb >= 2), "Basic block lable should be greater than 1");
}

void Goto_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n";
	file_buffer << AST_SPACE << "Goto statement:\n";
	file_buffer << AST_NODE_SPACE << "Successor: " << bb;
}

Eval_Result & Goto_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// Print the result
	print(file_buffer);
	Eval_Result & result = *new Eval_Result_Value_Int();
	result.set_value_int(bb);
	file_buffer << "\n";
	file_buffer << AST_SPACE << "GOTO (BB " << successor << ")\n";
	successor = bb;
	return result;
}

Code_For_Ast & Goto_Ast::compile()
{
	CHECK_INVARIANT((bb >= 2), "Basic block lable should be greater than 1");

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();

	Const_Opd<int> * bb_id = new Const_Opd<int>(bb);
	Icode_Stmt * goto_stmt = new Control_Flow_IC_Stmt(goto_op, bb_id);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	ic_list.push_back(goto_stmt);

	Code_For_Ast * goto_code_stmt;
	if (ic_list.empty() == false)
		goto_code_stmt = new Code_For_Ast(ic_list, result_register);

	return *goto_code_stmt;
}

Code_For_Ast & Goto_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((bb >= 2), "Basic block lable should be greater than 1");

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();

	Const_Opd<int> * bb_id = new Const_Opd<int>(bb);
	Icode_Stmt * goto_stmt = new Control_Flow_IC_Stmt(goto_op, bb_id);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	ic_list.push_back(goto_stmt);

	Code_For_Ast * goto_code_stmt;
	if (ic_list.empty() == false)
		goto_code_stmt = new Code_For_Ast(ic_list, result_register);

	return *goto_code_stmt;
}
///////////////////////////////////////////////////////////////////////////////
Function_Call_Ast::Function_Call_Ast(string name , Data_Type return_data_type, list <Ast *> * input_list, int line){

	func_name = name;
	input_argument_list = input_list;
	successor = -1;
	node_data_type = return_data_type;
	lineno = line;
}

Function_Call_Ast::~Function_Call_Ast()
{

}

Data_Type Function_Call_Ast::get_data_type()
{
	return node_data_type;
}

bool Function_Call_Ast::check_ast()
{

}

void Function_Call_Ast::print(ostream & file_buffer)
{

		Procedure *funct_call = program_object.get_procedure(func_name);
		CHECK_INVARIANT(funct_call->is_body_defined() == true, "Procedure Body is not defined");

		file_buffer<<"\n";
		file_buffer << AST_SPACE << "FN CALL: ";
		list<Ast *>::iterator i;
		if(input_argument_list != NULL){
			//file_buffer << AST_NODE_SPACE 	;
			file_buffer <<func_name << "(";
			for(i = input_argument_list->begin(); i != input_argument_list->end(); i++){
				file_buffer<<"\n";
				file_buffer << AST_NODE_SPACE 	;
				(*i)->print(file_buffer);

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
	CHECK_INVARIANT(funct_call->is_body_defined() == true, "Procedure Body is not defined");

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
			 	k->set_value_int((int)result.get_int_value());
				func_eval_env.put_variable_value(*k, variable_name);

			}
			else if(result.get_result_enum() == float_result && (*j)->get_data_type() == float_data_type){
				k = new Eval_Result_Value_Float();
			 	k->set_value_float(result.get_float_value());
					func_eval_env.put_variable_value(*k, variable_name);

			}
			else{
				CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Passed argument is not compatible");
			}
		}
	}
	Eval_Result & return_result = funct_call->evaluate(func_eval_env,file_buffer);
	return return_result;
}

Code_For_Ast & Function_Call_Ast::compile()
{
	Procedure *funct_call = program_object.get_procedure(func_name);
	CHECK_INVARIANT(funct_call->is_body_defined() == true, "Procedure Body is not defined");

	Argument_Table & arg = funct_call->get_argument_symbol_table();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	list<Ast *>::iterator i;
	int offset = 0;
	if(input_argument_list != NULL){
		list<Symbol_Table_Entry *>::iterator j;
		j = arg.variable_table.end();
		j--;
		for(i = input_argument_list->begin(); i != input_argument_list->end(); i++, j--){
			Code_For_Ast & arg_stmt = (*i)->compile();

			if (arg_stmt.get_icode_list().empty() == false)
				ic_list.splice(ic_list.end(), arg_stmt.get_icode_list());

				Register_Descriptor * store_register = arg_stmt.get_reg();
				CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");

				Ics_Opd * register_opd = new Register_Addr_Opd(store_register);

				Ics_Opd * opd = new Mem_Addr_Opd((**j), offset);

				if((*j)->get_data_type() == int_data_type){
					offset += 4;
				}
				else{
					offset += 8;
				}

				Register_Descriptor * new_register;
				new_register = machine_dscr_object.get_new_register();

				Icode_Stmt * store_stmt = new Move_IC_Stmt(store, register_opd, opd);

				ic_list.push_back(store_stmt);
		}
	}
	if(offset > 0){
		Register_Descriptor * stack_register = machine_dscr_object.spim_register_table[sp];
		Ics_Opd * stack_opd = new Register_Addr_Opd(stack_register);
		Ics_Opd * offset_opd = new Const_Opd<int>(offset);
		Compute_IC_Stmt * stack_sub_stmt = new Compute_IC_Stmt(stack_sub, stack_opd, offset_opd, stack_opd);
		ic_list.push_back(stack_sub_stmt);
	}

	//call statement
	Move_IC_Stmt * funcion_call_stmt = new Move_IC_Stmt(call, func_name);
	ic_list.push_back(funcion_call_stmt);

	if(offset > 0){
		Register_Descriptor * stack_register = machine_dscr_object.spim_register_table[sp];
		Ics_Opd * stack_opd = new Register_Addr_Opd(stack_register);
		Ics_Opd * offset_opd = new Const_Opd<int>(offset);
		Compute_IC_Stmt * stack_add_stmt = new Compute_IC_Stmt(stack_add, stack_opd, offset_opd, stack_opd);
		ic_list.push_back(stack_add_stmt);
	}

	//move statement
	Ics_Opd * opd;
	Ics_Opd * register_opd;
	Register_Descriptor * new_res_register;
	if(funct_call->get_return_type() == int_data_type){
		Register_Descriptor * fn_result_register = machine_dscr_object.spim_register_table[v1];
		opd = new Register_Addr_Opd(fn_result_register);

		Register_Descriptor * result_register;
		result_register = machine_dscr_object.get_new_register();
		register_opd = new Register_Addr_Opd(result_register);

		new_res_register = machine_dscr_object.get_new_register();

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_op, opd, register_opd);
		ic_list.push_back(move_stmt);
	}
	else if(funct_call->get_return_type() == float_data_type){
		Register_Descriptor * fn_result_register = machine_dscr_object.spim_register_table[f0];
		opd = new Register_Addr_Opd(fn_result_register);

		Register_Descriptor * result_register;
		result_register = machine_dscr_object.get_new_float_register();
		register_opd = new Register_Addr_Opd(result_register);

		new_res_register = machine_dscr_object.get_new_float_register();

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_op, opd, register_opd);
		ic_list.push_back(move_stmt);
	}
	else{
		new_res_register = machine_dscr_object.get_new_register();
	}

	Code_For_Ast * fn_stmt;
	if (ic_list.empty() == false)
		fn_stmt = new Code_For_Ast(ic_list, new_res_register);

	return *fn_stmt;
}

Code_For_Ast & Function_Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
		Procedure *funct_call = program_object.get_procedure(func_name);
	CHECK_INVARIANT(funct_call->is_body_defined() == true, "Procedure Body is not defined");

	Argument_Table & arg = funct_call->get_argument_symbol_table();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	list<Ast *>::iterator i;
	int offset = 0;
	if(input_argument_list != NULL){
		list<Symbol_Table_Entry *>::iterator j;
		j = arg.variable_table.end();
		j--;
		for(i = input_argument_list->begin(); i != input_argument_list->end(); i++, j--){
			if((typeid(**i) != typeid(Relational_Expr_Ast)) &&
				(typeid(**i) != typeid(Arithmetic_Expr_Ast)) &&
				(typeid(**i) != typeid(Function_Call_Ast))){
				lra.optimize_lra(mc_2r, NULL, *i);
			}
			Code_For_Ast & arg_stmt = (*i)->compile_and_optimize_ast(lra);

			if (arg_stmt.get_icode_list().empty() == false)
				ic_list.splice(ic_list.end(), arg_stmt.get_icode_list());

			Register_Descriptor * store_register = arg_stmt.get_reg();
			CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");

			Ics_Opd * register_opd = new Register_Addr_Opd(store_register);

			Ics_Opd * opd = new Mem_Addr_Opd((**j), offset);

			if((*j)->get_data_type() == int_data_type){
				offset += 4;
			}
			else{
				offset += 8;
			}

			Register_Descriptor * new_register;
			new_register = machine_dscr_object.get_new_register();

			Icode_Stmt * store_stmt = new Move_IC_Stmt(store, register_opd, opd);

			(*j)->free_register(store_register);

			ic_list.push_back(store_stmt);
		}
	}
	if(offset > 0){
		Register_Descriptor * stack_register = machine_dscr_object.spim_register_table[sp];
		Ics_Opd * stack_opd = new Register_Addr_Opd(stack_register);
		Ics_Opd * offset_opd = new Const_Opd<int>(offset);
		Compute_IC_Stmt * stack_sub_stmt = new Compute_IC_Stmt(stack_sub, stack_opd, offset_opd, stack_opd);
		ic_list.push_back(stack_sub_stmt);
	}

	//call statement
	Move_IC_Stmt * funcion_call_stmt = new Move_IC_Stmt(call, func_name);
	ic_list.push_back(funcion_call_stmt);

	if(offset > 0){
		Register_Descriptor * stack_register = machine_dscr_object.spim_register_table[sp];
		Ics_Opd * stack_opd = new Register_Addr_Opd(stack_register);
		Ics_Opd * offset_opd = new Const_Opd<int>(offset);
		Compute_IC_Stmt * stack_add_stmt = new Compute_IC_Stmt(stack_add, stack_opd, offset_opd, stack_opd);
		ic_list.push_back(stack_add_stmt);
	}

	machine_dscr_object.clear_local_register_mappings();

	//move statement
	Ics_Opd * opd;
	Ics_Opd * register_opd;
	Register_Descriptor * new_res_register;
	if(funct_call->get_return_type() == int_data_type){
		Register_Descriptor * fn_result_register = machine_dscr_object.spim_register_table[v1];
		opd = new Register_Addr_Opd(fn_result_register);

		Register_Descriptor * result_register;

		result_register = machine_dscr_object.get_new_register();
		register_opd = new Register_Addr_Opd(result_register);

		new_res_register = machine_dscr_object.get_new_register();

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_op, opd, register_opd);
		ic_list.push_back(move_stmt);
	}
	else if(funct_call->get_return_type() == float_data_type){
		Register_Descriptor * fn_result_register = machine_dscr_object.spim_register_table[f0];
		opd = new Register_Addr_Opd(fn_result_register);

		Register_Descriptor * result_register;
		result_register = machine_dscr_object.get_new_float_register();
		register_opd = new Register_Addr_Opd(result_register);

		new_res_register = machine_dscr_object.get_new_float_register();

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_op, opd, register_opd);
		ic_list.push_back(move_stmt);
	}
	else{
		new_res_register = machine_dscr_object.get_new_register();
	}

	Code_For_Ast * fn_stmt;
	if (ic_list.empty() == false)
		fn_stmt = new Code_For_Ast(ic_list, new_res_register);

	return *fn_stmt;
}
/////////////////////////////////////////////////////////////////
Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	variable_symbol_entry = &var_entry;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");

	ast_num_child = zero_arity;
	node_data_type = variable_symbol_entry->get_data_type();
	lineno = line;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return *variable_symbol_entry;
}

void Name_Ast::print(ostream & file_buffer)
{
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << "\n" << AST_SPACE << variable_name << " : ";

	if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
		file_buffer << "undefined";

	else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
		CHECK_INVARIANT((loc_var_val->get_result_enum() == int_result) || (loc_var_val->get_result_enum() == float_result), "Result type can only be int and float");
		if(loc_var_val->get_result_enum() == int_result)
			file_buffer << loc_var_val->get_int_value() << "\n";
		else if(loc_var_val->get_result_enum() == float_result)
			file_buffer << loc_var_val->get_float_value() << "\n";
	}

	else
	{
		CHECK_INVARIANT(((glob_var_val->get_result_enum() == int_result) || (glob_var_val->get_result_enum() == float_result)),
			"Result type can only be int and float");

		if (glob_var_val == NULL)
			file_buffer << "0\n";
		else if(glob_var_val->get_result_enum() == int_result)
			file_buffer << glob_var_val->get_int_value() << "\n";
		else if(glob_var_val->get_result_enum() == float_result)
			file_buffer << glob_var_val->get_float_value() << "\n";
	}
	file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	if (eval_env.does_variable_exist(variable_name))
	{
		CHECK_INPUT_AND_ABORT((eval_env.is_variable_defined(variable_name) == true),
					"Variable should be defined before its use", lineno);

		Eval_Result * result = eval_env.get_variable_value(variable_name);
		return *result;
	}

	CHECK_INPUT_AND_ABORT((interpreter_global_table.is_variable_defined(variable_name) == true),
				"Variable should be defined before its use", lineno);

	Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
	return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	Eval_Result * i;
	string variable_name = variable_symbol_entry->get_variable_name();

	if (variable_symbol_entry->get_data_type() == int_data_type)
		i = new Eval_Result_Value_Int();
	else if (variable_symbol_entry->get_data_type() == float_data_type)
		i = new Eval_Result_Value_Float();
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (result.get_result_enum() == int_result)
	 	i->set_value_int(result.get_int_value());
	else if (result.get_result_enum() == float_result)
		i->set_value_float(result.get_float_value());
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (eval_env.does_variable_exist(variable_name))
		eval_env.put_variable_value(*i, variable_name);
	else
		interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	return get_value_of_evaluation(eval_env);
}

Code_For_Ast & Name_Ast::compile()
{
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry, -1);
	Register_Descriptor * result_register;
	if(variable_symbol_entry->get_data_type() == int_data_type)
		result_register = machine_dscr_object.get_new_register();
	else if (variable_symbol_entry->get_data_type() == float_data_type)
		result_register = machine_dscr_object.get_new_float_register();

	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(load, opd, register_opd);

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");

	Ics_Opd * register_opd = new Register_Addr_Opd(store_register);
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry, -1);

	Icode_Stmt * store_stmt = new Move_IC_Stmt(store, register_opd, opd);

	if (command_options.is_do_lra_selected() == false)
		variable_symbol_entry->free_register(store_register);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(store_stmt);

	Code_For_Ast & name_code = *new Code_For_Ast(ic_list, store_register);

	return name_code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;;

	bool load_needed = lra.is_load_needed();
	Register_Descriptor * result_register = lra.get_register();
	CHECK_INVARIANT((result_register != NULL), "Register cannot be null");
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = NULL;
	if (load_needed)
	{
		Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry, -1);

		load_stmt = new Move_IC_Stmt(load, opd, register_opd);

		ic_list.push_back(load_stmt);
	}

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	constant = number;
	node_data_type = constant_data_type;

	ast_num_child = zero_arity;

	lineno = line;
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
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	file_buffer << std::fixed;
	file_buffer << std::setprecision(2);

	file_buffer << "Num : " << constant;
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_value_int(constant);

		return result;
	}
	else if(node_data_type == float_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Float();
		result.set_value_float(constant);
		return result;
	}
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	Register_Descriptor * result_register;
	Ics_Opd * opd;
	if(node_data_type == int_data_type){
		opd = new Const_Opd<int>(constant);
		result_register = machine_dscr_object.get_new_register();
	}
	else if(node_data_type == float_data_type){
		opd = new Const_Opd<float>(constant);
		result_register = machine_dscr_object.get_new_float_register();
	}

	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(result_register);
	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, result_register);

	return num_code;
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lra.get_register() != NULL), "Register assigned through optimize_lra cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(lra.get_register());
	Ics_Opd * opd;
	if(node_data_type == int_data_type)
		opd = new Const_Opd<int>(constant);
	else
		opd = new Const_Opd<float>(constant);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, lra.get_register());

	return num_code;
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast * to_return, string name, int line)
{
	return_Ast = to_return;
	lineno = line;
	ast_num_child = zero_arity;
	successor = -2;
	fn_name = name;
	if(return_Ast == NULL){
		node_data_type = void_data_type;
	}
}

Return_Ast::~Return_Ast()
{}

Data_Type Return_Ast::get_data_type()
{
	return node_data_type;
}

bool Return_Ast::check_ast(){
	if(return_Ast == NULL){
		node_data_type = void_data_type;
	}
}

void Return_Ast::print(ostream & file_buffer)
{
	if(return_Ast == NULL){
		file_buffer <<"\n";
		file_buffer << AST_SPACE << "RETURN <NOTHING>\n";
	}
	else{
		file_buffer <<"\n";
		file_buffer << AST_SPACE << "RETURN ";
		return_Ast->print(file_buffer);
		file_buffer<<"\n";
	}
}

int Return_Ast::get_successor(){
	return successor;
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if(return_Ast!=NULL){
		Eval_Result & result = return_Ast->evaluate(eval_env,file_buffer);
		print(file_buffer);
		return result;
	}
	else{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_result_enum(void_result);
		print(file_buffer);
		return result;
	}
}

Code_For_Ast & Return_Ast::compile()
{

	Register_Descriptor * return_value_register;

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Return_IC_Stmt * ret_stmt;
	Code_For_Ast * ret_code;

	if(return_Ast!=NULL){
		Code_For_Ast & return_value = return_Ast->compile();
		return_value_register = return_value.get_reg();
		return_value_register->used_for_expr_result = true;

		ic_list = return_value.get_icode_list();

		//move statement
		Register_Descriptor * fn_result_register;
		if(return_Ast->get_data_type() == int_data_type)
			fn_result_register = machine_dscr_object.spim_register_table[v1];
		else
			fn_result_register = machine_dscr_object.spim_register_table[f0];
		Ics_Opd * opd = new Register_Addr_Opd(fn_result_register);

		Ics_Opd * register_opd = new Register_Addr_Opd(return_value_register);

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_op, register_opd, opd);
		ic_list.push_back(move_stmt);

		ret_stmt = new Return_IC_Stmt(return_op, fn_name);
		ic_list.push_back(ret_stmt);

		if (ic_list.empty() == false)
			ret_code = new Code_For_Ast(ic_list, result_register);
		return_value_register->used_for_expr_result = false;
	}
	else{
		ret_stmt = new Return_IC_Stmt(return_op, fn_name);
		ic_list.push_back(ret_stmt);
		if (ic_list.empty() == false)
			ret_code = new Code_For_Ast(ic_list, result_register);
	}
	return *ret_code;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	Register_Descriptor * return_value_register;

	Register_Descriptor * result_register = machine_dscr_object.get_new_register();
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	Return_IC_Stmt * ret_stmt;
	Code_For_Ast * ret_code;

	if(return_Ast!=NULL){
		if((typeid(*return_Ast) != typeid(Relational_Expr_Ast)) && 
			(typeid(*return_Ast) != typeid(Arithmetic_Expr_Ast)) &&
			(typeid(*return_Ast) != typeid(Function_Call_Ast))){
			lra.optimize_lra(mc_2r, NULL, return_Ast);
		}
		Code_For_Ast & return_value = return_Ast->compile_and_optimize_ast(lra);
		return_value_register = return_value.get_reg();
		return_value_register->used_for_expr_result = true;

		ic_list = return_value.get_icode_list();

		//move statement
		Register_Descriptor * fn_result_register;
		if(return_Ast->get_data_type() == int_data_type)
			fn_result_register = machine_dscr_object.spim_register_table[v1];
		else
			fn_result_register = machine_dscr_object.spim_register_table[f0];
		Ics_Opd * opd = new Register_Addr_Opd(fn_result_register);

		Ics_Opd * register_opd = new Register_Addr_Opd(return_value_register);

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_op, register_opd, opd);
		ic_list.push_back(move_stmt);

		ret_stmt = new Return_IC_Stmt(return_op, fn_name);
		ic_list.push_back(ret_stmt);

		if (ic_list.empty() == false)
			ret_code = new Code_For_Ast(ic_list, result_register);
		return_value_register->used_for_expr_result = false;
	}
	else{
		ret_stmt = new Return_IC_Stmt(return_op, fn_name);
		ic_list.push_back(ret_stmt);
		if (ic_list.empty() == false)
			ret_code = new Code_For_Ast(ic_list, result_register);
	}
	return *ret_code;
}

template class Number_Ast<int>;
template class Number_Ast<float>;