# scalc - A simple calculator
# Copyright (C) 2009, 2010  Alexander Korsunsky
#
# For terms and conditions of redistribution and modification of this file
# please see the file LICENSE.txt.

# we need at least cmake 2.6
cmake_minimum_required(VERSION 2.6)

# we need the parsing module
add_subdirectory(parsing)

# the executable needs only main.cpp
add_executable(scalc main.cpp)

# link libraries to final executable
target_link_libraries(scalc scalc-parsing)
