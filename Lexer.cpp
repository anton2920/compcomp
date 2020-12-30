#include <cstring>
#include "Lexer.hpp"

int Lexer::prev_line = 0;
int Lexer::line = 1;

static const char *romans = "XVI";

Lexer::Lexer(std::ifstream &inp)
{
    /* Initializing variables */
    input = &inp;

    /* Main part */
    reserve(new Word("if", Tag::IF));
    reserve(new Word("else", Tag::ELSE));
    reserve(new Word("while", Tag::WHILE));
    reserve(new Word("do", Tag::DO));
    reserve(new Word("break", Tag::BREAK));
    reserve(&Word::_true);
    reserve(&Word::_false);
    reserve(Type::_int);
    reserve(Type::_char);
    reserve(Type::_bool);
    reserve(Type::_float);
}

void Lexer::reserve(Word *w)
{
    /* Main part */
    words.insert({ w->lexeme, w });
}

void Lexer::readch()
{
    /* Initializing variables */
    char temp;

    /* Main part */
    if (!input->get(temp)) {
        iseof = true;
    } else {
        peek = temp;
    }
}

bool Lexer::readch(char c)
{
    /* Main part */
    readch();

    if (peek != c) {
        return false;
    }
    peek = ' ';

    /* Returning value */
    return true;
}

Token *Lexer::scan()
{
    /* Main part */
    Lexer::prev_line = Lexer::line;
    for (; !iseof; readch()) {
        if (peek == '\n') {
            ++line;
        } else if (std::isspace(peek)) {
            continue;
        } else {
            break;
        }
    }

    switch (peek) {
        case '&':
            if (readch('&')) {
                return (Token *) &(Word::_and);
            } else {
                return new Token((Tag) '&');
            }
        case '|':
            if (readch('|')) {
                return (Token *) &(Word::_or);
            } else {
                return new Token((Tag) '|');
            }
        case '=':
            if (readch('=')) {
                return (Token *) &(Word::_eq);
            } else {
                return new Token((Tag) '=');
            }
        case '!':
            if (readch('=')) {
                return (Token *) &(Word::_ne);
            } else {
                return new Token((Tag) '!');
            }
        case '<':
            if (readch('=')) {
                return (Token *) &(Word::_le);
            } else {
                return new Token((Tag) '<');
            }
        case '>':
            if (readch('=')) {
                return (Token *) &(Word::_ge);
            } else {
                return new Token((Tag) '>');
            }
        default:
            break;
    }

    if (isdigit(peek)) {
        int v = 0;

        /*if (peek == '0') {
            do {
                int add = 0;
                switch (peek) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        add = peek - '0';
                        break;
                    case 'F':
                        ++add;
                    case 'E':
                        ++add;
                    case 'D':
                        ++add;
                    case 'C':
                        ++add;
                    case 'B':
                        ++add;
                    case 'A':
                        add += 10;
                        break;
                    default:
                        add = 0;
                        break;
                }
                v = 16 * v + add;
                readch();
            } while (isxdigit(peek));
        } else {*/
            do {
                v = 10 * v + (peek - '0');
                readch();
            } while (isdigit(peek));
        // }

        if (peek != '.') {
            Token *n;
            if ((n = words[std::to_string(v)])) {
                return n;
            }

            n = new Num(v);
            words.insert({ std::to_string(v), n });
            return n;
        }

        auto x = static_cast<float>(v);
        float d = 10;
        for (;;) {
            readch();
            if (!isdigit(peek)) {
                break;
            }

            x = x + static_cast<float>(peek - '0') / d;
            d = d * 10;
        }

        Token *r;
        if ((r = words[std::to_string(x)])) {
            return r;
        }

        r = new Real(x);
        words.insert({ std::to_string(x), r });
        return r;
    } else if (isalpha(peek)) {
        std::string s;
        do {
            s += peek;
            readch();
        } while (isalnum(peek));
        Token *w;
        if ((w = words[s])) {
            return w;
        }
        if (s.find_first_not_of(romans) == std::string::npos && is_roman(s)) {
            w = new Roman(s);
        } else {
            w = new Word(s, Tag::ID);
        }
        words.insert({ s, w });
        return w;
    }
    // do i need this?
    if (iseof) {
        return new Token((Tag) 0);
    }

    auto *tok = new Token((Tag) peek);
    peek = ' ';

    /* Returning value */
    return tok;
}

bool Lexer::is_roman(std::string &str)
{
    /* Main part */
    /* Rule 1. The roman digits I and X
     * are repeated upto three times in succession to form the numbers.
     */
    if (str.find("XXXX") != std::string::npos || str.find("IIII") != std::string::npos) {
        return false;
    }

    /* The digits V, L and D are not repeated.
     * The repetition of V, L and D is invalid in the formation of numbers.
     */
    if (std::strchr(str.c_str(), 'V') != std::strrchr(str.c_str(), 'V')) {
        return false;
    }

    /* Rule 3. V is never written to the left of X. */
    if (str.find("IXX") != std::string::npos || str.find("VX") != std::string::npos) {
        return false;
    }

    /* Returning value */
    return true;
}
