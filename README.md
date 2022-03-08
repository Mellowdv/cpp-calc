########################################################
#						       						   #
#						       						   #
#   This is going to be a calculator project using     #
#   basic lexical analysis			       			   #
#						       						   #
#						       						   #
########################################################

The basic grammar:
	1. Evaluate ()
	2. Evaluate * and /
	3. Evaluate + and -

# usually this hierarchy is written in reverse order, but I find this more intuitive at the moment
More advanced:
	1. Primaries:
		numbers, ()
	2. Terms:
		primary
		term '*' term
		term '/' term
	3. Expressions:
		term
		expression '+' expression
		expression '-' expression
		
		
