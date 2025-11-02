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

// note got the holds_alternative  from: https://www.geeksforgeeks.org/cpp/std-variant-in-cpp-17/


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
    ast_line(os,prefix+"   ",true,"Compound Statement");
    for (auto& s: stmts) s->print_tree(os,"\t"+ prefix);
  }

  void interpret(ostream& out) override{
    for(auto& s: stmts) s->interpret(out); }
};


struct Read : Statement
{


  string target;


  void print_tree(ostream&os,string prefix) override
  {
    //(void)prefix;
    
    ast_line(os,prefix,true,"read " +target);

  }

  void interpret(ostream&out) override
  {
    (void)out;
    
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
    ast_line(os,prefix,false,"Assign := " + value);
  }
  
  void interpret(ostream&out) override
  {
    (void)out;
    auto it = symbolTable.find(id);
    auto ident_val = symbolTable.find(value);
  
    if(it != symbolTable.end())
    {

      switch (type)
      {
        case INTLIT:
          
          it->second = stoi(value);

          break;
        case FLOATLIT:

          it->second = stod(value);

          break;
        case IDENT: 

          // gets the value of the identifier

          if(ident_val != symbolTable.end())
          {
            it->second = ident_val->second; // Sets the variable equal to Identifer VAL := IDENT
          }
          else
          {
            throw runtime_error("was unable to find value to assign it to new variable");
          }
          break;
          
        default:
        
          throw runtime_error("Do not have one the nessarcy types to assign a veraible");

          break;
      }
      
    }
    else
    {
      throw runtime_error("was unable to find value to assign it to new variable");
    }


  }
};







struct Write:Statement
{

  // string stored;
  //string stringlit;

  string content;
  Token type;
  
  //tringlit.pop_back();



  void interpret(ostream& out)
  {
    //out << content << " " << type << endl;

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
        throw runtime_error("unable to find Identifer for write.");
      }
    }
    
  }

  void print_tree(ostream& os,string prefix)
  {

    ast_line(os,prefix,true,"Write");
    ast_line(os,prefix+"    ",true,"content: " +content);
    
  }

 

};

// struct Declarations
// {

//   Token type;
//   string name;


//   void print_tree(ostream& os,string prefix)
//   {

//     ast_line(os,prefix+"    ",false,"Declarations:");
//     if(type == INTEGER)
//     {
//       ast_line(os,prefix+"        ",false,name +" : INTEGER = 0");
//     }
//     else
//     {
//       ast_line(os,prefix+"        ",false,name +" : REAL = 0.0000");

//     }
    
//   }

//   void interpret(ostream& out)
//   {
//     auto it = symbolTable.find(name);
//     out << "we are in here " << type << " "<< name << endl;
//     if(it!= symbolTable.end())
//     {
//       throw runtime_error("duplicate");
//     }

//     if(type == INTEGER)
//     {
//       symbolTable[name] = 0;
//     }
//     else
//     {
//       symbolTable[name] = 0.0;

//     }
//   }
// };





struct Block 
{

 // unique_ptr<Write> write;
  unique_ptr<CompoundStatement> comp;
  //vector<int> //TODO look at later
  //unique_ptr<Declarations> declare;
  void print_tree(ostream& os,const char *tree,bool status)
  {

    ast_line(os,tree,status,"Block"); //TODO might need to remove this line. 
    ast_line(os,"    ",false,"Declearatons");

    for(auto & pair: symbolTable)
    {
      if(holds_alternative<int>(pair.second)) 
      {
        //string value_to_send = to_string(pair.second);
        ast_line(os,"    |   ",false, pair.first + ": INTEGER = 0");
      }
      else if (holds_alternative<double>(pair.second))
      {
        ast_line(os,"    |   ",false, pair.first + ": REAL = 0.0");
      }
      
    }
    
    if(comp) comp->print_tree(os," ");
  }


  void interpret(ostream& out)
  { 
    
    //if(declare){out<< "hi";declare->interpret(out);}
    
    if(comp){comp->interpret(out);}
  
  }
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

  void print_symbols(ostream& out)
  {
    for(auto & pair: symbolTable)
    {
      if(holds_alternative<int>(pair.second)) 
      {
        //string value_to_send = to_string(pair.second);
        //ast_line(os,"    |   ",false, pair.first + ": INTEGER = " +);
        out << pair.first << ": INTEGER = " << get<int>(pair.second)<< "\n";
      }
      else if (holds_alternative<double>(pair.second))
      {
        out << pair.first << ": REAL = " << get<double>(pair.second)<< "\n";
      }
      
    }
  }
  friend ostream& operator<<(ostream &out, unique_ptr<Program>& root)
  {

    
    root->print_tree(out);
   
    return out;
  }

};




