
/*******************************************************************************************

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


**********************************************************************************************/

%scanner ../scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union
{
	int integer_value;
	float float_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Argument_Table * argument_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	list<Basic_Block *> * basic_block_list;
	Procedure * procedure;
};

%token <integer_value> INTEGER_NUMBER
%token <integer_value> BASIC_BLOCK
%token <float_value> FLOAT_NUMBER
%token <string_value> NAME
%token RETURN INTEGER FLOAT DOUBLE VOID IF ELSE GOTO ASSIGN_OP
%left ne eq
%left lt le gt ge
%left ADD_OP SUB_OP
%left MULT_OP DIV_OP
%token nt andTok orTok
%type <argument_table> argument_list
%type <symbol_entry> argument
%type <symbol_table> declaration_statement_list
%type <symbol_entry> declaration_statement
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
%type <ast_list> argument_input_list
%type <ast> if_else_statement
%type <ast> goto_statement
%type <ast> assignment_statement_or_function_call
%type <ast> assignment_statement
%type <ast> relational_expression
%type <ast> expression
%type <ast> arithmetic_expression
%type <ast> function_call
%type <ast> return_value
%type <ast> and_expression
%type <ast> or_expression
%type <ast> not_expression
%type <ast> variable_or_constant
%type <ast> variable_or_constant_typecast
%type <ast> variable
%type <ast> constant






%start program

%%

program:
	declaration_statement_list function_declaration_statement_list{		
		program_object.set_global_table(*$1);
		return_statement_used_flag = true;				// Do not check for return here, checked in the procedure file
		
	} procedure_list

|	
	function_declaration_statement_list 
	{		
		return_statement_used_flag = true;				// Do not check for return here, checked in the procedure file
		
	} procedure_list{

		bool functionDefined = false ;
		int j = 0 ;
		for(int i = 0 ; i < functionCall.size() ; i++){
			for(j = 0 ; j < function_body_defined.size() ; j++){
				if(function_body_defined[j].compare(functionCall[i]) == 0){
					functionDefined = true;
					break;
				}
			}
			if(functionDefined == false){
				report_error("Procedure called is not Defined" , -1);
			}
		}

	}
|	
	declaration_statement_list 
	{

		program_object.set_global_table(*$1);
		return_statement_used_flag = true;				// Do not check for return here, checked in the procedure file

	} procedure_list {

		bool functionDefined = false ;
		int j = 0 ;
		for(int i = 0 ; i < functionCall.size() ; i++){
			for(j = 0 ; j < function_body_defined.size() ; j++){
				if(function_body_defined[j].compare(functionCall[i]) == 0){
					functionDefined = true;
					break;
				}
			}
			if(functionDefined == false){
				report_error("Procedure called is not Defined" , -1);
			}
		}

	}
|
	procedure_list
	{
		bool functionDefined = false ;
		int j = 0 ;
		for(int i = 0 ; i < functionCall.size() ; i++){
			for(j = 0 ; j < function_body_defined.size() ; j++){
				if(function_body_defined[j].compare(functionCall[i]) == 0){
					functionDefined = true;
					break;
				}
			}
			if(functionDefined == false){
				report_error("Procedure called is not Defined" , -1);
			}
		}
	}
;
procedure_list:	
	procedure_list	procedure_name
	{
		
		return_statement_used_flag = true;				// No return statement in the current procedure till now
		
	}

	procedure_body
	{
		
		string var_name = current_procedure->get_proc_name();
		if(current_procedure->get_return_type() != void_data_type && return_statement_used_flag == true){
				int line = get_line_number();
				report_error("Last return statement type, of procedure, and its prototype should match", line);
			}

	}
|
	procedure_name
	{
		
		return_statement_used_flag = true;				// No return statement in the current procedure till now
		
	}

	procedure_body
	{
		string var_name = current_procedure->get_proc_name();

		if(current_procedure->get_return_type() != void_data_type  && return_statement_used_flag == true){
			int line = get_line_number();
			report_error("Last return statement type, of procedure, and its prototype should match", line);
		}
		
		
	}
;

procedure_name:
	NAME '(' argument_list ')'
	{
		
		function_body_defined.push_back(*$1);

		if(program_object.variable_in_symbol_list_check(*$1)){
			int line = get_line_number();
			report_error("Procedure name cannot be same as global variable",line);
		}

		int line = get_line_number();
		program_object.check_procedure_predefined(*$1,line);
		current_procedure = program_object.get_procedure(*$1); 
		
		current_procedure->get_argument_symbol_table().symbol_table_entry_check((*$3).variable_table);

		
	}
