
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

%token <integer_value> INTEGER_NUMBER BBNUM
%token <float_value> FLOAT_NUMBER
%token <string_value> NAME
%token RETURN INTEGER FLOAT DOUBLE VOID IF ELSE GOTO ASSIGN_OP
%left ne eq
%left lt le gt ge
%left ADD_OP SUB_OP
%left MULT_OP DIV_OP
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
%type <ast> variable_or_constant
%type <ast> variable_or_constant_typecast
%type <ast> variable
%type <ast> constant






%start program

%%

program:
	declaration_statement_list function_declaration_statement_list{
		if (NOT_ONLY_PARSE)
		{
			program_object.set_global_table(*$1);
			return_statement_used_flag = true;
		}
	} procedure_list

|
	function_declaration_statement_list
	{
		if (NOT_ONLY_PARSE)
		{
			return_statement_used_flag = true;
		}

	} procedure_list{
		if (NOT_ONLY_PARSE)
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
				CHECK_INVARIANT(functionDefined != false, "Procedure called is not Defined");
			}
		}
	}
|
	declaration_statement_list
	{
		if (NOT_ONLY_PARSE)
		{
			program_object.set_global_table(*$1);
			return_statement_used_flag = true;
		}

	} procedure_list {

		if (NOT_ONLY_PARSE)
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
				CHECK_INVARIANT(functionDefined != false, "Procedure called is not Defined");
			}
		}
	}
|
	procedure_list
	{
		if(NOT_ONLY_PARSE)
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
				CHECK_INVARIANT(functionDefined != false, "Procedure called is not Defined");
			}
		}
	}
;
procedure_list:
	procedure_list	procedure_name
	{

		if(NOT_ONLY_PARSE)
		{
			return_statement_used_flag = true;
		}
	}

	procedure_body
	{

		if(NOT_ONLY_PARSE)
		{
			string var_name = current_procedure->get_proc_name();
			CHECK_INVARIANT((current_procedure->get_return_type() == void_data_type || return_statement_used_flag == false),"Last return statement type, of procedure, and its prototype should match");
		}

	}
|
	procedure_name
	{
		if(NOT_ONLY_PARSE)
		{
			return_statement_used_flag = true;
		}

	}

	procedure_body
	{
		if(NOT_ONLY_PARSE)
		{
			string var_name = current_procedure->get_proc_name();
			CHECK_INVARIANT((current_procedure->get_return_type() == void_data_type || return_statement_used_flag == false),"Last return statement type, of procedure, and its prototype should match");
		}
	}
;

procedure_name:
	NAME '(' argument_list ')'
	{
		if(NOT_ONLY_PARSE)
		{
			function_body_defined.push_back(*$1);

			CHECK_INVARIANT(program_object.variable_in_symbol_list_check(*$1) == false, "Procedure name cannot be same as global variable");

			program_object.check_procedure_predefined(*$1);
			current_procedure = program_object.get_procedure(*$1);

			current_procedure->get_argument_symbol_table().symbol_table_entry_check((*$3).variable_table);

			curr_function_name = *$1;
		}

	}
|
	NAME '(' ')'
	{
		if(NOT_ONLY_PARSE)
		{
			function_body_defined.push_back(*$1);

			CHECK_INVARIANT(program_object.variable_in_symbol_list_check(*$1) == false, "Procedure name cannot be same as global variable");

			program_object.check_procedure_predefined(*$1);
			current_procedure = program_object.get_procedure(*$1);
			current_procedure->get_argument_symbol_table().symbol_table_entry_check((*(new Argument_Table())).variable_table);

			curr_function_name = *$1;
		}
	}
;

procedure_body:
	'{' declaration_statement_list
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure->set_local_list(*$2);
			CHECK_INVARIANT(current_procedure->check_parameter_local_var() == false, "Formal parameter and local variable name cannot be same");
			delete $2;
		}
	}
	basic_block_list '}'
	{

		if(NOT_ONLY_PARSE)
		{
			current_procedure->set_basic_block_list(*$4);
			delete $4;
		}
	}
