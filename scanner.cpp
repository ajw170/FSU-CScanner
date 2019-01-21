/*
 *  Name: Andrew J Wood
 *  Class: COP4020
 *  Assignment: Proj 1 (Implementing a C Scanner)
 *  Complie: "gcc -g -o cscan.exe cscan.c"
 */

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <map>
#include <string>
#define MAXTOK 256 /* maximum token size */

typedef std::pair<const std::string, int> tokenPair;

int cur;    /* current character being processed */
int peek;   /* next charcter to be processed */

int skip() {
  /* Potentially needs more updating */
  while (isspace(cur)) {
    cur = peek;
    peek = std::fgetc(stdin);
  }
  return cur;
}

/* Find a token and store the chars into lexeme buffer */
int scan(char *lexeme) {
  int i = 0;
  /* skip over whitespaces and check for EOF */
  if (skip() == EOF)
    return EOF;
  else if (isalpha(cur) || cur == '_') {
    // ID TOKEN
    while (isalpha(cur) || isdigit(cur) || cur == '_') {
      lexeme[i++] = cur;
      cur = peek;
      if (peek != EOF)
        peek = std::fgetc(stdin);

    }
    lexeme[i] = '\0';
    return i; // return any value other than EOF
  }
  else
  {


    /*** yada yada ***/



  }
  return 0;
}

void printSummary (std::map<const std::string,int> map)
{
  map.begin();
}

int main()
{
  char lexeme[MAXTOK];
  int  result;

  //std::string testString = "<<=";
  //std::string testToken = "string";

  //std::cout << testString.compare(testToken);


  //create map to hold counts
  std::map<const std::string,int> tokenMap;

  //initializes the token map with 0 values
  tokenMap.insert(tokenPair("(",0)); tokenMap.insert(tokenPair(")",0)); tokenMap.insert(tokenPair(",",0));
  tokenMap.insert(tokenPair(".",0)); tokenMap.insert(tokenPair(":",0)); tokenMap.insert(tokenPair(";",0));
  tokenMap.insert(tokenPair("?",0)); tokenMap.insert(tokenPair("[",0)); tokenMap.insert(tokenPair("]",0));
  tokenMap.insert(tokenPair("{",0)); tokenMap.insert(tokenPair("}",0)); tokenMap.insert(tokenPair("",0)); //possible removal, is a string
  tokenMap.insert(tokenPair("&&",0)); tokenMap.insert(tokenPair("||",0)); tokenMap.insert(tokenPair("++",0));
  tokenMap.insert(tokenPair("--",0)); tokenMap.insert(tokenPair("->",0));

  tokenMap.insert(tokenPair("|",0)); tokenMap.insert(tokenPair("^",0)); tokenMap.insert(tokenPair("&",0));
  tokenMap.insert(tokenPair("+",0)); tokenMap.insert(tokenPair("-",0)); tokenMap.insert(tokenPair("%",0));
  tokenMap.insert(tokenPair("*",0));








  /* setup for scanning */
  cur = peek = std::fgetc(stdin);
  if (cur != EOF)
    peek = std::fgetc(stdin);

  while ((result = scan(lexeme)) != EOF) {
    std::cout << lexeme << std::endl;
  }

  //prints the summary table
  printSummary(tokenMap);

  return 0;
}
