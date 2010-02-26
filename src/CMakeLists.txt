# scalc - A simple calculator
# Copyright (C) 2009, 2010  Alexander Korsunsky
#
# For terms and conditions of redistribution and modification of this file
# please see the file LICENSE.txt.

# we need at least cmake 2.8
cmake_minimum_required(VERSION 2.8)

# we are compiling the project "scalc"
project(scalc)

# we need the parsing module
add_subdirectory(parsing)


if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DYYDEBUG)
endif()

# the executable needs only main.cpp
add_executable(scalc main.cpp)

# link libraries to final executable
target_link_libraries(scalc scalc-parsing)
