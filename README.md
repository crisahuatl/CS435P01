# CS435P01 
Scanner for Simple_PL1
Open a source file and scan it. Build lexemes by appending to a c-string, ignoring whitespaces.
The scanner should take the file and return a stream of the file translated into tokens in each line. 
For identifiers and numbers, output the token type and lexeme.
For special characters like operators, just return the token.

EXAMPLE:
  x := 3; 

RETURNED:
  ID, x
  ASSIGN
  NUMBER, 3
  SEMICOLON
