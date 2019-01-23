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
#include <iomanip>
#include <vector>

#define MAXTOK 256 /* maximum token size */

typedef std::pair<const std::string, int> tokenPair;

int analyzeChar(std::vector<char> &); //function prototype

//custom comparator object to use with standard map; will make life easier when outputting table
//ensures table is already sorted by longest string first, then by dictionary order
//it is up to progammer to scan table to apply number level filter
struct cmpByLengthThenByLexOrder {
    bool operator() (const std::string& a, const::std::string& b) const
    {
      if (a.length() == b.length())
      {
        return a > b;
      }
      else
      {
        return a.length() > b.length();
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
    if (cur == '\n')
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

  //quote case, if a ditto character is encountered
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
        lexeme[i] = '\0';
        cur = peek;
        if (peek != EOF)
          peek = std::fgetc(inputStream);
        ++tokenMap["string"];
        break; //break out of loop and continue
      }
      else if (cur == '\n') //in this case, no end quote was found.  print error and increment line number
      {
        lexeme[i] = '\0';
        std::cout << "missing \" for " << lexeme << " on line " << line << std::endl;
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

  //digit case
  else if (isdigit(cur))
  {
    while (isdigit(cur))
    {
      lexeme[i++] = cur;
      cur = peek;
      if (peek != EOF)
        peek = std::fgetc(inputStream);
    }
    ++tokenMap["number"];
    lexeme[i] = '\0';
    return 0; //success
  }


  //identifier case
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
    ++tokenMap["ident"];
    lexeme[i] = '\0';
    return 0; // successful return
  }



  //char case
  else if (cur == '\'')
  {
    std::vector<char> charVector;
    charVector.push_back(cur);

    lexeme[i++] = cur;
    cur = peek;
    if (peek != EOF)
      peek = std::fgetc(inputStream);

    while (true)
    {
      if (cur == '\\' && charVector.size() < 2) //if an escape character is encountered followed by a '
      {
        lexeme[i++] = cur;
        charVector.push_back(cur);
        cur = peek; //advance
        if (peek != EOF)
          peek = std::fgetc(inputStream);
        if (cur == '\'')
        {
          // a \' character has been identified, so continue with the loop with no effect
          lexeme[i++] = cur;
          charVector.push_back(cur);
          cur = peek; //advance
          if (peek != EOF)
            peek = std::fgetc(inputStream);
        }
      } //end dealing with \' case

      else if (cur == '\n' || cur == EOF) //in this case, no end quote was found.  print error and increment line number
      {
        lexeme[i] = '\0';
        std::cout << "missing \' for " << lexeme << " on line " << line << std::endl;
        return 1; //return error code 1
      }
      else if (cur == '\'') //ending quote found.  put character on vector and send for further analysis
      {
        lexeme[i++] = cur;
        charVector.push_back(cur);
        lexeme[i] = '\0'; //terminate lexeme

        cur = peek; //advance
        if (peek != EOF)
          peek = std::fgetc(inputStream);

        int result = analyzeChar(charVector);

        /*process here depending on result */
        if (result == 0)
        {
          ++tokenMap["char"];
          return 0;
        }
        else if (result == -1)
        {
          std::cout << "character has 0 length on line " << line << std::endl;
          return 1;
        }
        else if (result == -2)
        {
          std::cout << "missing \' for " << lexeme << " on line " << line << std::endl;
          return 1;
        }
        else if (result == -3)
        {
          std::cout << "character constant " << lexeme << " is too long on line " << line << std::endl;
          return 1;
        }
        else if (result == -4)
        {
          std::cout << "illegal octal constant " << lexeme << " on line " << line << std::endl;
          return 1;
        }
        else
        {
          std::cout << "a serious error which should not have occurred has occurred." << std::endl;
          return 1; //if we get here then something went wrong
        }

      }
      else //if any other printable character other than \n was found
      {
        lexeme[i++] = cur;
        charVector.push_back(cur);
        cur = peek;
        if (peek != EOF)
          peek = std::fgetc(inputStream);
      }
    }
  } //end char casr

  else //somethine else was found...to be continued
  {
    /*** yada yada ***/
  }
  return 1;
}

int analyzeChar(std::vector<char> & charVector)
{
  //NOTE:  all charVectors that arrive to this function will have the form 'xxxxxx' where xxxxxx is 1 or more characters
  //Therefore, the minimum length will always be 2.

  //codes:  0 => success, the char is valid
  //codes: -1 => character has zero length on line x
  //codes: -2 => missing ' for '\' on line x
  //codes: -3 => character constant 'xxxxxx' is too long on line x
  //codes: -4 => illegal octal constant '\xxx' on line x

  //see if the char is valid.  if it is, print it and
  int length = charVector.size(); //if it is less than 2, there is a major problem

  if (length == 2)
    return -1;
  else if (length == 3) //correct length, just ensure it isn't '\'
  {
    if (charVector[1] == '\\')
      return -2;
    else //correct char character
      return 0;
  }
  else if (length == 4) //could be correct assuming first character is '\'
  {
    if (charVector[1] == '\\')
      return 0;
    else
      return -3;
  }
  else if (length == 5) //would only be correct if it is in the form of '\yy' where y is an octal
  {
    if ((charVector[1] == '\\') && (isdigit(charVector[2])) && (((int)(charVector[2] - '0') <= 7))
          && (isdigit(charVector[3])) && (((int)(charVector[3] - '0') <= 7)))
    {
      return 0; //valid octal
    }
    else if (charVector[1] != '\\')
    {
      return -3; //was too long but not an octal
    }
    else
    {
      return -4; //invalid octal constnat
    }
  }
  else if (length == 6)
  {
    if ((charVector[1] == '\\') && (isdigit(charVector[2])) && (((int)(charVector[2] - '0') <= 7))
        && (isdigit(charVector[3])) && (((int)(charVector[3] - '0') <= 7))
        && (isdigit(charVector[4])) && (((int)(charVector[4] - '0') <= 7)))
    {
      return 0; //valid octal
    }
    else if (charVector[1] != '\\')
    {
      return -3; //was too long but not an octal
    }
    else
    {
      return -4; //invalid octal constnat
    }
  }
  else //all other cases, too long
  {
    return -3;
  }
}





void printSummary (std::map<const std::string,int, cmpByLengthThenByLexOrder> map)
{
  printf("        token         count\n");
  printf("--------------------- -----\n");

  //iterate through map and print the token counts

  //TODO - ensure order is correct
  for (auto it = map.cbegin(); it != map.end(); ++it)
  {
    if (it->second != 0) //if there is something to print
    {
      std::cout << std::right << std::setw(21) << it->first << std::setw(5) << it->second << std::endl;
    }
  }

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