|
	'{' basic_block_list '}'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure->set_basic_block_list(*$2);
			delete $2;
		}
	}
;

function_declaration_statement_list:
	function_declaration_statement
	{
		if(NOT_ONLY_PARSE)
		{
			program_object.set_procedure_map(current_procedure, get_line_number());
		}
	}
|
	function_declaration_statement_list function_declaration_statement
	{
		if(NOT_ONLY_PARSE)
		{
			program_object.set_procedure_map(current_procedure, get_line_number());
		}
	}
;


function_declaration_statement:
	INTEGER NAME '(' argument_list ')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(int_data_type, *$2 , *$4, get_line_number());
		}

	}
|
	FLOAT NAME '(' argument_list ')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(float_data_type, *$2 , *$4, get_line_number());
		}
	}
|
	DOUBLE NAME '(' argument_list ')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(float_data_type, *$2 , *$4,get_line_number());
		}
	}
|
	VOID NAME '(' argument_list ')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(void_data_type, *$2 , *$4, get_line_number());
		}
	}
|
	INTEGER NAME '(' ')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(int_data_type, *$2 , *(new Argument_Table()), get_line_number());
		}
	}
|
	FLOAT NAME '('')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(float_data_type, *$2 , *(new Argument_Table()),get_line_number());
		}
	}
|
	DOUBLE NAME '(' ')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(float_data_type, *$2 , *(new Argument_Table()), get_line_number());
		}
	}
|
	VOID NAME '('')' ';'
	{
		if(NOT_ONLY_PARSE)
		{
			current_procedure = new Procedure(void_data_type, *$2 , *(new Argument_Table()),get_line_number());
		}
	}
;
argument_list:
	argument_list ',' argument
	{
		if(NOT_ONLY_PARSE)
		{
			if ($1 != NULL)
			{

				$$ = $1;
			}
			else
				$$ = new Argument_Table();

			CHECK_INVARIANT($$->variable_in_symbol_list_check((*$3).get_variable_name()) == false, "Formal Parameter declared twice");
			program_object.variable_in_proc_map_check($3->get_variable_name());
			$$->push_symbol($3);
		}
	}
|
	argument
	{
		if(NOT_ONLY_PARSE)
		{
			program_object.variable_in_proc_map_check($1->get_variable_name());
			$$ = new Argument_Table();
			$$->push_symbol($1);
		}
	}
;

argument:
	INTEGER NAME
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Symbol_Table_Entry(*$2, int_data_type, get_line_number());

			delete $2;
		}
	}
|
	FLOAT NAME
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Symbol_Table_Entry(*$2, float_data_type, get_line_number());

			delete $2;
		}
	}
|
	DOUBLE NAME
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Symbol_Table_Entry(*$2, float_data_type, get_line_number());

			delete $2;
		}

	}
;

declaration_statement_list:
	declaration_statement
	{
		if(NOT_ONLY_PARSE)
		{
			program_object.variable_in_proc_map_check($1->get_variable_name());

			string var_name = $1->get_variable_name();
			CHECK_INVARIANT((current_procedure && current_procedure->get_proc_name() == var_name) == false, "Variable name cannot be same as procedure name");

			$$ = new Symbol_Table();
			$$->push_symbol($1);
		}
	}
|
	declaration_statement_list declaration_statement
	{
		if(NOT_ONLY_PARSE)
		{
			// if declaration is local then no need to check in global list
			// if declaration is global then this list is global list

			program_object.variable_in_proc_map_check($2->get_variable_name());

			string var_name = $2->get_variable_name();
			CHECK_INVARIANT((current_procedure && current_procedure->get_proc_name() == var_name) == false, "Variable name cannot be same as procedure name");

			if ($1 != NULL)
			{
				CHECK_INVARIANT($1->variable_in_symbol_list_check(var_name) == false, "Variable is declared twice");

				$$ = $1;
			}

			else
				$$ = new Symbol_Table();

			$$->push_symbol($2);
		}
	}
;

declaration_statement:
	INTEGER NAME ';'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Symbol_Table_Entry(*$2, int_data_type,get_line_number());

			delete $2;
		}
	}
