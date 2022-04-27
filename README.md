This calculator project is based on the exercises and specification from the book:
Programming: Principles and Practice Using C++, 2nd ed.

The project is meant as an exercise to learn:
1) Explore lexical analysis and control flow.
2) Prototyping and expanding existing code.

The calculator is currently using the below grammar:

	1.Second Order:
		Second Order
		Second Order '+' First Order
		Second Order '-' First Order
	2. First Order:
		Primary
		First Order '*' Primary
		First Order '/' Primary
		First Order '%' Primary
	3. Primaries:
		Numbers, ()