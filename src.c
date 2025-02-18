// Cristopher Ahuatl
// CS435-01 SPR 2025 
// Project 01: Simple Scanner for PL1
//      Use command-line parameters to access the source file. Read it and output all tokens on seperate lines. 
//      FOR LEXEMES AND IDENTIFIERS: Also output the lexeme
//      **Emphasis on test cases

#include <stdio.h>  //for c I/O
#include <stdlib.h> //for exit()
#include <ctype.h>  //for isalpha(), isalnum(), ...

enum tokenType{
    READ, WRITE, ID, NUMBER, LPAREN, RPAREN, SEMICOLON, COMMA, ASSIGN, PLUS, MINUS, TIMES, DIV, SCAN_EOF
};

char *mnemonic[] = {"READ", "WRITE", "ID", "NUMBER", "LPAREN", "RPAREN", "SEMICOLON", "COMMA", "ASSIGN", "PLUS", "MINUS", "TIMES", "DIV", "SCAN_EOF"};

void lexical_error(char ch){
    fprintf(stderr, "Lexical error. Unexpected character: %c. \n", ch);
}

char lexeme[265] = {'\0'};
unsigned int lexLen = 0;
FILE *src;

enum tokenType scan() {
    static int currentCh = ' ';
    static int tempCh = ' ';
    char* reserved[2] = { "read", "write" };
    lexLen = 0; //clear lexeme buffer for each scan
    lexeme[0] = '\0';
    extern FILE *src;   //pointer to FILE handle that binds to source file

    if (feof(src)) {    //EOF indicator is set when a read operation attempts to read past the end of a file
        return SCAN_EOF; // if end of file, return the eof mnemonic token
    }
    while ((currentCh = fgetc(src))!= EOF){ // while current scan doesn't hit end of file
        if (isspace(currentCh)){    //skips whitespace characters when scanning and goes to next iteration
            continue;
        }

        /*===== IDENTIFIER LEXEME CASE =====*/
    
        else if (isalpha(currentCh) || currentCh == '_') {  //NEEDS TO BE MODIFIED ***
            lexeme[0] = currentCh;
            lexLen = 1;
            //temp = next char scanned. Do while scanned char is a char, num, or underscore, if all conditions met, temp is valid and can be evaluated.
            for (tempCh = fgetc(src); isalnum(tempCh) || tempCh == '_'; tempCh = fgetc(src)) {
                //BUILD IDENTIFIER LEXEME
                //add characters into the lexeme container
                if (lexLen != 265){     //stopping case if lexeme is too large
                    lexeme[lexLen] = tempCh;    //append next char to lexeme string
                    lexLen++;                   //adjust length after appending
                }
                else {
                    fprintf(stderr, "Lexical error. Max lexeme length 256 reached.\n");
                    return 1; //non-zero returned to indicate error
                }
            }

            lexeme[lexLen] = '\0'; //add sentinel value to current lexeme string

            ungetc(tempCh, src);    //put back character that is not an alpha/digit or '_'
            //see if lememe is a reserved word. If not, return ID.
            return ID;
        }

        /*===== NUMBER LEXEME CASE =====*/
        else if (isdigit(currentCh)){
            //BUILD LEXEME FOR THE NUMBER
            lexeme[0] = currentCh;
            lexLen = 1;
            for (tempCh = fgetc(src); isdigit(tempCh); tempCh = fgetc(src)) {
                lexeme[lexLen] = tempCh;
                lexLen++;
            }

            lexeme[lexLen] = "\0";
            ungetc(tempCh, src);
            
            //finish fixing lexeme string, ungetc the last character read that is not a digit and then return a NUMBER
            return NUMBER; 
        }
        /*===== OPERATOR TOKEN LEXEME CASE =====*/
        else if (currentCh == ":") {
            tempCh = fgetc(src);
            if (tempCh == '='){
                return ASSIGN;
            }
            lexical_error(currentCh);
        }
        else if (currentCh == '+'){
            return PLUS;
        }
        else if (currentCh == "-") {
            return MINUS;
        }
        else if (currentCh == "*") {
            return TIMES;
        }   
        else if (currentCh == "/") {
            return DIV;
        }
        else if (currentCh == ";") {
            return SEMICOLON ;
        }
        else if (currentCh == ",") {
            return COMMA;
        }
        else if (currentCh == "(") {
            return LPAREN;
        }
        else if (currentCh == ")") {
            return RPAREN;
        }
        /*===== INVALID INPUT CHARACTER CASE =====*/
        //use selection statements to look for tokens for operators and delimiters and the assignment (:=)
        else {
            lexical_error(currentCh);
        }
    }
    return SCAN_EOF;
}

int main (int argc[], char* argv[]){
    extern FILE *src;
    enum tokenType currentToken;
    if(argc > 1){
        if (fopen_s(&src, argv[1], "r")){
            fprintf(stderr, "Error opening source file %s", argv[1]);
            exit(1);
        }
    }
    while((currentToken = scan()) != SCAN_EOF){
        //finish body for displaying the string version of the internal token name and
        //also print the lexeme if the token is an ID or a number. Do not print the lexeme for other tokens
        printf("%s", mnemonic[currentToken]); 

        if (currentToken == ID || currentToken == NUMBER){
            printf(" %s", lexeme); //prints actual lexeme read for the cooresponding token. Ex: (ID, x) or (NUMBER, 23)
            printf("\n");
        }
    }
    fclose(src);
    return 0;
} //end driver 
