This calculator project is based on the exercises and specification from the book:
Programming: Principles and Practice Using C++, 2nd ed.

The project is meant as an exercise to learn:
1) Explore lexical analysis and control flow.
2) Prototyping and expanding existing code.

The calculator is currently using the below grammar:

    Statement:
        Second Order'Print'
        Second Order
        Declaration
        Reassignment
        Power
        Square Root
        Print
        Quit
        Help

    Declaration:
        'let' name '=' Second Order'Print'
        'const' name '=' Second Order'Print'

    Reassignment:
        name '=' Second Order'Print'

    Power:
        pow(Primary, Primary)
        pow(Primary, Primary)'Print'

    Square Root:
        sqrt(Primary)
        sqrt(Primary)'Print'

    Print:
        ;
    
    Quit:
        q

    Help:
        h or H

    Second Order:
        First Order
        Second Order '+' First Order
        Second Order '-' First Order
    
    First Order:
        Factorial
        First Order '*' Factorial
        First Order '/' Factorial
        First Order '%' Factorial
    
    Factorial:
        Primary
        Primary'!'
    
    Primary:
        Variable
        Number
        ( Second Order )
        { Second Order }
        '-' Primary
        '+' Primary
    
    Variable:
        Name
        
    Number:
        Floating point numerals