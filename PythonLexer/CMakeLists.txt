﻿# CMakeList.txt : CMake project for PythonLexer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (PythonLexer "PythonLexer.cpp" "PythonLexer.h" "lexer.cpp" "escape.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PythonLexer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
