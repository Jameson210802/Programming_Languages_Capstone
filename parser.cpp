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
#include <typeinfo> // got from https://www.geeksforgeeks.org/cpp/type-inference-in-c-auto-and-decltype/
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

//extern int line_number;



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
unique_ptr<Statement> parseStatement();
unique_ptr<Write> parseWrite();
unique_ptr<Read> parseRead();
unique_ptr<Assign> parseAssign();
unique_ptr<CompoundStatement> parseCompound();
unique_ptr<Block> parseBlock();
unique_ptr<Program> parseProgram();
unique_ptr<valueNode> parseValue();
unique_ptr<valueNode> parseTerm();
unique_ptr<valueNode> parseFactor();

//unique_ptr<Declarations> parseDeclarations();


unique_ptr<Write> parseWrite()
{
  auto s = make_unique<Write>();
  if(peek() != OPENPAREN)
  {
    throw runtime_error("Parse error: expected OPENPAREN after WRITE");
  }

  expect(OPENPAREN,"OPENPAREN");

  s->type = peek();
  if(s->type != STRINGLIT && s->type != IDENT)
  {
    throw runtime_error("Parse error: expected STRINGLIT or IDENT inside WRITE(...)");
  }

  expect(s->type,"STRINGLIT or IDENT");
  //cout << "content for " << peekLex << endl;
  s->content = peekLex;

  if(s->type == IDENT)
  {
    auto confirm = symbolTable.find(s->content);

    if(confirm == symbolTable.end())
    {
    throw runtime_error("IDENT was not declared to WRITE to");
    }
  }
  

  


  


  
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

  

  expect(IDENT,"IDENT");
  r->target = peekLex;

  auto confirm = symbolTable.find(r->target);

  if(confirm == symbolTable.end())
  {
    throw runtime_error("IDENT was not declared to read");
  }


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
  //cout <<  <<" last line" << last_line;
 
  
  auto a = make_unique<Assign>();
  if(peek() != IDENT)
  {
    throw runtime_error("expected an IDENT for Assign");
  }
 
  expect(IDENT,"var name for assign");
  a->id = peekLex;

  auto confirm = symbolTable.find(a->id);

  if(confirm == symbolTable.end())
  {
    throw runtime_error("IDENT was not declared to Assign");
  }

  if(peek() != ASSIGN)
  {
    throw runtime_error("expected an ASSIGN (:=) after IDENT");
  }
  expect(ASSIGN,"Assign(:=) operator");
  Token tok = peek();
  a->type = tok;
  
  if(tok != INTLIT && tok != FLOATLIT && tok!= IDENT)
  {
    throw runtime_error("Expected Either INTLIT or FLOATLIT or IDENT after assign");
  }

  //cout << "we are here before a->type" << endl;
  //  cout << a->type << endl;
  //cout << "this is the token" << tok << endl;
  //cout << "we made it before expect " << peekLex << endl; 
  expect(tok,"Value to be assigned to variable");
  a->value = peekLex;

  //cout << "we made it past expect " << peekLex << endl;

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

      throw runtime_error("Expected either READ,WRITE,IDENT, or BEGIN in statement");

      break;
  }


  
  
  
}

