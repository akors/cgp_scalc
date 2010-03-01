// scalc.y

/*
 *   scalc - A simple calculator
 *   Copyright (C) 2009, 2010  Alexander Korsunsky
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
#include <sstream>

#include "parsing.hpp"
#include "semantic.hpp"


#include "lex.scalc.hpp"

void print_prompt(const ParserOptions& parser_options);
void yyerror(const ParserOptions& parser_options, const char* s);

RegisteredPtrs<NumericValue> numericValue_Ptrs;

%}

%token  UINT
%token  NUMBER
%token  IDENTIFIER


%union
{
    NumericValue *numeric_value_ptr;
};

%type <numeric_value_ptr> expression
%type <numeric_value_ptr> number



// Turn on verbose error messages to get a proper error message
%error-verbose

// take parser options as argument
%parse-param {const ParserOptions& parser_options}

// declare operator precedence
%left '+' '-'
%left '*'
%left '/'
%right '^'
%left NEGATION


%%



// Parser input is none or more statements
input:
    // empty
    {
        // this action should only be performend on startup
        print_prompt(parser_options);
    }
|
    input statement
;


// a statement is an expression terminated by a newline
statement:
    expression '\n'
    {
        std::cout<<*$1<<'\n';
        print_prompt(parser_options);
    }
|   error '\n'
    {
        yyerrok;
        print_prompt(parser_options);
    }
|   '\n'
    {
        print_prompt(parser_options);
    }
;

// preliminarily, an expression is just a number
expression:
    number
    { $$ = $1; }
|   expression '+' expression
    {
        // add second expression to first, delete second, return first
        *$1 += *$3;
        numericValue_Ptrs.registeredDelete($3);
        $$ = $1;
    }

|   expression '-' expression
    {
        *$1 -= *$3;
        numericValue_Ptrs.registeredDelete($3);
        $$ = $1;
    }

|   expression '*' expression
    {
        *$1 *= *$3;
        numericValue_Ptrs.registeredDelete($3);
        $$ = $1;
    }

|   expression '/' expression
    {
        *$1 /= *$3;
        numericValue_Ptrs.registeredDelete($3);
        $$ = $1;
    }
|   expression '^' expression
    {
        $1->pow(*$3);
        numericValue_Ptrs.registeredDelete($3);
        $$ = $1;
    }

|   '-' expression  %prec NEGATION
    {
        $2->negate();
        $$ = $2;
    }

|   '(' expression ')'
    { $$ = $2; }
;

// A number is whatever looks a number
number:
    UINT
    {
        // create new NumericValue object
        $$ = numericValue_Ptrs.registerNew(new NumericValue);
        MEMORY_ASSERT($$);

        try {
            // try to read the number from the string
            $$->from_exact(yytext);
        }
        catch(const NumericError& e)
        {
            yyerror(parser_options, (std::string("Error: ")+e.what()).c_str());
            YYERROR;
        }
    }
|
    NUMBER
    {
        // create new NumericValue object
        $$ = numericValue_Ptrs.registerNew(new NumericValue);
        MEMORY_ASSERT($$);

        try {
            // try to read the number from the string
            $$->from_floating(yytext);
        }
        catch(const NumericError& e)
        {
            yyerror(parser_options, (std::string("Error: ")+e.what()).c_str());
            YYERROR;
        }
    }
;

%%

void yyerror(const ParserOptions& parser_options, const char* s)
{
    std::cerr<<s<<". line "<<yylineno<<std::endl;
}

void do_cleanup()
{
    numericValue_Ptrs.deleteAll();
}

void print_prompt(const ParserOptions& parser_options)
{
    if (!parser_options.file_input)
        printf("> ");
}