|	
	NAME '(' ')'
	{
		function_body_defined.push_back(*$1);

		if(program_object.variable_in_symbol_list_check(*$1)){
			int line = get_line_number();
			report_error("Procedure name cannot be same as global variable",line);
		}

		int line  = get_line_number();
		program_object.check_procedure_predefined(*$1,line);
		current_procedure = program_object.get_procedure(*$1); 

	current_procedure->get_argument_symbol_table().symbol_table_entry_check((*(new Argument_Table())).variable_table);		
	}
;

procedure_body:
	'{' declaration_statement_list
	{
	
		current_procedure->set_local_list(*$2);
		if(current_procedure->check_parameter_local_var()){
			int line = get_line_number();
			report_error("Formal parameter and local variable name cannot be same", line);			
		}
		delete $2;
		

		 
		
	}
	basic_block_list '}'
	{
		 
		/*if (return_statement_used_flag == false)
		{
			
			int line = get_line_number();
			report_error("Atleast 1 basic block should have a return statement", line);
			

		}*/

		
		current_procedure->set_basic_block_list(*$4);

		delete $4;
		

		 
		
	}
|
	'{' basic_block_list '}'
	{
		 

		current_procedure->set_basic_block_list(*$2);

		delete $2;

		 
		
	}
;

function_declaration_statement_list:
	function_declaration_statement
	{
		program_object.set_procedure_map(*current_procedure);
	}
|	
	function_declaration_statement_list function_declaration_statement
	{
		program_object.set_procedure_map(*current_procedure);
	}
;


function_declaration_statement:
	INTEGER NAME '(' argument_list ')' ';'
	{

		current_procedure = new Procedure(int_data_type, *$2 , *$4);	
	
	}
|	
	FLOAT NAME '(' argument_list ')' ';'
	{

		current_procedure = new Procedure(float_data_type, *$2 , *$4);
	}
|
	DOUBLE NAME '(' argument_list ')' ';'
	{

		current_procedure = new Procedure(float_data_type, *$2 , *$4);
	}
|
	VOID NAME '(' argument_list ')' ';'
	{

		current_procedure = new Procedure(void_data_type, *$2 , *$4);
	}
|
	INTEGER NAME '(' ')' ';'
	{

		current_procedure = new Procedure(int_data_type, *$2 , *(new Argument_Table()));
	}
|	
	FLOAT NAME '('')' ';'
	{
\
		current_procedure = new Procedure(float_data_type, *$2 , *(new Argument_Table()));
	}
|
	DOUBLE NAME '(' ')' ';'
	{

		current_procedure = new Procedure(float_data_type, *$2 , *(new Argument_Table()));
	}
|
	VOID NAME '('')' ';'
	{

		current_procedure = new Procedure(void_data_type, *$2 , *(new Argument_Table()));
	}
;
argument_list:
	argument_list ',' argument
	{
		if ($1 != NULL)
		{

			$$ = $1;
		}
		else
			$$ = new Argument_Table();

		if($$->variable_in_symbol_list_check((*$3).get_variable_name())) {
			int line= get_line_number();
			report_error("Formal Parameter declared twice" , line);
		}
		int line= get_line_number();
		program_object.variable_in_proc_map_check($3->get_variable_name(), line);
		$$->push_symbol($3);
	}
|	
	argument
	{
		int line= get_line_number();
		program_object.variable_in_proc_map_check($1->get_variable_name(), line);
		$$ = new Argument_Table();
		$$->push_symbol($1);
	}
;

argument:
	INTEGER NAME 
	{

		$$ = new Symbol_Table_Entry(*$2, int_data_type);

		delete $2;
	}
|
	FLOAT NAME 
	{

		$$ = new Symbol_Table_Entry(*$2, float_data_type);

		delete $2;
		

	}
|
	DOUBLE NAME 
	{
		
		$$ = new Symbol_Table_Entry(*$2, float_data_type);

		delete $2;
		

	}
;

declaration_statement_list:
	declaration_statement
	{
		
		int line = get_line_number();
		program_object.variable_in_proc_map_check($1->get_variable_name(), line);

		string var_name = $1->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
			int line = get_line_number();
			report_error("Variable name cannot be same as procedure name", line);
		}

		
		$$ = new Symbol_Table();
		$$->push_symbol($1);
		 
	}
|
	declaration_statement_list declaration_statement
	{
		
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list
		
		int line = get_line_number();
		program_object.variable_in_proc_map_check($2->get_variable_name(), line);

		string var_name = $2->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
			int line = get_line_number();
			report_error("Variable name cannot be same as procedure name", line);
		} 


		if ($1 != NULL)
		{
			if($1->variable_in_symbol_list_check(var_name))
			{
				int line = get_line_number();
				report_error("Variable is declared twice", line);
			}

			$$ = $1;
		}

		else
			$$ = new Symbol_Table();

		$$->push_symbol($2);

		 
		
	}
