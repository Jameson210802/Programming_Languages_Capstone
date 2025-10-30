/*****************************************************************************
 *   rules.l - Flex Rules for TIPS Language Subset
 *   MSU CSE 4714/6714 Capstone Project (Fall 2025)
 *   Author: Derek Willis
*****************************************************************************/
%option noyywrap nodefault nounput
%option yylineno

%{
#include "lexer.h"
%}

%%
 /* DEFINE YOUR RULES STARTING HERE */

PROGRAM        {return PROGRAM;}
[\t\r\n]+            {        }
" "                 {   }
BEGIN          {return TOK_BEGIN;}
END            {return END;}
WRITE          {return WRITE;}
[A-Z][A-Z0-9]{8,} {return UNKNOWN;}
[A-Z][A-Z0-9]{0,7}     {return IDENT;}
'[^\'\n]{81,}'   {return UNKNOWN;} 
'[^\'\n]{0,80}'   {return STRINGLIT;} 

VAR               {return VAR;}
INTEGER          {return INTEGER;}
REAL            {return REAL;}


READ               {return READ;}
:=                 {return ASSIGN;}
[0-9]+          {return INTLIT;}
[0-9]+.[0-9]+   {return FLOATLIT;}


:                  {return COLON;}



\(                {return OPENPAREN;}
\)                {return CLOSEPAREN;}
;                {return SEMICOLON;}

<<EOF>>        {return TOK_EOF;}
.              {return UNKNOWN;}





%%
