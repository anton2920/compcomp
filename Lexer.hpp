#ifndef LEXER
#define LEXER

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "Tokens.hpp"

class Lexer {
private: 
    char peek = ' ';
    std::ifstream *input;
    std::unordered_map<std::string, Token*> words;
    void reserve(Word* w);
    void readch();
    bool readch(char c);
public:
    static int line, prev_line;
    bool iseof = false;
    explicit Lexer(std::ifstream &inp);
    static bool is_roman(std::string &str);
    Token* scan();
};

#endif