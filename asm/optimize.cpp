#include <iostream>
#include <iterator>
#include <fstream>
#include <list>
#include <set>
#include <vector>
#include "Lexer.hpp"

static std::ofstream *outputs = nullptr;

class Optimizer {
public:
    std::vector<std::string> *lines;
    Lexer *lex;
    std::string result;

    explicit Optimizer(std::vector<std::string> *l)
    {
        if (outputs == nullptr) {
            outputs = new(std::ofstream);
            outputs->open("final.txt", std::ios_base::out);
            if (!outputs->is_open()) {
                throw std::range_error("cannot open file final.txt");
            }
        }
        lines = l;
        lex = new Lexer();
    }

    void removeUnused()
    {
        std::set<Word *> unused;
        std::map<std::string, Token *>::iterator it;
        Word *w;
        Token *tok;

        for (const std::string &line: *lines) {
            lex->nextline(line);
            while (!lex->iseof) {
                lex->scan();
            }
        }

        for (it = lex->words.begin(); it != lex->words.end(); it++) {
            if (it->second && it->second->tag == Tag::ID) {
                w = (Word *) it->second;
                if (w->use_count <= 1 && !isLabel(w))
                    unused.insert(w);
            }
        }

        if (!unused.empty()) {
            std::cout << "Unused vars removed: ";
            *outputs << "Unused vars removed: ";
            for (auto w1: unused) {
                std::cout << w1->toString() << " ";
                *outputs << w1->toString() << " ";
            }
            std::cout << "\n\n";
            *outputs << "\n\n";
        }

        std::vector<std::string>::iterator list_i;
        std::set<Word *>::iterator set_i;
        for (list_i = lines->begin(); list_i != lines->end(); ++list_i) {
            lex->nextline(*list_i);
            while (!lex->iseof) {
                tok = lex->scan();
                // check this token among unused variables
                // and delete line if found
                Word *tmp = (Word *) tok;
                set_i = unused.find(tmp);
                if (set_i != unused.end()) {
                    lines->erase(list_i);
                    --list_i;
                    lines->erase(list_i);
                    break;
                }
            }
        }

        // join list of strings
        std::ostringstream os;
        std::copy(lines->begin(), lines->end() - 1,
                  std::ostream_iterator<std::string>(os, "\n"));
        os << *lines->rbegin();
        result = os.str();
    }

    static bool isLabel(Word *w)
    {
        std::string s = w->toString();
        if (s.length() > 0) return (s[0] == 'L');
        return false;
    }

    void removeTemps()
    {

    }

    void run()
    {
        removeUnused();
        removeTemps();
    }

};

void readFile(const std::string &filename, std::vector<std::string> *lines)
{
    lines->clear();
    std::ifstream file(filename);
    std::string s;

    while (std::getline(file, s)) {
        lines->push_back(s);
    }
}

int main()
{
    /* Main part */
    try {
        auto lines = new std::vector<std::string>();
        readFile("raw.txt", lines);

        auto *optmzr = new Optimizer(lines);
        optmzr->run();

        std::cout << optmzr->result << '\n';
        *outputs << optmzr->result << '\n';
    }
    catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
    outputs->close();

    /* Returning value */
    return 0;
}
