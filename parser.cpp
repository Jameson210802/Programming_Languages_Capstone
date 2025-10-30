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

  Token type = peek();
  if(type != STRINGLIT || type != IDENT)
  {
    throw runtime_error("Parse error: expected STRINGLIT or IDENT inside WRITE(...)");
  }

  auto s = make_unique<Write>();
  s->content = peekLex;




  
  expect(type,"STRINGLIT or IDENT");

  
  if(peek() != CLOSEPAREN)
  {
    throw runtime_error("Parse error: expected CLOSEPAREN after STRINGLIT");
  }
  expect(CLOSEPAREN,"CLOSEPAREN");
  

 

  return s;

}
unique_ptr<Read> parseRead()
{

  auto r = make_unique<Read>();

  

  // if(peek() != READ)
  // {
  //   throw runtime_error("expected read");

  // }

  //expect(READ,"Reading user value");

  if(peek()!=OPENPAREN)
  {
    throw runtime_error("expected OPENPAREN after Read");
  }

  expect(OPENPAREN,"OPENPAREN");
  if(peek() != IDENT)
  {
    throw runtime_error("expected IDENT after OPENPAREN");

  }

  r->target = peekLex;

  expect(IDENT,"IDENT");

  if(peek()!=CLOSEPAREN)
  {
    throw runtime_error("expected CLOSEPAREN after IDENT");
  }

  expect(CLOSEPAREN,"OPENPAREN");

  return r;
  
}
unique_ptr<Assign> parseAssign()
{
 // string var_name; 
  //Token value_type;
  //int int_val;
  //double double_val;

  auto a = make_unique<Assign>();
  if(peek() != IDENT)
  {
    throw runtime_error("expected an IDENT for Assign");
  }
  a->id = peekLex;
  expect(IDENT,"var name for assign");

  if(peek() != ASSIGN)
  {
    throw runtime_error("expected an ASSIGN (:=) after IDENT");
  }
  expect(ASSIGN,"Assign(:=) operator");

  a->type = peek();
  a->value = peekLex;
  if(a->type != INTLIT || a->type != FLOATLIT || a->type != IDENT)
  {
    throw runtime_error("Expected Either INTLIT or FLOATLIT or IDENT after assign");
  }

  expect(a->type,"Value to be assigned to variable");

  //TODO FIGURE OUT HOW TO DO IDENT with assign
  // auto it = symbolTable.find(a->id);
  // if(a->type == INTLIT)
  // {
  //   int_val = stoi(a->value);
  //   it->second = int_val;
  // }
  // else if (a->type == INTLIT)
  // {
  //   double_val = stod(a->value);
  //   it->second = double_val;
  // }
  // else
  // {
  //   auto ident_val = symbolTable.find(a->value); // gets the value of the identifier

  //   it->second = ident_val->second; // Sets the variable equal to Identifer VAL := IDENT
  // }

  


  return a;



}




unique_ptr<Statement> parseStatement()
{

  //auto c = make_unique<CompoundStatement>;


  switch (peek())
  {
    case READ: // if token is goes to parseRead
      expect(READ,"Read token");
      return parseRead();
      break;

    case WRITE: //if token is write goest to parseRead
      expect(WRITE,"Write token");
      return parseWrite();
      break;
    
    case IDENT: // if token is an Identifier goes to assign
      return parseAssign();
      break;

    case TOK_BEGIN:
      return parseCompound();  
      break;

    default:
      runtime_error("Expected either READ,WRITE,IDENT, or BEGIN in statement");

      break;
    }


  
  
  
}

unique_ptr<CompoundStatement> parseCompound()
{

 auto c = make_unique<CompoundStatement>();
 Token type; 

  if(peek() != TOK_BEGIN)
  {
    throw runtime_error("exepected BEGIN to start compound statement");
  }
  expect(TOK_BEGIN,"BEGIN for Compound");

  //c->stmts.push_back()
  do
  {



    c->stmts.push_back(parseStatement());
    
    // if(peek() == READ) // if token is goes to parseRead
    // {
    //   expect(READ,"Read token");
    //   c->stmts.push_back(parseRead());
    // }
    // else if (peek() == WRITE) // if token is 
    // {
    //   expect(WRITE,"Write token");
    //   c->stmts.push_back(parseWrite());
    // }
    // else if (peek() == IDENT)
    // {
    //   c->stmts.push_back(parseAssign());
    // }
    
    



    
  }while (peek() == SEMICOLON && nextTok());

}



// unique_ptr<CompoundStatement> parseDeclaration()
// {

//   string previous_name = " ";
//   if(peek() != IDENT)
//   {
//     throw runtime_error("Parser Error: expected IDENT after VAR");
//   }

//   string current_name = peekLex;

//   if()
//   expect(IDENT,"Var Identifier");

  




// }





unique_ptr<Block> parseBlock()
{
  auto b =  make_unique<Block>();

  string var_name; 
  Token var_type; 
  bool var_list = true;

  if(peek() == TOK_BEGIN){var_list = false;}
  

  while(var_list)
  {
    if(peek() != VAR) //checks to see if the variable decleartions exists
    {
      throw runtime_error("EXPECTED VAR since there was not a begin token");
    }
    expect(VAR,"Var Declarations");
    if(peek() != IDENT){throw runtime_error("Expected IDENT after VAR");}

    var_name = peekLex; // stoes the name of the variable. 

    expect(IDENT,"variable name");
    
    if(peek() != COLON){throw runtime_error("Expected colon (:) after IDENT");}

    expect(COLON,"Colon");

    var_type = peek(); // stores the token type. 

    if(var_type != INTEGER || var_type != REAL){throw runtime_error("Expected INTEGER or REAL after COLON (:)");} // if the variable is neither a INTERGER or a REAL number it errors out. 

    expect(var_type,"Value of intialized variable");

    if(peek() != SEMICOLON){throw runtime_error("Expected a SEMICOLON(;) after variable type");} //errors out if no SEMICOLON


    auto it = symbolTable.find(var_name);

    if(it!= symbolTable.end())
    {
      throw runtime_error("duplicate");
    }

    if(var_type == INTEGER)
    {
      symbolTable[var_name] = 0;
    }
    else
    {
      symbolTable[var_name] = 0.0;
    }

    if(peek()!= IDENT)
    {
      var_list = false;
    }

  }


  if(peek() != TOK_BEGIN)
  {
    throw runtime_error("Parse error: expected BEGIN");
  }
  expect(TOK_BEGIN,"Beginning of Block");

  
  b->comp = parseCompound();








 // unique_ptr<CompoundStatement> c = 






  // if(peek()!= WRITE)
  // {
  //   throw runtime_error("Parse error: expected WRITE after BEGIN");
  // }

 // expect(WRITE,"string to print");



 //b->write = parseWrite();

 
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

