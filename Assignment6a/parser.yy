
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

%token <integer_value> INTEGER_NUMBER BBNUM
%token <float_value> FLOAT_NUMBER
%token <string_value> NAME
%token RETURN INTEGER IF ELSE GOTO ASSIGN_OP FLOAT DOUBLE
%left ne eq
%left lt le gt ge
%left ADD_OP SUB_OP
%left MULT_OP DIV_OP
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
%type <ast> expression
%type <ast> variable_or_constant_typecast
%type <ast> variable_or_constant
%type <ast> variable
%type <ast> constant

%start program

%%

program:
	optional_declaration_list procedure_name procedure_body
	{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
			current_procedure->check_valid_bb();

			program_object.set_procedure_map(current_procedure, get_line_number());
			program_object.global_list_in_proc_map_check();
		}
	}
;

optional_declaration_list:
	{
		if (NOT_ONLY_PARSE)
		{
			Symbol_Table * global_table = new Symbol_Table();
			program_object.set_global_table(*global_table);
		}
	}
|
	declaration_statement_list
	{
		if (NOT_ONLY_PARSE)
		{
			Symbol_Table * global_table = $1;

			CHECK_INVARIANT((global_table != NULL), "Global declarations cannot be null");

			program_object.set_global_table(*global_table);
		}
	}
;



procedure_name:
	NAME '(' ')'
	{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($1 != NULL), "Procedure name cannot be null");

			string proc_name = *$1;

			current_procedure = new Procedure(void_data_type, proc_name, get_line_number());
		}
	}
;

procedure_body:
	'{' declaration_statement_list
	{
		if (NOT_ONLY_PARSE)
		{

			CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");

			Symbol_Table * local_table = $2;

			if (local_table == NULL)
				local_table = new Symbol_Table();

			current_procedure->set_local_list(*local_table);
		}
	}
	basic_block_list '}'
	{

		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
			CHECK_INVARIANT(($4 != NULL), "Basic block list cannot be null");

			current_procedure->set_basic_block_list(*$4);
		}
	}
|
	'{' basic_block_list '}'
	{

		if (NOT_ONLY_PARSE)
		{
			list<Basic_Block *> * bb_list = $2;

			CHECK_INVARIANT((current_procedure != NULL), "Current procedure cannot be null");
			CHECK_INVARIANT((bb_list != NULL), "Basic block list cannot be null");

			current_procedure->set_basic_block_list(*bb_list);
		}
	}
;

declaration_statement_list:
	declaration_statement
	{
		if (NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			program_object.variable_in_proc_map_check($1->get_variable_name());

			string var_name = $1->get_variable_name();
			if (current_procedure != NULL)
			{
				CHECK_INPUT((current_procedure->get_proc_name() != var_name),
				"Variable name cannot be same as procedure name", get_line_number());
			}

			$$ = new Symbol_Table();
			$$->push_symbol($1);

		}

	}