unique_ptr<CompoundStatement> parseCompound()
{

 auto c = make_unique<CompoundStatement>();
 //Token type; 

  if(peek() != TOK_BEGIN)
  {
    throw runtime_error("exepected BEGIN to start compound statement");
  }
  expect(TOK_BEGIN,"BEGIN for Compound");

  c->stmts.push_back(parseStatement());

    // if(peek() == SEMICOLON)
    // {
    //  //cout << "we are in here" << endl; 
    //   //throw runtime_error("Started Compound statement without a semicolon");
    //   expect(SEMICOLON,"SEMICOLON after statment");

    // }

   

  //c->stmts.push_back()
  do
  {

    //cout << peekLex << endl;

    if(peek() == SEMICOLON)
    {
     //throw runtime_error("Started Compound statement without a semicolon");
      expect(SEMICOLON,"SEMICOLON after statment");

    }

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
  
  if(peek() != END)
  {
    throw runtime_error("expected end to end compound statement");
  }
  
  expect(END,"end of compound statement");

  return c;

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


// unique_ptr<Declarations> parseDeclarations()
// {
//   auto d = make_unique<Declarations>();

//   bool var_list = true;

//   if(peek()!= VAR)
//   {
//     throw runtime_error("expected VAR after IDENT since there was no BEGIN");
//   }
//   expect(VAR,"Variable declaratons");

//   while(var_list)
//   {
   
//     if(peek() != IDENT){throw runtime_error("Expected IDENT after VAR");}


//     expect(IDENT,"variable name");

//     d->name = peekLex; // stoes the name of the variable. 
   
    
//     if(peek() != COLON){throw runtime_error("Expected colon (:) after IDENT");}

//     expect(COLON,"Colon");

//     d->type = peek(); // stores the token type. 
//     //std::cout << peekLex << std::endl;

//     if(d->type != INTEGER && d->type != REAL) // if the variable is neither a INTERGER or a REAL number it errors out. 
//     {
//       throw runtime_error("Expected INTEGER or REAL after COLON (:)");
//     } 
//     expect(d->type,"Value of intialized variable");

//     //cout << peekLex << endl;
//     if(peek() != SEMICOLON){throw runtime_error("Expected a SEMICOLON(;) after variable type");} //errors out if no SEMICOLON

//     expect(SEMICOLON,"SEMICOLON after variable type");

//     // auto it = symbolTable.find(var_name);

//     // if(it!= symbolTable.end())
//     // {
//     //   throw runtime_error("duplicate");
//     // }

//     // if(var_type == INTEGER)
//     // {
//     //   symbolTable[var_name] = 0;
//     // }
//     // else
//     // {
//     //   symbolTable[var_name] = 0.0;
//     // }

//     //std::cout << "right before IDENT at the end " << peekLex << std::endl;
//     if(peek()!= IDENT)
//     {
//       var_list = false;
//     }

//   }

//   return d;

// }


unique_ptr<valueNode> parseTerm()
{
  auto node = parseFactor();
  
  while(true) {

    Token t = peek();

    if (t == MULTIPLY || t == DIVIDE || t = MOD || t == CUSTOM_OPER)
    {
     
      Token op = t;

      expect(t,"multiplicative operator (*,/,MOD,^^) in term");
      auto rhs = parseFactor();

      auto bin = make_unique<BinaryOP>();

      bin->op = op;
      bin->left = move(node);
      bin->right = move(rhs);
      node = move(bin);
    }
    else {
      break;
    }
  }

  return node;

}





unique_ptr<valueNode> parseValue()
{
  auto node = parseTerm();

  while(true) {

    Token t = peek();
    
    if(t == PLUS || t == MINUS) {

      Token op = t;
      expect(t, "additive operator (+,-) in Value");
      auto rhs = parseTerm();

      auto bin = make_unique<BinaryOP>();

      bin->op = op;

      bin->left = move(node);
      bin->right = move(rhs);

      node = move(bin);
    }
    else {
      break;
    }
  }
  return node;
}


unique_ptr<Block> parseBlock()
{
  auto b =  make_unique<Block>();
  //auto d = make_unique<Declarations>();
  string var_name; 
  Token var_type; 
  bool var_list = true;


  if(peek() == TOK_BEGIN){var_list = false;}
  
 if(peek() == VAR) //checks to see if the variable decleartions exists
  {
    // std::cout << peek() << std::endl;;
    // std::cout << peekLex << std::endl;
    // throw runtime_error("EXPECTED VAR since there was not a begin token");
    expect(VAR,"Var Declarations");
  }



  while(var_list)
  {

    if(peek() != IDENT){throw runtime_error("Expected IDENT after VAR");}


    expect(IDENT,"variable name");

    var_name = peekLex; // stoes the name of the variable. 
   
    
    if(peek() != COLON){throw runtime_error("Expected colon (:) after IDENT");}

    expect(COLON,"Colon");

    var_type = peek(); // stores the token type. 
    //std::cout << peekLex << std::endl;

    if(var_type != INTEGER && var_type != REAL) // if the variable is neither a INTERGER or a REAL number it errors out. 
    {
      throw runtime_error("Expected INTEGER or REAL after COLON (:)");
    } 
    expect(var_type,"Value of intialized variable");

    //cout << peekLex << endl;
    if(peek() != SEMICOLON){throw runtime_error("Expected a SEMICOLON(;) after variable type");} //errors out if no SEMICOLON

    expect(SEMICOLON,"SEMICOLON after variable type");

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

    //std::cout << "right before IDENT at the end " << peekLex << std::endl;
    if(peek()!= IDENT)
    {
      var_list = false;
    }


    

   

  }


  //std::cout << peekLex << std::endl;
  if(peek() != TOK_BEGIN)
  {
    throw runtime_error("Parse error: expected BEGIN");
  }
  //expect(TOK_BEGIN,"Beginning of Block");

  
  b->comp = parseCompound();


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

