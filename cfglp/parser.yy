
/********************************************************************************************

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
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	list<Basic_Block *> * basic_block_list;
	Procedure * procedure;
};

%token <integer_value> INTEGER_NUMBER
%token <float_value> FLOAT_NUMBER
%token <integer_value> BASIC_BLOCK
%token <string_value> NAME
%token RETURN INTEGER IF ELSE GOTO ASSIGN_OP FLOAT
%left MULT_OP DIV_OP
%left ADD_OP SUB_OP
%left ne eq
%left lt le gt ge
%token nt andTok orTok 
%type <symbol_table> declaration_statement_list
%type <symbol_entry> declaration_statement
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
%type <ast> if_else_statement
%type <ast> goto_statement
%type <ast> assignment_statement
%type <ast> relational_expression
%type <ast> arithmetic_expression
%type <ast> and_expression
%type <ast> or_expression
%type <ast> not_expression
%type <ast> variable_or_constant
%type <ast> variable
%type <ast> constant




%start program

%%

program:
	declaration_statement_list procedure_name
	{

		
		program_object.set_global_table(*$1);
		return_statement_used_flag = true;				// Do not check for return here, checked in the procedure file
		
	}
	procedure_body
	{
		
		program_object.set_procedure_map(*current_procedure);

		if ($1)
			$1->global_list_in_proc_map_check(get_line_number());

		delete $1;
		
		 
		
	}
|
	procedure_name
	{
			 
		return_statement_used_flag = true;				// No return statement in the current procedure till now
		 
		
	}
	procedure_body
	{
		 
		program_object.set_procedure_map(*current_procedure);

		 
		
	}
;

procedure_name:
	NAME '(' ')'
	{
		 
		current_procedure = new Procedure(void_data_type, *$1);	 
		
	}
;

procedure_body:
	'{' declaration_statement_list
	{
		 
		current_procedure->set_local_list(*$2);
		delete $2;

		 
		
	}
	basic_block_list '}'
	{
		 
		if (return_statement_used_flag == false)
		{
			int line = get_line_number();
			report_error("Atleast 1 basic block should have a return statement", line);

		}

		current_procedure->set_basic_block_list(*$4);

		delete $4;

		 
		
	}
|
	'{' basic_block_list '}'
	{
		 
		if (return_statement_used_flag == false)
		{
			int line = get_line_number();
			report_error("Atleast 1 basic block should have a return statement", line);
		}

		current_procedure->set_basic_block_list(*$2);

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
	assignment_statement_list RETURN ';'
	{
		 
		Ast * ret = new Return_Ast();

		return_statement_used_flag = true;					// Current procedure has an occurrence of return statement

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
	assignment_statement_list assignment_statement
	{
		 
		if ($1 == NULL)
			$$ = new list<Ast *>;

		else
			$$ = $1;

		$$->push_back($2);
		 
		
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

	arithmetic_expression le relational_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, LE, $3);
		
		 
	}	
|
	arithmetic_expression ge relational_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, GE, $3);
		
		 
	}
|
	arithmetic_expression lt relational_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, LT, $3);
		
		 
	}
|
	arithmetic_expression gt relational_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, GT, $3);
		
		 
	}
|
	arithmetic_expression eq relational_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, EQ, $3);
		
		 
	}
|
	arithmetic_expression ne relational_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
		 
	}
|
	arithmetic_expression le arithmetic_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
		 
	}
|
	arithmetic_expression ge arithmetic_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
		 
	}
|
	arithmetic_expression gt arithmetic_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
		 
	}
|
	arithmetic_expression lt arithmetic_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
		 
	}
|
	arithmetic_expression eq arithmetic_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
		 
	}
|
	arithmetic_expression ne arithmetic_expression
	{
		 
		$$ = new Relational_Expr_Ast($1, NE, $3);
		
		 
	}

;

arithmetic_expression:
	SUB_OP variable_or_constant
	{
		$$ = new Arithmetic_Expr_Ast($2, UMINUS, NULL);
	}	
|
	'(' FLOAT ')' arithmetic_expression
	{
		$$ = new Arithmetic_Expr_Ast($4, F_NUM, NULL);
	}	
|
	'(' INTEGER ')' arithmetic_expression
	{
		$$ = new Arithmetic_Expr_Ast($4, I_NUM, NULL);
	}	
|
	variable_or_constant ADD_OP arithmetic_expression
	{
		$$ = new Arithmetic_Expr_Ast($1, PLUS, $3);
	}
|
	variable_or_constant SUB_OP arithmetic_expression
	{
		$$ = new Arithmetic_Expr_Ast($1, MINUS, $3);
	}
|
	variable_or_constant MULT_OP arithmetic_expression
	{
		$$ = new Arithmetic_Expr_Ast($1, MULT, $3);
	}
|
	variable_or_constant DIV_OP arithmetic_expression
	{
		$$ = new Arithmetic_Expr_Ast($1, DIV, $3);
	}
|
	variable_or_constant ADD_OP variable_or_constant
	{
		 
		$$ = new Arithmetic_Expr_Ast($1, PLUS, $3);
		
		 
	}
|
	variable_or_constant SUB_OP variable_or_constant
	{
		 
		$$ = new Arithmetic_Expr_Ast($1, MINUS, $3);
				 
	}
|
	variable_or_constant MULT_OP variable_or_constant
	{
		 
		$$ = new Arithmetic_Expr_Ast($1, MULT, $3);
		
		 
	}
|
	variable_or_constant DIV_OP variable_or_constant
	{
		 
		$$ = new Arithmetic_Expr_Ast($1, DIV, $3);
		
		 
	}
|	
	variable_or_constant
	{
		$$ = new Arithmetic_Expr_Ast($1, VAR, NULL);
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

		else if (program_object.variable_in_symbol_list_check(*$1))
			var_table_entry = program_object.get_symbol_table_entry(*$1);

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
