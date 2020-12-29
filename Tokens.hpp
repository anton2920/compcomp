#ifndef TOKENS
#define TOKENS

#include "Tag.hpp"
#include <iostream>
#include <utility>

class Token {
public:
    Tag tag;

    Token() = default;

    explicit Token(Tag t)
    {
        this->tag = t;
    }

    virtual std::string toString()
    {
        return std::string(1, (char) this->tag);
    }
};


class Num : public Token {
public:
    int value;

    explicit Num(int v) : Token(Tag::NUM)
    {
        this->value = v;
    }

    std::string toString() override
    {
        return std::to_string(this->value);
    }
};

class Hex : public Num {
public:
    explicit Hex(int v) : Num(0)
    {
        this->value = static_cast<int>(std::strtol(std::to_string(v).c_str(), nullptr, 16));
    }
};

class Roman : public Num {
    static int convert(std::string &str)
    {
        /* Initializing variables */
        int res = 0;

        /* Main part */
        for (int i = 0; i < str.length(); i++) {
            // Getting value of symbol s[i]
            int s1 = str[i] == 'I' ? 1 : str[i] == 'V' ? 5 : str[i] == 'X' ? 10 : -1;

            if (i + 1 < str.length()) {
                int s2 = str[i + 1] == 'I' ? 1 : str[i + 1] == 'V' ? 5 : str[i + 1] == 'X' ? 10 : -1;

                if (s1 >= s2) {
                    res = res + s1;
                } else {
                    res = res + s2 - s1;
                    i++;
                }
            } else {
                res = res + s1;
            }
        }
        return res;
    }

public:
    explicit Roman(std::string &num) : Num(0)
    {
        this->tag = Tag::ROMAN;
        /*original_val = num;*/
        this->value = convert(num);
    }
};

class Real : public Token {
public:
    float value;

    explicit Real(float v) : Token(Tag::REAL)
    {
        this->value = v;
    }

    std::string toString() override
    {
        return std::to_string(this->value);
    }
};


class Word : public Token {
public:
    std::string lexeme;

    Word() : Token((Tag) 0)
    {};

    Word(std::string s, Tag tag) : Token(tag)
    {
        this->lexeme = std::move(s);
    }

    static Word _and, _or, _eq, _ne, _le, _ge, _minus, _true, _false, _temp;

    std::string toString() override
    {
        return this->lexeme;
    }
};


class Type : public Word {
public:
    int width = 0;  // width is used for storage allocation

    Type() = default;

    Type(std::string s, Tag tag, int w) : Word(std::move(s), tag)
    {
        this->width = w;
    }

    static Type *_int, *_float, *_char, *_bool;

    static bool numeric(Type *p)
    {
        return (p == Type::_char || p == Type::_int || p == Type::_float);
    }

    static Type *max(Type *p1, Type *p2)
    {
        if (!numeric(p1) || !numeric(p2)) {
            return nullptr;
        } else if (p1 == Type::_float || p2 == Type::_float) {
            return Type::_float;
        } else if (p1 == Type::_int || p2 == Type::_int) {
            return Type::_int;
        } else {
            return Type::_char;
        }
    }
};


class Array : public Type {
public:
    Type *of;
    int size = 1;

    Array(int sz, Type *p) : Type("[]", Tag::INDEX, sz * p->width)
    {
        this->size = sz;
        this->of = p;
    }

    std::string toString() override
    {
        return "[" + std::to_string(this->size) + "]" + this->of->toString();
    }
};

#endif
