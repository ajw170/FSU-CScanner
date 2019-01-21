/*
 *  Simple infix to postfix translator using grammar and translation scheme
 *  This particular example uses predictive parsing (top-down parsing) for 
 *  syntax analysis and use translation scheme to emit postfix expression 
 *  as a target code generation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* lookahead symbol to guide the syntax analysis (LL1) */
int lookahead;
int yylex();

/* C function prototype */
void expr();
void expr_prime();
void term();
void match(char);
void error();

/* 
 * lexical analysis : scanner impelemntation 
 */
int yylex() 
{
   int cur = getchar();
   while (isspace(cur))
       cur = getchar(); // advance
   return cur;
}

void match(char t)
{
  if (lookahead == t) 
    lookahead = yylex();
  else 
    error("Illegal infix expression");
}

/*
 * Predictive parser (top-down parsing)
 * Note: EOF from stdin is ctrl-d on unix
 *                         ctrl-z on window
 */
int main()
{
  //lookahead = yylex();
  lookahead = yylex();  
  expr();
  putchar('\n'); fflush(stdout);
  return(EXIT_SUCCESS);
}

void expr() 
{
  term();
  expr_prime();
}

void expr_prime() 
{
  if (lookahead == '+') {
    match('+');
    term();
    putchar('+'); fflush(stdout);  // translation scheme
    expr_prime();
  }

  else if (lookahead == '-') {
    match('-');
    term();
    putchar('-'); fflush(stdout);
    expr_prime();
  }
  else if (isdigit(lookahead))
    error();
}

void term()
{
  if (isdigit(lookahead)) {
    putchar(lookahead); fflush(stdout); // translation scheme
    match(lookahead);
  }
  else 
      error();
}


void error(char *msg)
{
    fprintf(stderr,"%s\n\n", msg);
    exit(1);
}


   


