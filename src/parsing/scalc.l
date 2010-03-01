/* scalc.l */

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
#include "semantic.hpp"
#include "scalc.tab.hpp"

%}

    // create a start condition for comments
%x comment_cond

digit   [0-9]
letter  [A-Za-z]

identifier  ({letter}|_)+({letter}|{digit}|_)*

    // see http://www.cplusplus.com/reference/clibrary/cstdlib/strtod.html
    // for an exact definition of how a floating point number looks like
exponent_part   [eE][-+]?{digit}+
number  {digit}+\.?{digit}*{exponent_part}?

u_integer   {digit}+


%{

#include <iostream>

%}

%option yylineno

%%

#   {
        // set lexer to comment condition
        BEGIN(comment_cond);
    }

<comment_cond>\n {
        // every comment ends with a newline, so end condition here
        BEGIN(INITIAL);
        return *yytext;
    }

<comment_cond>. {
    // when in comment mode, eat any character that is not a newline
}

{u_integer}   { // only unsigned integers
        return UINT;
    }


{identifier}    {
        return IDENTIFIER;
    }

{number}  { // any other numerical value
        return NUMBER;
    }

[-+*/\^();]    return *yytext; // special characters

\n  return *yytext;

[ \t]   ; /*skip whitespace */


.   return *yytext; // return the rest to the responsibility of the parser

%%

// callback function that is called when lex reached the end of input
int yywrap()
{
    return 1;
}

