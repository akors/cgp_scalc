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


digit   [0-9]
letter  [A-Za-z]

    // see http://www.cplusplus.com/reference/clibrary/cstdlib/strtod.html
    // for an exact definition of how a floating point number looks like
exponent_part   [eE][-+]?{digit}+
number  [-+]?{digit}+\.?{digit}*{exponent_part}?

u_integer   {digit}+
integer [-+]?{u_integer}



%{

#include <iostream>



%}



%%



{u_integer}   { // only unsigned integers
                std::cout<<"Found unsigned Integer: "<<yytext;
            }

{integer}   { // only integers
                std::cout<<"Found Integer: "<<yytext;
            }


{number}  { // any other numerical value
                std::cout<<"Found Number: "<<yytext;
        }

[-+*/();]     std::cout<<"Found special character: "<<*yytext; // special characters

[ \t]   ; /*skip whitespace */


.   std::cerr<< ("Invalid characters!");

%%

// callback function that is called when lex reached the end of input
int yywrap()
{
    return 1;
}

