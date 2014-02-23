
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented by Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors (theory and lab)  at IIT
           Bombay.

           Release  date Jan  15, 2013.  Copyrights reserved  by Uday
           Khedker. This implemenation has been made available purely
           for academic purposes without any warranty of any kind.

           A  doxygen   generated  documentation  can  be   found  at
           http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>

using namespace std;

#include "user-options.hh"
#include "error-display.hh"

void report_error(string error_message, int line)
{
	string file_name = command_options.get_file_name();

	stringstream message;
	if (line > NOLINE)
		message << file_name << " : line " << line << " :: error : " << error_message;
	else
		message << file_name << " :: cfglp error : " << error_message;

	print_error(message.str(), NOTEXIT);
}

void print_error(string error_message, int exit_flag)
{
	cerr << error_message << "\n";

	command_options.remove_files();
	exit(0);
}
