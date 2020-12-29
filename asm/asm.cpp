#include <iostream>
#include <vector>
#include <sstream>
#include <string>

static std::ofstream *outputs = nullptr;

class Generator {
public:
    std::ifstream *ifile;

    explicit Generator(std::ifstream *f)
    {
        ifile = f;
        if (outputs == nullptr) {
            outputs = new(std::ofstream);
            outputs->open("raw.txt", std::ios_base::out);
            if (!outputs->is_open()) {
                throw std::range_error("cannot open file raw.txt");
            }
        }
    }

    void go() const
    {
        std::string line;
        bool endofline;
        while (std::getline(*ifile, line)) {
            endofline = false;
            std::istringstream iss(line);
            std::string token;
            iss >> token;

            while (islabel(token)) {
                std::cout << token << "\n";
                *outputs << token << '\n';
                if (!(iss >> token)) {
                    endofline = true;
                    break;
                }
            }

            if (!endofline) {
                if (token == "if") {
                    _if(iss);
                } else if (token == "iffalse") {
                    _iffalse(iss);
                } else if (token == "goto") {
                    _goto(iss);
                } else {
                    threeAddress(token, iss);
                }
            }
        }
    }

    static void _if(std::istringstream &iss)
    {
        try {
            std::string left, op, right, _goto, label, jmp;

            if (left != "true")
                iss >> op >> right;
            iss >> _goto >> label;

            if (left == "true") jmp = "JMP"; // unconditional
            else if (op == ">=") jmp = "JGE";
            else if (op == "==") jmp = "JE";
            else if (op == "<=") jmp = "JLE";
            else if (op == "!=") jmp = "JNE";
            else if (op == ">") jmp = "JG";
            else if (op == "<") jmp = "JL";

            std::cout << '\t' << "CMP " << left << ", " << right << "\n";
            *outputs << '\t' << "CMP " << left << ", " << right << "\n";
            std::cout << '\t' << jmp << " " << label << "\n";
            *outputs << '\t' << jmp << " " << label << "\n";

        } catch (std::exception &e) {}
    }

    static void _iffalse(std::istringstream &iss)
    {
        try {
            std::string left, op, right, _goto, label, jmp;
            iss >> left;

            if (left != "false")
                iss >> op >> right;
            iss >> _goto >> label;

            if (left == "false") jmp = "JMP"; // unconditional
            else if (op == ">=") jmp = "JL";
            else if (op == "==") jmp = "JNE";
            else if (op == "<=") jmp = "JG";
            else if (op == "!=") jmp = "JE";
            else if (op == ">") jmp = "JLE";
            else if (op == "<") jmp = "JGE";

            std::cout << '\t' << "CMP " << left << ", " << right << "\n";
            *outputs << '\t' << "CMP " << left << ", " << right << "\n";
            std::cout << '\t' << jmp << " " << label << "\n";
            *outputs << '\t' << jmp << " " << label << "\n";

        } catch (std::exception &e) {}
    }

    static void _goto(std::istringstream &iss)
    {
        try {
            std::string lbl;

            iss >> lbl;

            std::cout << '\t' << "JMP " << lbl << '\n';
            *outputs << '\t' << "JMP " << lbl << '\n';

        } catch (std::exception &e) {}
    }

    static void threeAddress(const std::string &left, std::istringstream &iss)
    {
        std::string asgn, first, op, second;
        iss >> asgn >> first >> op >> second;

        std::cout << '\t' << "LOAD " << first << "\n";
        *outputs << '\t' << "LOAD " << first << "\n";
        if (!op.empty()) {
            if (op == "*") {
                std::cout << '\t' << "MPY " << second << "\n";
                *outputs << '\t' << "MPY " << second << "\n";
            } else if (op == "/") {
                std::cout << '\t' << "DIV " << second << "\n";
                *outputs << '\t' << "DIV " << second << "\n";
            } else if (op == "+") {
                std::cout << '\t' << "ADD " << second << "\n";
                *outputs << '\t' << "ADD " << second << "\n";
            } else if (op == "-") {
                std::cout << '\t' << "SUB " << second << "\n";
                *outputs << '\t' << "SUB " << second << "\n";
            }
        }
        std::cout << '\t' << "STORE " << left << "\n";
        *outputs << '\t' << "STORE " << left << "\n";
    }

    static bool islabel(std::string s)
    {
        return s[s.length() - 1] == ':';
    }
};
