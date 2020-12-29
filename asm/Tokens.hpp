#ifndef TOKENS
#define TOKENS

#include "Tag.hpp"
#include <iostream>

class Token {
public:
    Tag tag;
    Token() {};
    Token(Tag t) { tag = t; }

    virtual std::string toString() {
        return std::string(1, (char) tag);
    }
};


class Num : public Token {
public: 
    int value;
    Num(int v): Token(Tag::NUM) {
        value = v;
    }
    virtual std::string toString() {
        return std::to_string(value);
    }
};

class Real : public Token {
public: 
    float value;
    Real(float v): Token(Tag::REAL) {
        value = v;
    }
    virtual std::string toString() {
        return std::to_string(value);
    }
};


class Word: public Token {
public: 
    std::string lexeme = "";
    int use_count = 1;
    Word(): Token((Tag)0) {};
    Word(std::string s, Tag tag): Token(tag) {
        lexeme = s;
    }
    static Word 
        _and, _or, _eq, _ne, _le, _ge, 
        _minus, _true, _false, _temp;
    virtual std::string toString() {
        return lexeme;
    }
};


class Type: public Word {
public:
    int width = 0;  // width is used for storage allocation
    Type() {}
    Type(std::string s, Tag tag, int w): Word(s, tag) { 
        width = w; 
    }
    static Type *_int, *_float, *_char, *_bool;
    static bool numeric(Type * p) {
        return (p == Type::_char || p == Type::_int || p == Type::_float);
    }
    static Type * max(Type *p1, Type *p2) {
        if ( !numeric(p1) || !numeric(p2) ) return nullptr;
        else if (p1 == Type::_float || p2 == Type::_float) return Type::_float;
        else if (p1 == Type::_int || p2 == Type::_int) return Type::_int;
        else return Type::_char;
    }
};


class Array: public Type {
public:
    Type *of;
    int size = 1;
    Array(int sz, Type *p)
        :Type("[]", Tag::INDEX, sz * p->width) {
        size = sz;
        of = p;
    }
    virtual std::string toString() {
        return "[" + std::to_string(size) + "]" + of->toString();
    }
};


#endif