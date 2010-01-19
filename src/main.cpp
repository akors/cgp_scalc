// main.cpp

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

#include <iostream>
#include <cstring>

extern int yyparse();

#if defined(YYDEBUG)
    extern int yydebug;
#endif

int main(int argc, char** argv)
{
#if defined(YYDEBUG)
    // turn on debugging when -d option is specified
    if (argc > 2 && !strcmp("-d", argv[2]))
        yydebug = 1;
#endif

    yyparse();
    return 0;
}
