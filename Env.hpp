#ifndef ENV
#define ENV

#include <unordered_map>

#include "Inter.hpp"
#include "Tokens.hpp"


class Env {
private:
    std::unordered_map<std::string, Id*> *table;
protected: 
    Env *prev;
public:
    explicit Env(Env *n) {
        table = new std::unordered_map<std::string, Id*>();
        prev = n;
    }
    void put(Token *w, Id *i) {
        table->insert({ ((Word*)w)->lexeme, i });
    }
    Id * get(Token *w) {
        std::string word = ((Word*)w)->lexeme;
        for (Env *e = this; e != nullptr; e = e->prev)
        {
            if (e->table->find(word) != e->table->end())
                return e->table->at(word);
        }
        return nullptr;
    }

};

#endif