|
	FLOAT NAME ';'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Symbol_Table_Entry(*$2, float_data_type, get_line_number());

			delete $2;
		}

	}
|
	DOUBLE NAME ';'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Symbol_Table_Entry(*$2, float_data_type, get_line_number());

			delete $2;
		}

	}
;


basic_block_list:
	basic_block_list basic_block
	{
		if(NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((!$2) == false, "Basic block doesn't exist");

			bb_strictly_increasing_order_check($$, $2->get_bb_number());

			$$ = $1;
			$$->push_back($2);
		}
	}
|
	basic_block
	{
		if(NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((!$1) == false, "Basic block doesn't exist");

			$$ = new list<Basic_Block *>;
			$$->push_back($1);
		}
	}
;

basic_block:
	BBNUM ':' executable_statement_list
	{
		if(NOT_ONLY_PARSE)
		{
			int bb_number = $1;
			list<Ast *> * exe_stmt = $3;

			CHECK_INPUT((bb_number >= 2), "Illegal basic block lable", get_line_number());

			Basic_Block * bb = new Basic_Block(bb_number, get_line_number());

			if (exe_stmt != NULL)
				bb->set_ast_list(*exe_stmt);
			else
			{
				list<Ast *> * ast_list = new list<Ast *>;
				bb->set_ast_list(*ast_list);
			}

			$$ = bb;
		}
	}
;

executable_statement_list:
	assignment_statement_list
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
|
	assignment_statement_list RETURN  return_value ';'
	{

		if(NOT_ONLY_PARSE)
		{

			if($3 != NULL){
				return_statement_used_flag = false;
			}
			Ast * ret = new Return_Ast($3, curr_function_name, get_line_number());

			if ($1 != NULL)
				$$ = $1;

			else
				$$ = new list<Ast *>;

			$$->push_back(ret);
		}
	}
|
	assignment_statement_list if_else_statement
	{
		if(NOT_ONLY_PARSE)
		{
			if ($1 != NULL)
				$$ = $1;

			else
				$$ = new list<Ast *>;

			$$->push_back($2);
		}
	}
|
	assignment_statement_list goto_statement
	{
		if(NOT_ONLY_PARSE)
		{
			if ($1 != NULL)
				$$ = $1;

			else
				$$ = new list<Ast *>;

			$$->push_back($2);
		}

	}
;



assignment_statement_list:
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = NULL;
		}
	}
|
	assignment_statement_list assignment_statement_or_function_call
	{
		if(NOT_ONLY_PARSE)
		{
			if ($1 == NULL)
				$$ = new list<Ast *>;

			else
				$$ = $1;

			$$->push_back($2);
		}
	}
;

assignment_statement_or_function_call:
	assignment_statement
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
|
	function_call ';'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
;

return_value:
	expression
	{
		if(NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((current_procedure->get_proc_name() != "main" && current_procedure->get_return_type() != $1->get_data_type()) == false,"Last return statement type, of procedure, and its prototype should match");
			$$ =  $1;
		}
	}
|
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = NULL;
		}
	}
;

assignment_statement:
	variable ASSIGN_OP expression ';'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Assignment_Ast($1, $3, get_line_number());
			$$->check_ast();
		}
	}
;

if_else_statement:
	IF '(' relational_expression ')'
		goto_statement
	ELSE
		goto_statement
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new If_Else_Ast($5, $3, $7, get_line_number());
		}
	}
;

goto_statement:
	GOTO BBNUM ';'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Goto_Ast($2, get_line_number());
		}

	}
;


expression:
	arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}

	}
|
	relational_expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
;

relational_expression:
	expression eq expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Relational_Expr_Ast($1, EQ, $3, get_line_number());
		}
	}
|
	expression ne expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Relational_Expr_Ast($1, NE, $3, get_line_number());
		}
	}
|
	expression le expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Relational_Expr_Ast($1, LE, $3, get_line_number());
		}
	}
|
	expression ge expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Relational_Expr_Ast($1, GE, $3, get_line_number());
		}
	}
