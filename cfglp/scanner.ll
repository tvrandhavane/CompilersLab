
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

%filenames="scanner"
%lex-source="scanner.cc"

%%

int		{
			store_token_name("INTEGER");
			return Parser::INTEGER;
		}
float	{
			store_token_name("FLOAT");
			return Parser::FLOAT;
		}

return	{
			store_token_name("RETURN");
			return Parser::RETURN;
		}

if		{
			store_token_name("IF");
			return Parser::IF;
		}

else		{
			store_token_name("ELSE");
			return Parser::ELSE;
		}

goto	{
			store_token_name("GOTO");
			return Parser::GOTO;
		}

">="	{
			store_token_name("GE");
			return Parser::ge;
		}

"<="	{
			store_token_name("LE");
			return Parser::le;
		}
[>] 	{
			store_token_name("GT");
			return Parser::gt;
		}
[<]		{
			store_token_name("LT");
			return Parser::lt;
		}
"=="	{
			store_token_name("EQ");
			return Parser::eq;
		}
"&&"	{
			store_token_name("AND");
			return Parser::andTok;
		}
"||"	{
			store_token_name("OR");
			return Parser::orTok;
		}
"!="	{
			store_token_name("NE");
			return Parser::ne;
		}
[!]	{
			store_token_name("NT");
			return Parser::nt;
		}

[=] 	{
			store_token_name("ASSIGN_OP");
			return Parser::ASSIGN_OP;
		}

[:{}();]	{
			store_token_name("META CHAR");
			return matched()[0];
		}
[+]	{
		store_token_name("ARITHOP");
		return Parser::ADD_OP;
	}
[-]	{
		store_token_name("ARITHOP");
		return Parser::SUB_OP;
	}
[*]	{
		store_token_name("ARITHOP");
		return Parser::MULT_OP;
	}
[/]	{
		store_token_name("ARITHOP");
		return Parser::DIV_OP;
	}

[<][b][b][ ][[:digit:]_]+[>] {
			store_token_name("BASIC BLOCK");
			ParserBase::STYPE__ * val = getSval();
			char *c = (char *)malloc(25);
			strcpy(c, matched().c_str());
			char *token = (char *)malloc(25);
			int k;
			for(k = 4; c[k] != '>'; k++){
				token[k - 4] = c[k];
			}
			val->integer_value = atoi(token);
			return Parser::BASIC_BLOCK;
		}

[-]?[[:digit:]_]+ 	{
				store_token_name("NUM");

				ParserBase::STYPE__ * val = getSval();
				val->integer_value = atoi(matched().c_str());

				return Parser::INTEGER_NUMBER;
			}

[-]?[[:digit:]_]+[.][[:digit:]_]+ {
				store_token_name("FNUM");

				ParserBase::STYPE__ * val = getSval();
				val->float_value = atof(matched().c_str());
				printf("%f\n",val->float_value);
				return Parser::FLOAT_NUMBER;

			}
[[:alpha:]_][[:alpha:][:digit:]_]* {
					store_token_name("NAME");

					ParserBase::STYPE__ * val = getSval();
					val->string_value = new std::string(matched());

					return Parser::NAME;
				}

\n		{
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}

";;".*  	|
[ \t]		{
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}

.		{
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();
			int line_number = lineNr();
			report_error(error_message, line_number);
		}
