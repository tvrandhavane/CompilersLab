
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

Procedure::Procedure(Data_Type proc_return_type, string proc_name,Argument_Table & argument_list, int line)
{
	return_type = proc_return_type;
	argument_symbol_table = argument_list;
	argument_symbol_table.set_table_scope(local);
	name = proc_name;
	lineno = line;
	return_flag = 0;
}

Procedure::~Procedure()
{
	list<Basic_Block *>::iterator i;
	for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		delete (*i);
}

void Procedure::check_valid_bb(){

	list<int>::iterator i;
	for(i = basic_block_gotos.begin(); i != basic_block_gotos.end(); i++){
		if((*i) > (basic_block_list.size() + 1)){
			char buff[20];

			sprintf(buff, "bb %d does not exist", (*i));
			CHECK_INVARIANT(false,buff);
		}
	}
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

void Procedure::set_basic_block_list(list<Basic_Block *> & bb_list)
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
	return local_symbol_table.variable_in_symbol_list_check(variable);
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

Argument_Table & Procedure::get_argument_symbol_table(){

	return argument_symbol_table;
}

bool Procedure::is_body_defined(){

	if(basic_block_list.size() > 0)
		return true;
	else 
		return false;
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

void Procedure::print(ostream & file_buffer)
{
	file_buffer << PROC_SPACE << "Procedure: " << name << ", Return Type: ";
	if(return_type == void_data_type)
		file_buffer << "VOID";
	else if(return_type == float_data_type)
		file_buffer << "FLOAT";
	else
		file_buffer << "INT";
	file_buffer << "\n";

	if ((command_options.is_show_symtab_selected()) || (command_options.is_show_program_selected()))
	{
		file_buffer << "   Fromal Parameter List\n";
		argument_symbol_table.print(file_buffer);
		file_buffer << "   Local Declarartions\n";
		local_symbol_table.print(file_buffer);
	}
	if ((command_options.is_show_program_selected()) || (command_options.is_show_ast_selected()))
	{
		list<Basic_Block *>::iterator i;
		for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
			(*i)->print_bb(file_buffer);
	}
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
	return NULL;
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
			j->set_value_int(result->get_int_value());
			eval_env.put_variable_value(*j,var_return);
		}
		else if(result->get_result_enum() == float_result){
			Eval_Result_Value_Float * j = new Eval_Result_Value_Float();
			j->set_value_float(result->get_float_value());
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
/*
Eval_Result & Procedure::evaluate(ostream & file_buffer)
{
	Local_Environment & eval_env = *new Local_Environment();
	local_symbol_table.create(eval_env);

	Eval_Result * result = NULL;

	file_buffer << PROC_SPACE << "Evaluating Procedure " << name << "\n";
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
	file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating):\n";
	eval_env.print(file_buffer);

	return *result;
}
*/
void Procedure::compile()
{
	// assign offsets to local symbol table
	argument_symbol_table.set_start_offset_of_first_symbol(8);
	argument_symbol_table.set_size(4);
	argument_symbol_table.assign_offsets();

	// assign offsets to local symbol table
	local_symbol_table.set_start_offset_of_first_symbol(4);
	local_symbol_table.set_size(4);
	local_symbol_table.assign_offsets();

	// compile the program by visiting each basic block
	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++){
		(*i)->compile();
	}

}

void Procedure::print_icode(ostream & file_buffer)
{
	file_buffer << "  Procedure: " << name << "\n";
	file_buffer << "  Intermediate Code Statements\n";

	list<Basic_Block *>::iterator i;
	for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		(*i)->print_icode(file_buffer);
}

void Procedure::print_assembly(ostream & file_buffer)
{
	print_prologue(file_buffer);

	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		(*i)->print_assembly(file_buffer);

	print_epilogue(file_buffer);
}

void Procedure::print_prologue(ostream & file_buffer)
{
	stringstream prologue;

	prologue << "\n\
	.text \t\t\t# The .text assembler directive indicates\n\
	.globl " << name << "\t\t# The following is the code (as oppose to data)\n";

	prologue << name << ":\t\t\t\t# .globl makes main know to the \n\t\t\t\t# outside of the program.\n\
# Prologue begins \n\
	sw $ra, 0($sp)\t\t# Save the return address\n\
	sw $fp, -4($sp)\t\t# Save the frame pointer\n\
	sub $fp, $sp, 8\t\t# Update the frame pointer\n";

	int size = local_symbol_table.get_size();
	size = -size;
	if (size > 0)
		prologue << "\n\tsub $sp, $sp, " << (size + 8) << "\t\t# Make space for the locals\n";
	else
		prologue << "\n\tsub $sp, $sp, 8\t\t# Make space for the locals\n";

	prologue << "# Prologue ends\n\n";

	file_buffer << prologue.str();
}

void Procedure::print_epilogue(ostream & file_buffer)
{
	stringstream epilogue;

	int size = local_symbol_table.get_size();
	size = -size;
	epilogue << "\n# Epilogue Begins\n";
	epilogue << "epilogue_" << name << ":\n";
	if (size > 0)
		epilogue << "\tadd $sp, $sp, " << (size + 8) << "\n";
	else
		epilogue << "\tadd $sp, $sp, 8\n";

	epilogue << "\tlw $fp, -4($sp)  \n\tlw $ra, 0($sp)   \n\tjr        $31\t\t# Jump back to the called procedure\n# Epilogue Ends\n\n";

	file_buffer << epilogue.str();
}
