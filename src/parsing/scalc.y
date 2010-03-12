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

RegisteredPtrs<Expression> Expression_Ptrs;

%}

%token  UINT
%token  NUMBER
%token  IDENTIFIER


%union
{
    Expression *expression_ptr;
};

%type <expression_ptr> expression
%type <expression_ptr> number



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
        std::cout<<*$1<<std::endl;
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
        // unregister operand Expressions
        Expression_Ptrs.unregister($1);
        Expression_Ptrs.unregister($3);

        // allocate new expression with proper operation, return expression
        $$ = Expression_Ptrs.registerNew(
            new BinaryOperation(
                Expression::ptr_t($1),
                Expression::ptr_t($3),
                &plus_op
        ));
        MEMORY_ASSERT($$);
    }

|   expression '-' expression
    {
        // unregister operand Expressions
        Expression_Ptrs.unregister($1);
        Expression_Ptrs.unregister($3);

        // allocate new expression with proper operation, return expression
        $$ = Expression_Ptrs.registerNew(
            new BinaryOperation(
                Expression::ptr_t($1),
                Expression::ptr_t($3),
                &minus_op
        ));
        MEMORY_ASSERT($$);
    }

|   expression '*' expression
    {
        // unregister operand Expressions
        Expression_Ptrs.unregister($1);
        Expression_Ptrs.unregister($3);

        // allocate new expression with proper operation, return expression
        $$ = Expression_Ptrs.registerNew(
            new BinaryOperation(
                Expression::ptr_t($1),
                Expression::ptr_t($3),
                &multiply_op
        ));
        MEMORY_ASSERT($$);
    }

|   expression '/' expression
    {
        // unregister operand Expressions
        Expression_Ptrs.unregister($1);
        Expression_Ptrs.unregister($3);

        // allocate new expression with proper operation, return expression
        $$ = Expression_Ptrs.registerNew(
            new BinaryOperation(
                Expression::ptr_t($1),
                Expression::ptr_t($3),
                &divide_op
        ));
        MEMORY_ASSERT($$);
    }
|   expression '^' expression
    {
        // unregister operand Expressions
        Expression_Ptrs.unregister($1);
        Expression_Ptrs.unregister($3);

        // allocate new expression with proper operation, return expression
        $$ = Expression_Ptrs.registerNew(
            new BinaryOperation(
                Expression::ptr_t($1),
                Expression::ptr_t($3),
                &pow_op
        ));
        MEMORY_ASSERT($$);
    }

|   '-' expression  %prec NEGATION
    {
        // unregister operand Expressions
        Expression_Ptrs.unregister($2);

        // allocate new expression with proper operation, return expression
        $$ = Expression_Ptrs.registerNew(
            new UnaryOperation(Expression::ptr_t($2), &negation_op)
        );
        MEMORY_ASSERT($$);
    }

|   '(' expression ')'
    { $$ = $2; }
;

// A number is whatever looks a number
number:
    UINT
    {
        NumericValue val;

        try {
            // try to read the number from the string
            val.from_exact(yytext);
        }
        catch(const NumericError& e)
        {
            yyerror(parser_options, (std::string("Error: ")+e.what()).c_str());
            YYERROR;
        }

        // create new NumericValueExpression object
        $$ = Expression_Ptrs.registerNew(new NumericExpression(val));
        MEMORY_ASSERT($$);
    }
|
    NUMBER
    {
        NumericValue val;

        try {
            // try to read the number from the string
            val.from_floating(yytext);
        }
        catch(const NumericError& e)
        {
            yyerror(parser_options, (std::string("Error: ")+e.what()).c_str());
            YYERROR;
        }

        // create new NumericValueExpression object
        $$ = Expression_Ptrs.registerNew(new NumericExpression(val));
        MEMORY_ASSERT($$);
    }
;

%%

void yyerror(const ParserOptions& parser_options, const char* s)
{
    std::cerr<<s<<". line "<<yylineno<<std::endl;
}

void do_cleanup()
{
    Expression_Ptrs.deleteAll();
}

void print_prompt(const ParserOptions& parser_options)
{
    if (!parser_options.file_input)
        printf("> ");
}
