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

int cur = 0;    /* current character being processed */
int peek = 0;   /* next charcter to be processed */
int line = 0;   /* keeps track of the line number */

int skip(FILE  * inputStream) {
  /* Potentially needs more updating? */
  //TODO - check if this needs more updating
  while (isspace(cur)) {
    if (cur == '\n' || cur == '\r')
      ++line;
    cur = peek;
    peek = std::fgetc(inputStream);
  }
  return cur;
}

/* Find a token and store the chars into lexeme buffer */
int scan(char *lexeme, FILE * inputStream, std::map<const std::string,int, cmpByLengthThenByLexOrder> & tokenMap)
{
  int i = 0;

  /* skip over whitespaces and check for EOF */
  if (skip(inputStream) == EOF)
    return EOF;

  //if a ditto character is encountered
  else if (cur == '"')
  {
    lexeme[i++] = cur;
    cur = peek;
    if (peek != EOF)
      peek = std::fgetc(inputStream);
    while (isprint(cur) || cur == '\n') //if current character is printable, or if the character is newline
    {
      if (cur == '\\')
      {
        lexeme[i++] = cur;
        cur = peek;
        if (peek != EOF)
          peek = std::fgetc(inputStream);
        if (cur == '\"')
        {
          lexeme[i++] = cur;
          cur = peek;
          if (peek != EOF)
            peek = std::fgetc(inputStream);
        }
      }
      else if (cur == '\"') //end of string token found
      {
        lexeme[i++] = cur;
        lexeme[i++] = '\0';
        cur = peek;
        if (peek != EOF)
          peek = std::fgetc(inputStream);
        ++tokenMap["string"];
        break; //break out of loop and continue
      }
      else if (cur == '\n') //in this case, no end quote was found.  print error and increment line number
      {
        lexeme[i++] = '\0';
        std::cout << "missing \" for " << lexeme << " on line " << line++ << std::endl;
        return 1; //return error code 1
      }
      else
      {
        lexeme[i++] = cur;
        cur = peek;
        if (peek != EOF)
          peek = std::fgetc(inputStream);
      }
    }
  } //quote case


  //Dr. Uh's code begins here
  else if (isalpha(cur) || cur == '_')
  {
    // ident token, keep processing until white space is reached nor non-compliant character is reached
    while (isalpha(cur) || isdigit(cur) || cur == '_')
    {
      lexeme[i++] = cur;
      cur = peek;
      if (peek != EOF)
        peek = std::fgetc(inputStream);
    }
    lexeme[i] = '\0';
    return 0; // successful return
  }
  else
  {
    /*** yada yada ***/
  }
  return 0;
}

void printSummary (std::map<const std::string,int, cmpByLengthThenByLexOrder> map)
{
  printf("        token         count\n");
  printf("--------------------- -----\n");
  printf("%21s %5d\n","lexeme",5);
  map.begin();
}

int main()
{
  char lexeme[MAXTOK];
  line = 1; //
  int  result;

  //used for debugging purposes with CLion

  //TODO - Change this back to stdin for linprog use
  //FILE inputStream = stdin;
  FILE * inputStream = fopen("input.in","r");

  //std::string testString = "number";
  //std::string testToken = "string";

  //std::cout << testString.compare(testToken);


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


  while ((result = scan(lexeme, inputStream, tokenMap)) != EOF) //1 indicates error code
  {
    if (result != 1)
      std::cout << lexeme << std::endl;
  }

  //prints the summary table
  printSummary(tokenMap);

  return 0;
}
