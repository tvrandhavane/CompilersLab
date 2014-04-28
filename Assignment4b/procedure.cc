
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

#include<string>
#include<fstream>
#include<iostream>

using namespace std;

#include"error-display.hh"
#include"local-environment.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Procedure::Procedure(Data_Type proc_return_type, string proc_name ,Argument_Table & argument_list)
{
	return_type = proc_return_type;
	argument_symbol_table = argument_list;
	name = proc_name;
	return_flag = 0;
}

Procedure::~Procedure()
{
	list<Basic_Block *>::iterator i;
	for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		delete (*i);
}

void Procedure::set_return_flag(int ret){
	return_flag = ret;
}

int Procedure::get_return_flag(){
	return return_flag;
}

string Procedure::get_proc_name()
{
	return name;
}

void Procedure::set_basic_block_list(list<Basic_Block *> bb_list)
{
	basic_block_list = bb_list;
}

void Procedure::set_local_list(Symbol_Table & new_list)
{
	local_symbol_table = new_list;
	local_symbol_table.set_table_scope(local);
}

Data_Type Procedure::get_return_type()
{
	return return_type;
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable) ;
}

bool Procedure::variable_in_argument_list_check(string variable)
{
	return argument_symbol_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
	return local_symbol_table.get_symbol_table_entry(variable_name);
}

Symbol_Table_Entry & Procedure::get_argument_table_entry(string variable_name)
{
	return argument_symbol_table.get_symbol_table_entry(variable_name);
}

bool Procedure ::check_parameter_local_var(){

	list<Symbol_Table_Entry *>::iterator i;
	for (i = argument_symbol_table.variable_table.begin(); i != argument_symbol_table.variable_table.end(); i++)
	{
		if(local_symbol_table.variable_in_symbol_list_check((*i)->get_variable_name())){
			return true;
		}

	}
	return false;

}
void Procedure::print_ast(ostream & file_buffer)
{
	file_buffer << PROC_SPACE << "Procedure: "<< get_proc_name() << "\n\n";

	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		(*i)->print_bb(file_buffer);
}
Argument_Table & Procedure::get_argument_symbol_table(){

	return argument_symbol_table;
}
bool Procedure::is_body_defined(){

	if(basic_block_list.size() > 0)
		return true;
	else 
		return false;
}
	
Basic_Block & Procedure::get_start_basic_block()
{
	list<Basic_Block *>::iterator i;
	i = basic_block_list.begin();
	return **i;
}


Basic_Block * Procedure::get_next_bb(Basic_Block & current_bb)
{
	bool flag = false;

	list<Basic_Block *>::iterator i;
	if(current_bb.get_successor() == -1){
		for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		{
			if((*i)->get_bb_number() == current_bb.get_bb_number())
			{
				flag = true;
				continue;
			}
			if (flag)
				return (*i);
		}
	}
	else{
		for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		{
			if((*i)->get_bb_number() == current_bb.get_successor())
			{
				return (*i);
			}
		}
	}
	if(current_bb.get_successor() == -2){
		return NULL;
	}
	else{
		report_internal_error("Atleast one of true, false, direct successors should be set");
	}
	
	
}

Eval_Result & Procedure::evaluate(Local_Environment & eval_env , ostream & file_buffer)
{
	
	local_symbol_table.create(eval_env);
	Eval_Result * result = NULL;

	file_buffer << PROC_SPACE << "Evaluating Procedure << " << name << " >>\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (before evaluating):\n";
	eval_env.print(file_buffer);
	file_buffer << "\n";
	
	Basic_Block * current_bb = &(get_start_basic_block());
	while (current_bb)
	{
		result = &(current_bb->evaluate(eval_env, file_buffer));
		current_bb = get_next_bb(*current_bb);		
	}

	file_buffer << "\n\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating) Function: << " << name << " >>\n";
	string var_return = "return";
	if(result->get_result_enum() != void_result ){
		if(result->get_result_enum() == int_result){
			Eval_Result_Value_Int * j = new Eval_Result_Value_Int();
			j->set_value(result->get_value());
			eval_env.put_variable_value(*j,var_return);
		}
		else if(result->get_result_enum() == float_result){
			Eval_Result_Value_Float * j = new Eval_Result_Value_Float();
			j->set_value(result->get_value());
			eval_env.put_variable_value(*j,var_return);
		}

	}
	//else if(return_type != void_return_type)

	eval_env.print(file_buffer);
	/*if(result->get_result_enum() != void_result ){
		if(result->get_result_enum() == int_result)
			file_buffer << VAR_SPACE << "return : " << (int) result->get_value()<<"\n";
		else if(result->get_result_enum() == float_result)
			file_buffer << VAR_SPACE << "return : " << result->get_value()<<"\n";
	}*/

	return *result;
}

