cmake_minimum_required(VERSION 3.8)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

# Main application
set(LEXER Lexer.cpp Lexer.hpp)
set(TOKENS Tokens.cpp Tokens.hpp)
set(PARSER_OLD Parser.hpp)
set(ENV Env.cpp Env.hpp)
set(INTER Inter.cpp Inter.hpp)
set(MISC Tag.hpp)

add_executable(compiler main.cpp ${LEXER} ${TOKENS} ${PARSER_OLD} ${ENV} ${INTER} ${MISC})

# Optimization part
set(OPTIMIZER asm/Lexer.cpp asm/Tokens.cpp asm/Tokens.hpp asm/Tag.hpp)

add_executable(optimizer asm/optimize.cpp ${OPTIMIZER})
