################################################################################################
#
#	            Abstract Syntax Trees for cs306 2013-14
#                   Implemented by 
#                         - Tanu Kanvar (tanu@cse.iitb.ac.in), and
#                         - Uday Khedker (uday@cse.iitb.ac.in) 
#
#
################################################################################################

AST_OBJ = ast.o main.o
HEADERS_CORE = ast.hh
OBJ = $(AST_OBJ)
HEADERS = $(HEADERS_CORE)
TGT = ast

CC = g++

CFLAGS = -g 

$(TGT): $(OBJ) 
	$(CC) $(OBJ) -o $(TGT)

%.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean :
	rm -rf *.o ast
