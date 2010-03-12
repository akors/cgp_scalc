// main.cpp

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

#include <iostream>
#include <cstring>
#include <stdexcept>

#include <cstdio>

#include "parsing/parsing.hpp"

const char* usage_string =
    "Usage: scalc <options> <inputfile>\n"
    "\t-h:\t\tDisplay help\n"
#if defined(YYDEBUG)
    "\t-d:\t\tDisplay parser debug information on error\n"
#endif
    "\n"
    "\t<inputfile>:\tInput file for reading operations. If not specified,\n"
    "\t\tread from stdin.\n"
    ;

int main(int argc, char** argv)
{
    ParserOptions parser_options = {
        false
    };

    // name of the input file, NULL if reading from stdin
    char *infilename = NULL;

    unsigned positional_count = 0;

    // iterate through arguments, retrieve options
    for (unsigned i = 1; i < argc; ++i)
    {
        if (!strcmp("-h", argv[i]) || !strcmp("--help", argv[i]))
        {
            std::cout<<usage_string;
            return 0;
        }
#if defined(YYDEBUG)
        // turn on debugging when -d option is specified
        else if (!strcmp("-d", argv[i]) || !strcmp("--debug", argv[i]))
            yydebug = 1;
#endif
        // retrieve positional options
        else
        {
            switch(positional_count)
            {
                // first option is the input filename
                case 0:
                    infilename = argv[i];
                    break;
                default:
                    std::cerr<<"Unknown option \""<<argv[i]<<"\"";
                    return 0;
            }

            ++positional_count;
        }
    }

    // set yyin to the input file, if specified
    if (infilename != NULL)
    {
        yyin = fopen(infilename, "r");
        if (yyin == NULL)
        {
            perror ("Failed to open file");
            return 0;
        }

        parser_options.file_input = true;
    }

    // do the parsing
    try {
        yyparse(parser_options);

        if (!parser_options.file_input)
            std::cout<<std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr<<"Encountered exception while parsing: "<<e.what()<<'\n';
        do_cleanup();
    };


    return 0;
}