|
	declaration_statement_list declaration_statement
	{
		if (NOT_ONLY_PARSE)
		{
			// if declaration is local then no need to check in global list
			// if declaration is global then this list is global list

			int line = get_line_number();
			program_object.variable_in_proc_map_check($2->get_variable_name());

			string var_name = $2->get_variable_name();
			if (current_procedure != NULL)
			{
				CHECK_INPUT((current_procedure->get_proc_name() != var_name),
					"Variable name cannot be same as procedure name", get_line_number());
			}
			if ($1 != NULL)
			{
				CHECK_INPUT(($1->variable_in_symbol_list_check(var_name) == false),
				"Variable is declared twice", get_line_number());
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
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT(($2 != NULL), "Name cannot be null");
			$$ = new Symbol_Table_Entry(*$2, int_data_type, get_line_number());
		}


	}
;

basic_block_list:
	basic_block_list basic_block
	{
		if (NOT_ONLY_PARSE)
		{
			list<Basic_Block *> * bb_list = $1;
			Basic_Block * bb = $2;

			CHECK_INVARIANT((bb_list != NULL), "New basic block cannot be null");
			CHECK_INVARIANT((bb != NULL), "Basic block cannot be null");

			bb_strictly_increasing_order_check(bb_list, bb->get_bb_number());

			bb_list->push_back($2);
			$$ = bb_list;
		}

	}
|
	basic_block
	{
		if (NOT_ONLY_PARSE)
		{
			Basic_Block * bb = $1;

			CHECK_INVARIANT((bb != NULL), "Basic block cannot be null");

			list<Basic_Block *> * bb_list = new list<Basic_Block *>;
			bb_list->push_back(bb);

			$$ = bb_list;
		}

	}

;

basic_block:
	BBNUM ':' executable_statement_list
	{
		if (NOT_ONLY_PARSE)
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
		if (NOT_ONLY_PARSE)
		{
			$$ = $1;
		}
	}
|
	assignment_statement_list RETURN ';'
	{

		if (NOT_ONLY_PARSE)
		{
			list<Ast *> * assign_list = $1;
			Ast * ret = new Return_Ast(get_line_number());
			list<Ast *> * exe_list = NULL;

			if (assign_list)
				exe_list = assign_list;

			else
				exe_list = new list<Ast *>;

			exe_list->push_back(ret);

			$$ = exe_list;
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
		if (NOT_ONLY_PARSE)
		{
			$$ = NULL;
		}
	}
|
	assignment_statement_list assignment_statement
	{
		if(NOT_ONLY_PARSE)
		{
			Ast * assign_stmt = $2;
			CHECK_INVARIANT((assign_stmt != NULL), "Assignment statement cannot be null");

			if ($1 == NULL)
			$$ = new list<Ast *>;

			else
				$$ = $1;

			$$->push_back($2);
		}
	}
;

assignment_statement:
	variable ASSIGN_OP expression ';'
	{
		if (NOT_ONLY_PARSE)
		{
			CHECK_INVARIANT((($1 != NULL) && ($3 != NULL)), "lhs/rhs cannot be null");

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
			$$->check_ast();
		}
	}
;

goto_statement:
	GOTO BBNUM ';'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = new Goto_Ast($2, get_line_number());
			$$->check_ast();
			current_procedure->basic_block_gotos.push_back($2);
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

expression:
	arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			$$ =  $1;
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
|
	'(' relational_expression ')'
	{
		if(NOT_ONLY_PARSE)
		{
			$$ = $2;
		}
	}

;

arithmetic_expression:
	SUB_OP variable_or_constant_typecast
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($2, UMINUS, NULL, line);
			$$->check_ast();
		}

	}
|
	arithmetic_expression ADD_OP arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($1, PLUS, $3, line);
			$$->check_ast();
		}

	}
|
	arithmetic_expression SUB_OP arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($1, MINUS, $3, line);
			$$->check_ast();
		}

	}
|
	arithmetic_expression MULT_OP arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($1, MULT, $3, line);
			$$->check_ast();
		}
	}
|
	arithmetic_expression DIV_OP arithmetic_expression
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($1, DIV, $3, line);
			$$->check_ast();
		}
	}
|
	variable_or_constant_typecast
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($1, VAR, NULL, line);
			$$->check_ast();
		}

	}
;

variable_or_constant_typecast:
	'(' FLOAT ')' variable_or_constant
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($4, F_NUM, NULL, line);
			$$->check_ast();
		}
	}

|
	'(' DOUBLE ')' variable_or_constant
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($4, F_NUM, NULL, line);
			$$->check_ast();
		}
	}
|
	'(' INTEGER ')' variable_or_constant
	{
		if(NOT_ONLY_PARSE)
		{
			int line = get_line_number();
			$$ = new Arithmetic_Expr_Ast($4, I_NUM, NULL, line);
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
;


variable:
	NAME
	{
		if(NOT_ONLY_PARSE)
		{
			Symbol_Table_Entry * var_table_entry;

			CHECK_INVARIANT(($1 != NULL), "Variable name cannot be null");

			string var_name = *$1;

			if (current_procedure->variable_in_symbol_list_check(var_name) == true)
				 var_table_entry = &(current_procedure->get_symbol_table_entry(var_name));

			else if (program_object.variable_in_symbol_list_check(var_name) == true)
				var_table_entry = &(program_object.get_symbol_table_entry(var_name));

			else
				CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable has not been declared");

			Ast * name_ast = new Name_Ast(var_name, *var_table_entry, get_line_number());

			$$ = name_ast;
		}
	}
;

constant:
	INTEGER_NUMBER
	{
		if (NOT_ONLY_PARSE)
		{
			int num = $1;

			Ast * num_ast = new Number_Ast<int>(num, int_data_type, get_line_number());

			$$ = num_ast;
		}
	}

|	FLOAT_NUMBER
	{
		if (NOT_ONLY_PARSE)
		{
			$$ = new Number_Ast<float>($1, float_data_type, get_line_number());
		}
	}
;
