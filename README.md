########################################################
#						       						   #
#						       						   #
#   This is going to be a calculator project using     #
#   basic lexical analysis			       			   #
#						       						   #
#						       						   #
########################################################

The grammar:

	1. Primaries:
		numbers, ()
	2. Terms:
		primary
		term '*' primary
		term '/' primary
	3. Expressions:
		term
		expression '+' term
		expression '-' term