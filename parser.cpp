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
unique_ptr<ifStatment> parseIf();
unique_ptr<whileStatement> parseWhile();
unique_ptr<expression> parseExpression();


unique_ptr<valueNode> parseValue();
unique_ptr<valueNode> parseTerm();
unique_ptr<valueNode> parseFactor();
unique_ptr<valueNode> parsePrimary();


//unique_ptr<Declarations> parseDeclarations();


unique_ptr<expression> parseExpression()
{
 
  auto node = make_unique<expression>();


  node->value.push_back(parseValue()); // gets left node;

  if(peek() == CLOSEPAREN)
  {
    expect(CLOSEPAREN,"expected open paren");
  }
 

  Token t = peek();

  if(t == LESSTHAN || t == GREATERTHAN || t == EQUALTO || t == NOTEQUALTO) // checks to see if there is a right side to the expresion 
  {

    node->relationOperator = t;
    expect(t,"operation value");

    node->value.push_back(parseValue());
    if(peek() == CLOSEPAREN)
    {
      expect(CLOSEPAREN,"expected open paren");
    }
  }


  t = peek();

  if(t == TOK_AND || t == TOK_OR)
  {
    node->logicalOperator = t;

    node->value.push_back(parseExpression());
  }



  

  



  // /// LEFT NODE ONLY THING CALLED IF THAT IS IT
  // cout << "THIS IS TOKEN left val " << peekLex << endl << endl;
  // node->leftValue.push_back(parseValue()); 


  // if(peek() == CLOSEPAREN) // closes parentheses if there
  // {
  //   expect(CLOSEPAREN,"expect close Parentheses");
  // }






  // Token t = peek(); // checks to see if there is a relational operator

  // cout << "THIS IS TOKEN right val " << peekLex << endl << endl;


  // if(t == LESSTHAN || t == GREATERTHAN || t == EQUALTO || t == NOTEQUALTO) // checks to see if there is a right side to the expresion 
  // {
  //   expect(t,"operation value");

    
 
  //  node->rightValue.push_back(parseValue());
  // }

  // if(peek() == CLOSEPAREN)
  // {
  //   expect(CLOSEPAREN,"expect close Parentheses");
  // }

  // t = peek();
  // cout << "THIS IS TOKEN AND / OR val " << peekLex << endl << endl;
  // if(t == TOK_AND || t == TOK_OR)
  // {

  //   node->rightValue.push_back(parseExpression());

  // }

  // if(peek() == CLOSEPAREN)
  // {
  //   expect(CLOSEPAREN,"expect close Parentheses");
  // }
  
 
  return node;
  
}



unique_ptr<ifStatment> parseIf()
{

  
  auto if_node = make_unique<ifStatment>(); // creating if statment node


  // expresion

 

  if(peek() == OPENPAREN)
  {
    expect(OPENPAREN,"expect open Parentheses");
  }
  else
  {
    throw runtime_error("did not get paren after while");
  }


  if_node->express = parseExpression();

  


  // if_node->expression_values.push_back(parseValue()); // getting value from 

  // Token t = peek();

  // if(t == LESSTHAN || t == GREATERTHAN || t == EQUALTO || t == NOTEQUALTO)
  // {
  //   expect(t,"operation value");
  //   if_node->expression_values.push_back(parseValue()); //TODO CHECK and see if you need store token 
  // }

  // if(peek() == CLOSEPAREN)
  // {
  //   expect(CLOSEPAREN,"expect open Parentheses");
  // }
  // else
  // {
  //   throw runtime_error("did get paren after while");
  // }
  // expreson done.

  // checking for then

  if(peek() == THEN)
  {
    expect(THEN,"Expected THEN");
  }
  else
  {
    throw runtime_error("Did not recieve THEN");
  }


  cout << "WE ARE HERE" << endl;

  if_node->stmnts.push_back(parseStatement());


  if(peek() == ELSE) // 
  {
    if_node->stmnts.push_back(parseStatement());
  }




  return if_node;

}


unique_ptr<whileStatement> parseWhile()
{

  auto while_node = make_unique<whileStatement>();

  if(peek() == OPENPAREN)
  {
    expect(OPENPAREN,"expect open Parentheses");
  }
  else
  {
    throw runtime_error("did get paren after while");
  }

  
  while_node->express = parseExpression();
 

  // while_node->expression_values.push_back((parseValue()));

  // Token t = peek();

  // if(t == LESSTHAN || t == GREATERTHAN || t == EQUALTO || t == NOTEQUALTO)
  // {
  //   expect(t,"operation value");

  //   while_node->expression_values.push_back((parseValue())); //TODO CHECK and see if you need store token 
  // }


  
  // if(peek() == CLOSEPAREN)
  // {
  //   expect(CLOSEPAREN,"expect open Parentheses");
  // }
  // else
  // {
  //   throw runtime_error("did get paren after while");
  // }

  while_node->stmnt = parseStatement();


  return while_node;


}










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



unique_ptr<valueNode> parsePrimary()
{

  Token t = peek();
  //cout << "THIS IS PRIMARY TOKEN " << peek() << endl; 
  if(t == FLOATLIT)
  {
    expect(t,"FLOATINT in primary");

    auto node = make_unique<RealLitNode>();

    node->v = stod(peekLex);

    return node;

  }
  else if(t == INTLIT)
  {

    expect(t,"INTLIT in primary");

    auto node = make_unique<RealLitNode>();

    node->v = stoi(peekLex);
    

    return node;

  }
  else if(t == IDENT)
  {
    expect(t,"IDENT in primary");


    auto node = make_unique<IdentLitNode>();

    node->name = peekLex;

    return node;
  }
  else if(t == OPENPAREN)
  {
    expect(t,"Open Parentheses in Primary");
    
    auto node = parseValue();
    
    t = peek();

    expect(t,"Open Parentheses in Primary");

    return node;
  }
  else{
    throw runtime_error("Did not find proper ");
  }

  

  //if(t== CLOSEPAREN){expect(t,"Open Parentheses in Primary");}

  //todo might need to add runtime errors

  //return node;



}

unique_ptr<valueNode> parseFactor()
{
  Token t = peek();
  if(t==INCREMENT || t == DECREMENT || t == MINUS || t == TOK_NOT)
  {
    Token op = t;
    expect(t,"unary operator (++/--/-) in factor");


    auto p = parsePrimary();

    auto node = make_unique<UnaryOP>();

    node->op = op;

    node->sub = move(p);

    return node;


  }

  return parsePrimary();
}








unique_ptr<valueNode> parseTerm()
{
  auto node = parseFactor();
  
  while(true) {

    Token t = peek();

    if (t == MULTIPLY || t == DIVIDE || t == MOD || t == CUSTOM_OPER || t == TOK_OR)
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
    cout << "value lex " << peekLex << endl << endl;
    if(t == PLUS || t == MINUS || t == TOK_AND) {

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


unique_ptr<Assign> parseAssign()
{


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



  a->rhs = parseValue();



  


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
    
    case IF:
      expect(IF,"IF token");
      return parseIf();
      break;

    case WHILE:
      expect(WHILE,"while token");
      return parseWhile();
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


  }while (peek() == SEMICOLON && nextTok());
  
  if(peek() != END)
  {
    throw runtime_error("expected end to end compound statement");
  }
  
  expect(END,"end of compound statement");

  return c;

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

