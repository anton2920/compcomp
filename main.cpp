#include <iostream>

#include "Lexer.hpp"
#include "Parser.hpp"

#include "asm/asm.cpp"

int main()
{
    /* Stage #1: Lex, Parse and Gen */
    try {
        std::ifstream input("data.txt");
        auto *lex = new Lexer(input);
        auto *parse = new Parser(lex);
        parse->program();
    }
    catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
    file->close();
    delete(file);

    /* Stage #2 (almost) TRUE Asm */
    try {
        std::ifstream threecode("output.txt");
        auto *gen = new Generator(&threecode);
        gen->go();
    } catch (std::exception &e) {
        std::cout << e.what() << '\n';
    }
    outputs->close();
    std::cout << std::endl;

    /* Stage #3 Efficiency to go! */
    #ifdef _WIN32
        if (system("optimizer.exe")) {
            std::cerr << "Optimizer returned with non-zero exit code" << std::endl;
        }
    #else
        if (system("./optimizer")) {
            std::cerr << "Optimizer returned with non-zero exit code" << std::endl;
        }
    #endif

    /* Returning value */
    return 0;
}