;

declaration_statement:
	INTEGER NAME ';'
	{
		
		$$ = new Symbol_Table_Entry(*$2, int_data_type);

		delete $2;
		

	}
|
	FLOAT NAME ';'
	{
		
		$$ = new Symbol_Table_Entry(*$2, float_data_type);

		delete $2;
		
	}
|
	DOUBLE NAME ';'
	{
		
		$$ = new Symbol_Table_Entry(*$2, float_data_type);

		delete $2;
		
	}
;


basic_block_list:
	basic_block_list basic_block
	{
		
		if (!$2)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check($$, $2->get_bb_number());

		$$ = $1;
		if($2 != NULL){
			$$->back()->isSuccessor = 1;
		}
		$$->push_back($2);
		
				 
		
	}
|
	basic_block
	{
		 
		if (!$1)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
		
	}
;

basic_block:
	BASIC_BLOCK ':' executable_statement_list
	{
		
		if ($1 < 2)
		{
			int line = get_line_number();
			printf("%d \n", $1);
			report_error("Illegal basic block lable", line);
		}
		if ($3 != NULL){
			$$ = new Basic_Block($1, *$3);
			if($3->back()->get_successor() != -1){
				$$->isSuccessor = 1;
			}
		}
		else
		{
			list<Ast *> * ast_list = new list<Ast *>;
			$$ = new Basic_Block($1, *ast_list);
		}

		delete $3;
		
	}
;

executable_statement_list:
	assignment_statement_list
	{
		 
		$$ = $1;
		
		 
		
	}
|
	assignment_statement_list RETURN  return_value ';'
	{
		
		if($3 != NULL){
			return_statement_used_flag = false;
		}
		Ast * ret = new Return_Ast($3);

		if ($1 != NULL)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back(ret);
		
		
	}
|
	assignment_statement_list if_else_statement
	{
		 
		if ($1 != NULL)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back($2);

		 
	}
|
	assignment_statement_list goto_statement
	{
		
		if ($1 != NULL)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back($2);
		
	}
;



assignment_statement_list:
	{
		
		$$ = NULL;
		
		
	}
|
	assignment_statement_list assignment_statement_or_function_call
	{
		 
		if ($1 == NULL)
			$$ = new list<Ast *>;

		else
			$$ = $1;

		$$->push_back($2);
		
		 
		
	}
;

assignment_statement_or_function_call:
	assignment_statement
	{
		$$ = $1;
	}
|
	function_call ';'
	{
		$$ = $1;
	}
;

return_value:
	and_expression
	{
		if(current_procedure->get_proc_name() != "main" && current_procedure->get_return_type() != $1->get_data_type()){
			int line = get_line_number();
			report_error("Last return statement type, of procedure, and its prototype should match", line);
		}

		$$ =  $1;
	}
|
	{
		$$ = NULL;
	}
;

assignment_statement:
	variable ASSIGN_OP and_expression ';'
	{
		
		$$ = new Assignment_Ast($1, $3);

		int line = get_line_number();
		$$->check_ast(line);
		
		
	}
;

if_else_statement:
	IF '(' relational_expression ')'
		goto_statement
	ELSE
		goto_statement
	{
			 
		$$ = new If_Else_Ast($5, $3, $7);
		
		
	}
;

goto_statement:
	GOTO BASIC_BLOCK ';'
	{
		
		if ($2 < 2)
		{
			int line = get_line_number();
			//report_error("Illegal basic block lable", line);
		}
		
		$$ = new Goto_Ast($2);
		
		 
	}
;


not_expression:	
	arithmetic_expression
	{	
		$$ = $1;
		
	}
|
	relational_expression
	{
		 
		
		$$ = $1;
		
		
		 
	}
|
	nt relational_expression
	{
		 
		$$ = new Relational_Expr_Ast($2, NOT, NULL);
			
		 
	}

;
or_expression:
	not_expression
	{	
		 
		$$ = $1;
		
		
		 
	}
|
	not_expression orTok or_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, OR, $3);
		
		 
	}
;

and_expression:
	or_expression
	{
		
		$$ = $1;
		
	}
|
	or_expression andTok and_expression
	{
		
		$$ = new Relational_Expr_Ast($1, AND, $3);
		
	}
;

relational_expression:
	expression eq expression
	{
		
		$$ = new Relational_Expr_Ast($1, EQ, $3);
		
	}
