// scalc.y

/*
 *   scalc - A simple calculator
 *   Copyright (C) 2009  Alexander Korsunsky
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


%{
#include <iostream>
#include "lex.scalc.hpp"
%}

%token  UINT
%token  NUMBER
%token  IDENTIFIER


%union
{
    double dbl;
};

%type <dbl> expression
%type <dbl> number

%{
    void yyerror(const char* s);
%}

// Turn on verbose error messages to get a proper error message
%error-verbose

// declare operator precedence
%left '+' '-'
%left '*'
%left '/'
%left NEGATION

%%


// Parser input is none or more statements
input:
    // empty
|
    input statement
;


// a statement is an expression terminated by a newline
statement:
    expression '\n'
    {
        std::cout<<"Found expression. Value: "<<$<dbl>1<<'\n';
    }
;

// preliminarily, an expression is just a number
expression:
    number
    { $$ = $1; }
|   expression '+' expression
    { $$ = $1 + $3; }

|   expression '-' expression
    { $$ = $1 - $3; }

|   expression '*' expression
    { $$ = $1 * $3; }

|   expression '/' expression
    { $$ = $1 / $3; }

|   '-' expression  %prec NEGATION
    { $$ = -$2; }

|   '(' expression ')'
    { $$ = $2; }
;

// A number is whatever looks a number
number:
    UINT
    { $$ = strtod(yytext, NULL); }
|
    NUMBER
    { $$ = strtod(yytext, NULL); }
;

%%

void yyerror(const char* s)
{
    std::cerr<<s<<", line "<<yylineno<<std::endl;
}