|
	expression lt expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Relational_Expr_Ast($1, LT, $3, get_line_number());
		}
	}
|
	expression gt expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Relational_Expr_Ast($1, GT, $3, get_line_number());
		}
	}

;

arithmetic_expression:
	SUB_OP variable_or_constant_typecast
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($2, UMINUS, NULL, get_line_number());
			$$->check_ast();
		}
	}
|
	arithmetic_expression ADD_OP arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($1, PLUS, $3, get_line_number());
			$$->check_ast();
		}
	}
|
	arithmetic_expression SUB_OP arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($1, MINUS, $3, get_line_number());
			$$->check_ast();
		}

	}
|
	arithmetic_expression MULT_OP arithmetic_expression
	{

		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($1, MULT, $3, get_line_number());
			$$->check_ast();
		}

	}
|
	arithmetic_expression DIV_OP arithmetic_expression
	{

		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($1, DIV, $3, get_line_number());
			$$->check_ast();
		}

	}
|
	variable_or_constant_typecast
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}

	}
;

variable_or_constant_typecast:
	'(' FLOAT ')' variable_or_constant
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($4, F_NUM, NULL, get_line_number());
			$$->check_ast();
		}
	}

|
	'(' DOUBLE ')' variable_or_constant
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($4, F_NUM, NULL, get_line_number());
			$$->check_ast();
		}
	}
|
	'(' INTEGER ')' variable_or_constant
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Arithmetic_Expr_Ast($4, I_NUM, NULL, get_line_number());
			$$->check_ast();
		}

	}
|

	variable_or_constant
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
;

function_call:
	NAME '(' argument_input_list ')'
	{
		if(NOT_ONLY_PARSE)
		{
			functionCall.push_back(*$1);

			CHECK_INVARIANT(program_object.get_procedure(*$1) != NULL, "Function is not defined");
			$$ = new Function_Call_Ast(*$1, program_object.get_procedure(*$1)->get_return_type(),$3, get_line_number());
		}
	}
|
	NAME '(' ')'
	{
		if(NOT_ONLY_PARSE)
		{
			functionCall.push_back(*$1);

			CHECK_INVARIANT(program_object.get_procedure(*$1) != NULL, "Function is not defined");
			$$ = new Function_Call_Ast(*$1,program_object.get_procedure(*$1)->get_return_type(),NULL, get_line_number());
		}
	}
;

argument_input_list:
	expression ',' argument_input_list
	{
		if(NOT_ONLY_PARSE)
		{
			if ($3 == NULL)
				$$ = new list<Ast *>;
			else
				$$ = $3;

			$$->push_back($1);
		}
	}
|
	expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new list<Ast *>;
			$$->push_back($1);
		}
	}
;

variable_or_constant:
	variable
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}

	}
|
	constant
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
|
	function_call
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
|
	'('arithmetic_expression')'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $2;
		}

	}
|
	'('relational_expression')'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $2;
		}

	}
;

variable:
	NAME
	{
		if(NOT_ONLY_PARSE)
		{

			Symbol_Table_Entry * var_table_entry;

			CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

			string var_name = *$1;

			if (current_procedure->variable_in_symbol_list_check(var_name) == true){
				var_table_entry = &(current_procedure->get_symbol_table_entry(var_name));
			}
			else if (current_procedure->variable_in_argument_list_check(*$1)){
				var_table_entry = &(current_procedure->get_argument_table_entry(var_name));
			}
			else if (program_object.variable_in_symbol_list_check(var_name) == true){
				var_table_entry = &(program_object.get_symbol_table_entry(var_name));
			}

			else{
				CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared");
			}

			Ast * name_ast = new Name_Ast(var_name, *var_table_entry, get_line_number());

			$$ = name_ast;
		}
	}
;

constant:
	INTEGER_NUMBER
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Number_Ast<int>($1, int_data_type, get_line_number());
		}

	}
|	FLOAT_NUMBER
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Number_Ast<float>($1, float_data_type, get_line_number());
		}
	}
;