|
	expression ne expression
	{
		
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
	}
|
	expression le expression
	{
		
		$$ = new Relational_Expr_Ast($1, LE, $3);
		
	}
|
	expression ge expression
	{
		
		$$ = new Relational_Expr_Ast($1, GE, $3);
		
	}
|
	expression lt expression
	{
		
		$$ = new Relational_Expr_Ast($1, LT, $3);
		
	}
|
	expression gt expression
	{
		
		$$ = new Relational_Expr_Ast($1, GT, $3);
		
	}

;

expression:
	arithmetic_expression
	{
		$$ =  $1;
	}
|	
	relational_expression
	{
		$$ = $1;
	}

;
arithmetic_expression:
	SUB_OP variable_or_constant_typecast 
	{
		
		$$ = new Arithmetic_Expr_Ast($2, UMINUS, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
|
	arithmetic_expression ADD_OP arithmetic_expression
	{
		
		$$ = new Arithmetic_Expr_Ast($1, PLUS, $3);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
|
	arithmetic_expression SUB_OP arithmetic_expression
	{
		
		$$ = new Arithmetic_Expr_Ast($1, MINUS, $3);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
|
	arithmetic_expression MULT_OP arithmetic_expression
	{
		
		$$ = new Arithmetic_Expr_Ast($1, MULT, $3);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
|
	arithmetic_expression DIV_OP arithmetic_expression
	{
		
		$$ = new Arithmetic_Expr_Ast($1, DIV, $3);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
|
	variable_or_constant_typecast
	{
		
		$$ = new Arithmetic_Expr_Ast($1, VAR, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
;

variable_or_constant_typecast:
	'(' FLOAT ')' variable_or_constant
	{
		
		$$ = new Arithmetic_Expr_Ast($4, F_NUM, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		
	}

|
	'(' DOUBLE ')' variable_or_constant
	{
		
		$$ = new Arithmetic_Expr_Ast($4, F_NUM, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
|
	'(' INTEGER ')' variable_or_constant
	{
		
		$$ = new Arithmetic_Expr_Ast($4, I_NUM, NULL);
		int line = get_line_number();
		$$->check_ast(line);
		
	}
|

	variable_or_constant
	{
		
		$$ = $1;
		
	}
;

function_call:
	NAME '(' argument_input_list ')'
	{
		functionCall.push_back(*$1);

		if (program_object.get_procedure(*$1) == NULL){
			int line = get_line_number();
			report_error("Function is not defined",line);
		}
		else
			$$ = new Function_Call_Ast(*$1,program_object.get_procedure(*$1)->get_return_type(),$3);
	}
|
	NAME '(' ')'
	{
		functionCall.push_back(*$1);

		if (program_object.get_procedure(*$1) == NULL){
			int line = get_line_number();
			report_error("Function is not defined",line);
		}
		else{
			$$ = new Function_Call_Ast(*$1,int_data_type,NULL);
		}
	}
;
argument_input_list:
	argument_input_list ',' not_expression
	{
		if ($1 == NULL)
			$$ = new list<Ast *>;
		else
			$$ = $1;

		$$->push_back($3);
	}
|
	not_expression
	{
		$$ = new list<Ast *>;
		$$->push_back($1);
	}
;

variable_or_constant:
	variable
	{
		
		$$ = $1;
		
	}
|
	constant
	{
		
		$$ = $1;
		
	}
|
	function_call
	{
		$$ = $1;
	}
|
	'('arithmetic_expression')'
	{
		
		$$ = $2;
		
	}
|
	'('relational_expression')'
	{
		
		$$ = $2;
		
	}
;

variable:
	NAME
	{

		
		Symbol_Table_Entry var_table_entry;

		if (current_procedure->variable_in_symbol_list_check(*$1))
			var_table_entry = current_procedure->get_symbol_table_entry(*$1);
		else if (current_procedure->variable_in_argument_list_check(*$1))
			var_table_entry = current_procedure->get_argument_table_entry(*$1);
		else if (program_object.variable_in_symbol_list_check(*$1)){
			var_table_entry = program_object.get_symbol_table_entry(*$1);
			int line = get_line_number();
			}
		else
		{
			int line = get_line_number();
			report_error("Variable has not been declared", line);
		}

		$$ = new Name_Ast(*$1, var_table_entry);

		delete $1;
		
	}
;

constant:
	INTEGER_NUMBER
	{
		
		$$ = new Number_Ast<int>($1, int_data_type);
		
	}
|	FLOAT_NUMBER
	{
		
		$$ = new Number_Ast<float>($1, float_data_type);
		
	}
;
