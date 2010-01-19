# scalc - A simple calculator
# Copyright (C) 2009, 2010  Alexander Korsunsky
#
# For terms and conditions of redistribution and modification of this file
# please see the file LICENSE.txt.

# we need Flex and Bison packages, otherwise we cant compile
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

# create targets for Flex and Bison
BISON_TARGET(ScalcParser scalc.y ${CMAKE_CURRENT_BINARY_DIR}/scalc.tab.cpp)
FLEX_TARGET(ScalcScanner scalc.l ${CMAKE_CURRENT_BINARY_DIR}/lex.scalc.cpp
    COMPILE_FLAGS "--header-file=${CMAKE_CURRENT_BINARY_DIR}/lex.scalc.hpp")

ADD_FLEX_BISON_DEPENDENCY(ScalcScanner ScalcParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(scalc-parsing
    ${BISON_ScalcParser_OUTPUTS}
    ${FLEX_ScalcScanner_OUTPUTS}
)
