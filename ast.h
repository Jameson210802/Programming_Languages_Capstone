// =============================================================================
//   ast.h — AST for TIPS Subset (matches PDF diagrams)
// =============================================================================
// MSU CSE 4714/6714 Capstone Project (Fall 2025)
// Author: Derek Willis
//
//   Part 1 : PROGRAM, BLOCK, WRITE
//   Part 2 : VAR/READ/ASSIGN + symtab + Compound Statement + BLOCK (fr this time)
//   Part 3 : expression/simple/term/factor + relations/logic/arithmetic
//   Part 4 : IF/WHILE, custom op/keyword, skins
// =============================================================================
#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <map>
#include <variant>
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------
// Pretty printer
// -----------------------------------------------------------------------------
inline void ast_line(ostream& os, string prefix, bool last, string label) {
  os << prefix << (last ? "└── " : "├── ") << label << "\n";
}

inline map<string, variant<int,double>> symbolTable;

// TODO: Define and Implement structures to hold each data node
// TODO: Overload << for Program


struct Statement
{



  virtual void interpret(ostream& out)=0;
  virtual void print_tree(ostream& os,string prefix)=0;

};

struct CompoundStatement : Statement
{

  vector<unique_ptr<Statement>> stmts;

  void print_tree(ostream& os,string prefix) override
  {
    //TODO fill in to print tree later
    for (auto& s: stmts) s->print_tree(os,"    "+ prefix);
  }

  void interpret(ostream& out) override{
    for(auto& s: stmts) s->interpret(out); }
};


struct Read : Statement
{


  string target;


  void print_tree(ostream&os,string prefix) override
  {

  }

  void interpret(ostream&out) override
  {
    
    auto it = symbolTable.find(target);

    if(it != symbolTable.end())
    {
     visit([&](auto& value) { cin >> value; }, it->second);
    }
    else
    {
      throw runtime_error("unable to find Identifer for Read.");
    }

  }


};

struct Assign : Statement
{

  string id; 
  Token type;
  string value;



  void print_tree(ostream&os,string prefix) override
  {

  }
  
  void interpret(ostream&out) override
  {
    int int_val;
    double double_val;
    auto it = symbolTable.find(id);
    if(type == INTLIT)
    {
      int_val = stoi(value);
      it->second = int_val;
    }
    else if (type == FLOATLIT)
    {
      double_val = stod(value);
      it->second = double_val;
    }
    else
    {
      auto ident_val = symbolTable.find(value); // gets the value of the identifier

      it->second = ident_val->second; // Sets the variable equal to Identifer VAL := IDENT
    }
  }


};








struct Write : Statement
{

  // string stored;
  //string stringlit;

  string content;
  Token type;
  
  //tringlit.pop_back();



  void interpret(ostream& out)
  {
    

    if(type == STRINGLIT)
    {
      out << content << endl;
    }
    else
    {
      auto it = symbolTable.find(content);

      if(it != symbolTable.end())
      {
        visit([&out](auto&& value) { out << value << endl; }, it->second);
      }
      else
      {
        throw runtime_error("unable to find Identifer for Read.");
      }
    }
    




    // if(stringlit.empty()){

    //   out << "\'\'";

    // }
    // else{
    //   out << stringlit;
    // }
  }
  void print_tree(ostream& os)
  {

    ast_line(os,"    ",true,"Write( " + content + " )");
    
  }

};




struct Block 
{

 // unique_ptr<Write> write;
  unique_ptr<CompoundStatement> comp;

  void print_tree(ostream& os,const char *tree,bool status)
  {
    ast_line(os,tree,status,"Block"); //TODO might need to remove this line. 
    if(comp) comp->print_tree(os," ");
  }


  void interpret(ostream& out) { if (comp) comp->interpret(out); }
};

struct Program 
{
  string name; 
  unique_ptr<Block> block;
  void print_tree(ostream& os)
  {
    cout << "Program\n";
    ast_line(os, "", false, "name: " + name);
    if (block) block->print_tree(os, "", true);
    else 
    { 
      ast_line(os, "", true, "Block"); 
      ast_line(os, "    ", true, "(empty)");
    }
  }
  void interpret(ostream& out) { if (block) block->interpret(out); }

  
  friend ostream& operator<<(ostream &out, unique_ptr<Program>& root)
  {

    
    root->print_tree(out);
   
    return out;
  }

};




