This simple calculator project is meant as an exercise to learn about lexical analysis.

The grammar being implemented:

	1. Expressions:
		term
		expression '+' term
		expression '-' term
	2. Terms:
		primary
		term '*' primary
		term '/' primary
	3. Primaries:
		numbers, ()
