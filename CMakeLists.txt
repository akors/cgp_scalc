# scalc - A simple calculator
# Copyright (C) 2009, 2010  Alexander Korsunsky
#
# For terms and conditions of redistribution and modification of this file
# please see the file LICENSE.txt.

# we need at lest cmake 2.6
cmake_minimum_required(VERSION 2.6)

# we are compiling the project "scalc"
project(scalc)

# Add source directory, place resulting files in build directory
add_subdirectory(src build)
