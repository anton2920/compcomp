#include "Lexer.hpp"

int Lexer::line = 1;

Lexer::Lexer() {
    reserve( new Word("LOAD",  Tag::LOAD));
    reserve( new Word("STORE", Tag::STORE));
    reserve( new Word("GOTO",  Tag::GOTO));
    reserve( new Word("CMP",   Tag::CMP));
    reserve( new Word("JE",    Tag::JE));
    reserve( new Word("JLE",   Tag::JLE));
    reserve( new Word("JGE",   Tag::JGE));
    reserve( new Word("JMP",   Tag::JMP));
    reserve( new Word("JNE",   Tag::JNE));
    reserve( new Word("JG",    Tag::JG));
    reserve( new Word("MPY",   Tag::MPY));
    reserve( new Word("DIV",   Tag::DIV));
    reserve( new Word("SUB",   Tag::SUB));
    reserve( new Word("ADD",   Tag::ADD));

    reserve( &Word::_true );  reserve( &Word::_false );
}

void Lexer::nextline (std::string line) {
    input = new std::stringstream(line);
    peek = ' ';
    iseof = false;
}

void Lexer::reserve (Word *w) { 
    words.insert({ w->lexeme, w });
}

void Lexer::readch() { 
    int temp;
    if ((temp = input->get()) == -1)
        iseof = true;
    else peek = temp;
} 

bool Lexer::readch(char c) {
    readch();
    if (peek != c) return false;
    peek = ' ';
    return true;
}

Token* Lexer::scan() {
    for ( ; !iseof; readch() ) {
        if (peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') line = line + 1;
        else break;
    }

    
    if (isdigit(peek)) {
        int v = 0;
        do {
            v = 10*v + (peek - '0');
            readch();
        } while ( isdigit(peek) && !iseof);
        if (peek != '.') {
            Token *n;
            if (n = words[std::to_string(v)])
                return n;
            n = new Num(v);
            words.insert({std::to_string(v), n});
            return n;
        }
        float x = v;
        float d = 10;
        for (;;) {
            readch();
            if (!isdigit(peek) || iseof) break;
            x = x + (peek - '0') / d; 
            d = d * 10;
        }
        Token *r;
        if (r = words[std::to_string(x)])
            return r;
        r = new Real(x);
        words.insert({std::to_string(x), r});
        return r;
    }
    if (isalpha(peek)) {
        std::string s;
        do {
            s += peek; readch();
        } while ( isalnum(peek) && !iseof );
        Token *w;
        if (w = words[s]) {
            Word *tmp = (Word*)w;
            tmp->use_count++;
            return w;
        }
        words[s] = new Word(s, Tag::ID);
        return w;
    }
    // do i need this?
    if (iseof)
        return new Token((Tag)0);

    Token *tok = new Token((Tag)peek); 
    peek = ' ';
    return tok;
}

