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

//custom comparator object to use with standard map; will make life easier when outputting table
struct cmpByLengthThenByLexOrder {
    bool operator() (const std::string& a, const::std::string& b) const
    {
      if (a.length() == b.length())
      {
        return a < b;
      }
      else
      {
        return a.length() < b.length();
      }
    }
};

int cur;    /* current character being processed */
int peek;   /* next charcter to be processed */

int skip(FILE  * inputStream) {
  /* Potentially needs more updating */
  while (isspace(cur)) {
    cur = peek;
    peek = std::fgetc(inputStream);
  }
  return cur;
}

/* Find a token and store the chars into lexeme buffer */
int scan(char *lexeme, FILE * inputStream)
{
  int i = 0;
  /* skip over whitespaces and check for EOF */
  if (skip(inputStream) == EOF)
    return EOF;
  else if (isalpha(cur) || cur == '_') {
    // ID TOKEN
    while (isalpha(cur) || isdigit(cur) || cur == '_') {
      lexeme[i++] = cur;
      cur = peek;
      if (peek != EOF)
        peek = std::fgetc(inputStream);

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

void printSummary (std::map<const std::string,int, cmpByLengthThenByLexOrder> map)
{
  printf(" token count\n");
  printf("--------------------- -----\n");
  printf("%21s %5d\n","lexeme",5);
  map.begin();
}

int main()
{
  char lexeme[MAXTOK];
  int  result;

  //used for debugging purposes with CLion

  //FILE inputStream = stdin;
  FILE * inputStream = fopen("input.in","r");

  std::string testString = "number";
  std::string testToken = "string";

  std::cout << testString.compare(testToken);


  //create map to hold counts, initialize all to 0
  std::map<const std::string,int, cmpByLengthThenByLexOrder> tokenMap;

  //non-token-specific elements
  tokenMap.insert(tokenPair("number",0));
  tokenMap.insert(tokenPair("ident",0));
  tokenMap.insert(tokenPair("char",0));
  tokenMap.insert(tokenPair("string",0));

  //tokens that would not be followed by additional characters (white space would normally be expected afterward)
  tokenMap.insert(tokenPair("(",0)); tokenMap.insert(tokenPair(")",0)); tokenMap.insert(tokenPair(",",0));
  tokenMap.insert(tokenPair(".",0)); tokenMap.insert(tokenPair(":",0)); tokenMap.insert(tokenPair(";",0));
  tokenMap.insert(tokenPair("?",0)); tokenMap.insert(tokenPair("[",0)); tokenMap.insert(tokenPair("]",0));
  tokenMap.insert(tokenPair("{",0)); tokenMap.insert(tokenPair("}",0)); tokenMap.insert(tokenPair("",0)); //possible removal, is a string
  tokenMap.insert(tokenPair("&&",0)); tokenMap.insert(tokenPair("||",0)); tokenMap.insert(tokenPair("++",0));
  tokenMap.insert(tokenPair("--",0)); tokenMap.insert(tokenPair("->",0));

  //characters that could be followed by another character
  tokenMap.insert(tokenPair("|",0)); tokenMap.insert(tokenPair("^",0)); tokenMap.insert(tokenPair("&",0));
  tokenMap.insert(tokenPair("+",0)); tokenMap.insert(tokenPair("-",0)); tokenMap.insert(tokenPair("%",0));
  tokenMap.insert(tokenPair("*",0)); tokenMap.insert(tokenPair("/",0)); tokenMap.insert(tokenPair("=",0));
  tokenMap.insert(tokenPair("!",0)); tokenMap.insert(tokenPair(">",0)); tokenMap.insert(tokenPair(">>",0));
  tokenMap.insert(tokenPair("<",0)); tokenMap.insert(tokenPair("<<",0));

  //the previous characters, with an = appended
  tokenMap.insert(tokenPair("|=",0)); tokenMap.insert(tokenPair("^=",0)); tokenMap.insert(tokenPair("&=",0));
  tokenMap.insert(tokenPair("+=",0)); tokenMap.insert(tokenPair("-=",0)); tokenMap.insert(tokenPair("%=",0));
  tokenMap.insert(tokenPair("*=",0)); tokenMap.insert(tokenPair("/=",0)); tokenMap.insert(tokenPair("==",0));
  tokenMap.insert(tokenPair("!=",0)); tokenMap.insert(tokenPair(">=",0)); tokenMap.insert(tokenPair(">>=",0));
  tokenMap.insert(tokenPair("<=",0)); tokenMap.insert(tokenPair("<<=",0));

  /* setup for scanning */
  cur = peek = std::fgetc(inputStream);
  if (cur != EOF)
    peek = std::fgetc(inputStream);

  while ((result = scan(lexeme, inputStream)) != EOF) {
    std::cout << lexeme << std::endl;
  }

  //prints the summary table
  printSummary(tokenMap);

  return 0;
}
