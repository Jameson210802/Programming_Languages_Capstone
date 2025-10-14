// ============================================================================
//  parser.cpp — Recursive-descent parser 
// ----------------------------------------------------------------------------
// MSU CSE 4714/6714 Capstone Project (Fall 2025)
// Author: Derek Willis
// ============================================================================

#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <set>
#include "lexer.h"
#include "ast.h"
#include "debug.h"
using namespace std;

// -----------------------------------------------------------------------------
// One-token lookahead
// -----------------------------------------------------------------------------
bool   havePeek = false;
Token  peekTok  = 0;
string peekLex;

inline const char* tname(Token t) { return tokName(t); }

Token peek() 
{
  if (!havePeek) {
    peekTok = yylex();
    if (peekTok == 0) { peekTok = TOK_EOF; peekLex.clear(); }
    else              { peekLex = yytext ? string(yytext) : string(); }
    dbg::line(string("peek: ") + tname(peekTok) + (peekLex.empty() ? "" : " ["+peekLex+"]")
              + " @ line " + to_string(yylineno));
    havePeek = true;
  }
  return peekTok;
}
Token nextTok() 
{
  Token t = peek();
  dbg::line(string("consume: ") + tname(t));
  havePeek = false;
  return t;
}
Token expect(Token want, const char* msg) 
{
  Token got = nextTok();
  if (got != want) {
    dbg::line(string("expect FAIL: wanted ") + tname(want) + ", got " + tname(got));
    ostringstream oss;
    oss << "Parse error (line " << yylineno << "): expected "
        << tname(want) << " — " << msg << ", got " << tname(got)
        << " [" << (yytext ? yytext : "") << "]";
    throw runtime_error(oss.str());
  }
  return got;
}


// TODO: implement parsing functions for each grammar in your language


unique_ptr<Write> parseWrite()
{
  if(peek() != OPENPAREN)
  {
    throw runtime_error("Parse error: expected OPENPAREN after WRITE");
  }

  expect(OPENPAREN,"OPENPAREN");

  if(peek() != STRINGLIT)
  {
    throw runtime_error("Parse error: expected STRINGLIT inside WRITE(...)");
  }

  auto s = make_unique<Write>();
  s->stringlit = peekLex;


  
  expect(STRINGLIT,"STRINGLIT");

  
  if(peek() != CLOSEPAREN)
  {
    throw runtime_error("Parse error: expected CLOSEPAREN after STRINGLIT");
  }
  expect(CLOSEPAREN,"CLOSEPAREN");
  

 

  return s;

}



unique_ptr<Block> parseBlock()
{

  if(peek() != TOK_BEGIN)
  {
    throw runtime_error("Parse error: expected BEGIN");
  }
  expect(TOK_BEGIN,"Beginning of Block");

  if(peek()!= WRITE)
  {
    throw runtime_error("Parse error: expected WRITE after BEGIN");
  }

  expect(WRITE,"string to print");

 auto b =  make_unique<Block>();

 b->write = parseWrite();

 
 if(peek() != END)
 {
    throw runtime_error("Parse error: expected END to close block");
 }

 expect(END,"End of file");

 return b;

}


// -----------------------------------------------------------------------------
// Program → PROGRAM IDENT ';' Block EOF
// -----------------------------------------------------------------------------
unique_ptr<Program> parseProgram() {
  expect(PROGRAM, "start of program");
  if (peek() != IDENT)
    throw runtime_error("Parse error: expected IDENT after PROGRAM");
  string nameLex = peekLex;  
  expect(IDENT, "program name");
  expect(SEMICOLON, "after program name");
  
  auto p = make_unique<Program>();
  p->name  = nameLex;
  p->block = parseBlock();

  expect(TOK_EOF, "at end of file (no trailing tokens after program)");
  return p;
}

