#ifndef INTER
#define INTER

#include <string>
#include <iostream>
#include <exception>

#include "Tokens.hpp"
#include "Lexer.hpp"

static std::ofstream *file = nullptr;

class Node {
public:
    int lexline = 0;
    static int labels;

    Node()
    {
        if (file == nullptr) {
            file = new std::ofstream;
            file->open("output.txt", std::ios_base::out);
            if (!file->is_open()) {
                file->open("/dev/stdout", std::ios_base::app);
                if (!file->is_open()) {
                    throw std::range_error("cannot open file");
                }
            }
        }
        lexline = Lexer::prev_line;
    }

    void error(const std::string &s) const
    {
        throw std::logic_error("near line " + std::to_string(lexline) + ": " + s);
    }

    static int newLabel()
    {
        return ++labels;
    }

    static void emitlabel(int i)
    {
        *file << "L" << std::to_string(i) << ":";
    }

    static void emit(const std::string &s)
    {
        *file << "\t" << s << std::endl;
    }

    virtual std::string toString()
    { return ""; }
};


class Expr : public Node {
public:
    Token *op{};
    Type *type{};

    Expr() = default;;

    Expr(Token *tok, Type *p)
    {
        op = tok;
        type = p;
    }

    virtual Expr *gen()
    { return this; }

    virtual Expr *reduce()
    { return this; }

    virtual void jumping(int t, int f)
    {
        emitjumps(toString(), t, f);
    }

    static void emitjumps(const std::string &test, int t, int f)
    {
        if (t != 0 && f != 0) {
            emit("if " + test + " goto L" + std::to_string(t));
            emit("goto L" + std::to_string(f));
        } else if (t != 0)
            emit("if " + test + " goto L" + std::to_string(t));
        else if (f != 0)
            emit("iffalse " + test + " goto L" + std::to_string(f));
        else; // nothing since both t and f fall through
    }

    std::string toString() override
    {
        return op->toString();
    }
};

class Temp : public Expr {
public:
    static int count;
    int number = 0;

    explicit Temp(Type *p) : Expr(&Word::_temp, p)
    {
        number = ++count;
    }

    std::string toString() override
    {
        return "t" + std::to_string(number);
    }
};

class Id : public Expr {
public:
    int offset{}; // relative address
    Id() = default;

    Id(Word *id, Type *p, int b) : Expr(id, p)
    {
        offset = b;
    }
};

class Op : public Expr {
public:
    Op(Token *tok, Type *p) : Expr(tok, p)
    {}

    Expr *reduce() override
    {
        Expr *x = gen();
        Temp *t = new Temp(type);
        emit(t->toString() + " = " + x->toString());
        return t;
    }
};

class Arith : public Op {
public:
    Expr *expr1, *expr2;

    Arith(Token *tok, Expr *x1, Expr *x2) : Op(tok, nullptr)
    {
        expr1 = x1;
        expr2 = x2;
        type = Type::max(expr1->type, expr2->type);
        if (type == nullptr)
            error("type error");
    }

    Expr *gen() override
    {
        return new Arith(op, expr1->reduce(), expr2->reduce());
    }

    std::string toString() override
    {
        return expr1->toString() + " " + op->toString() + " " + expr2->toString();
    }
};

class Unary : public Op {
public:
    Expr *expr;

    Unary(Token *tok, Expr *x) : Op(tok, nullptr)
    {
        expr = x;
        type = Type::max(Type::_int, expr->type);
        if (type == nullptr) error("type error");
    }

    Expr *gen() override
    {
        return new Unary(op, expr->reduce());
    }

    std::string toString() override
    {
        return op->toString() + " " + expr->toString();
    }
};

class Constant : public Expr {
public:
    Constant(Token *tok, Type *p) : Expr(tok, p)
    {}

    explicit Constant(int i) : Expr(new Num(i), Type::_int)
    {}

    void jumping(int t, int f) override
    {
        if (this == _true && t != 0)
            emit("goto L" + std::to_string(t));
        else if (this == _false && f != 0)
            emit("goto L" + std::to_string(f));
    }

    static Constant *_true, *_false;
};


class Logical : public Expr {
public:
    Expr *expr1{}, *expr2{};

    explicit Logical(Token *tok) : Expr(tok, nullptr)
    {};

    Logical(Token *tok, Expr *x1, Expr *x2) : Expr(tok, nullptr)
    {
        expr1 = x1;
        expr2 = x2;
        type = check(expr1->type, expr2->type);
        if (type == nullptr)
            error("type error");
    }

    static Type *check(Type *p1, Type *p2)
    {
        if (p1 == Type::_bool && p2 == Type::_bool)
            return Type::_bool;
        else return nullptr;
    }

    Expr *gen() override
    {
        int f = newLabel();
        int a = newLabel();
        Temp *temp = new Temp(type);
        jumping(0, f);
        emit(temp->toString() + " = true");
        emit("goto L" + std::to_string(a));
        emitlabel(f);
        emit(temp->toString() + " = false");
        emitlabel(a);
        return temp;
    }

    std::string toString() override
    {
        return expr1->toString() + " " + op->toString() + " " + expr2->toString();
    }
};

class Or : public Logical {
public:
    Or(Token *tok, Expr *x1, Expr *x2) : Logical(tok, x1, x2)
    {}

    void jumping(int t, int f) override
    {
        int label = t != 0 ? t : newLabel();
        expr1->jumping(label, 0);
        expr2->jumping(t, f);
        if (t == 0)
            emitlabel(label);
    }
};

class And : public Logical {
public:
    And(Token *tok, Expr *x1, Expr *x2) : Logical(tok, x1, x2)
    {}

    void jumping(int t, int f) override
    {
        int label = f != 0 ? f : newLabel();
        expr1->jumping(0, label);
        expr2->jumping(t, f);
        if (f == 0)
            emitlabel(label);
    }
};

class Not : public Logical {
public:
    Not(Token *tok, Expr *x2) : Logical(tok, x2, x2)
    {}

    void jumping(int t, int f) override
    {
        expr2->jumping(f, t);
    }

    std::string toString() override
    {
        return op->toString() + " " + expr2->toString();
    }
};

class Rel : public Logical {
public:
    Rel(Token *tok, Expr *x1, Expr *x2) : Logical(tok)
    {
        expr1 = x1;
        expr2 = x2;
        type = check(expr1->type, expr2->type);
        if (type == nullptr)
            error("type error");
    }

    static Type *check(Type *p1, Type *p2)
    {
        // c++ does not support "instanseof" operation
        // the following is not the most optimal way
        // but it is clean and doesnt require more changes in the original code
        if (p1->lexeme == "[]" || p1->lexeme == "[]")
            return nullptr;
        else if (p1 == p2)
            return Type::_bool;
        else return nullptr;
    }

    void jumping(int t, int f) override
    {
        Expr *a = expr1->reduce();
        Expr *b = expr2->reduce();
        std::string test = a->toString() + " " + op->toString() + " " + b->toString();
        emitjumps(test, t, f);
    }
};


class Access : public Op {
public:
    Id *array;
    Expr *index;


    Access(Id *a, Expr *i, Type *p) : Op(new Word("[]", Tag::INDEX), p)
    {
        array = a;
        index = i;
    }

    Expr *gen() override
    {
        return new Access(array, index->reduce(), type);
    }

    void jumping(int t, int f) override
    {
        emitjumps(reduce()->toString(), t, f);
    }

    std::string toString() override
    {
        return array->toString() + " [ " + index->toString() + " ]";
    }
};


class Stmt : public Node {
public:
    Stmt() = default;

    static Stmt *_null;
    static Stmt *enclosing;

    virtual void gen(int b, int a)
    {} // called with labels begin and after
    int after = 0;            // saves label after

};

class If : public Stmt {
public:
    Expr *expr;
    Stmt *stmt;

    If(Expr *x, Stmt *s)
    {
        expr = x;
        stmt = s;
        if (expr->type != Type::_bool)
            expr->error("boolean required in if");
    }

    void gen(int b, int a) override
    {
        int label = newLabel(); // label for the code for stmt
        expr->jumping(0, a);    // fall through on true, goto a on false
        emitlabel(label);
        stmt->gen(label, a);
    }
};

class Else : public Stmt {
public:
    Expr *expr;
    Stmt *stmt1, *stmt2;

    Else(Expr *x, Stmt *s1, Stmt *s2)
    {
        expr = x;
        stmt1 = s1;
        stmt2 = s2;
        if (expr->type != Type::_bool)
            expr->error("boolean required in if");
    }

    void gen(int b, int a) override
    {
        int label1 = newLabel();  // label1 for stmt1
        int label2 = newLabel();  // label2 for stmt2
        expr->jumping(0, label2); // fall through to stmt1 on true
        emitlabel(label1);
        stmt1->gen(label1, a);
        emit("goto L" + std::to_string(a));
        emitlabel(label2);
        stmt2->gen(label2, a);
    }
};

class While : public Stmt {
public:
    Expr *expr;
    Stmt *stmt;

    While()
    {
        expr = nullptr;
        stmt = nullptr;
    }

    void init(Expr *x, Stmt *s)
    {
        expr = x;
        stmt = s;
        if (expr->type != Type::_bool)
            expr->error("boolean required in while");
    }

    void gen(int b, int a) override
    {
        after = a;               // save label a
        expr->jumping(0, a);
        int label = newLabel();  // label for stmt
        emitlabel(label);
        stmt->gen(label, b);
        emit("goto L" + std::to_string(b));
    }
};

class Do : public Stmt {
public:
    Expr *expr;
    Stmt *stmt;

    Do()
    {
        expr = nullptr;
        stmt = nullptr;
    }

    void init(Stmt *s, Expr *x)
    {
        expr = x;
        stmt = s;
        if (expr->type != Type::_bool)
            expr->error("boolean required in do");
    }

    void gen(int b, int a) override
    {
        after = a;
        int label = newLabel();  // label for expr
        stmt->gen(b, label);
        emitlabel(label);
        expr->jumping(b, 0);
    }
};


class Set : public Stmt {
public:
    Id *id;
    Expr *expr;

    Set(Id *i, Expr *x)
    {
        id = i;
        expr = x;
        if (check(id->type, expr->type) == nullptr)
            error("type error");
    }

    static Type *check(Type *p1, Type *p2)
    {
        if (Type::numeric(p1) && Type::numeric(p2))
            return p2;
        else if (p1 == Type::_bool && p2 == Type::_bool)
            return p2;
        else return nullptr;
    }

    void gen(int b, int a) override
    {
        emit(id->toString() + " = " + expr->gen()->toString());
    }
};


class SetElem : public Stmt {
public:
    Id *array;
    Expr *index, *expr;

    SetElem(Access *x, Expr *y)
    {
        array = x->array;
        index = x->index;
        expr = y;
        if (check(x->type, expr->type) == nullptr)
            error("type error");
    }

    static Type *check(Type *p1, Type *p2)
    {
        if (p1->lexeme == "[]" || p2->lexeme == "[]")
            return nullptr;
        else if (p1 == p2)
            return p2;
        else if (Type::numeric(p1) && Type::numeric(p2))
            return p2;
        else return nullptr;
    }

    void gen(int b, int a) override
    {
        std::string s1 = index->reduce()->toString(),
                s2 = expr->reduce()->toString();
        emit(array->toString() + " [" + s1 + " ] = " + s2);
    }
};

class Seq : public Stmt {
public:
    Stmt *stmt1, *stmt2;

    Seq(Stmt *s1, Stmt *s2)
    {
        stmt1 = s1;
        stmt2 = s2;
    }

    void gen(int b, int a) override
    {
        if (stmt1 == Stmt::_null)
            stmt2->gen(b, a);
        else if (stmt2 == Stmt::_null)
            stmt1->gen(b, a);
        else {
            int label = newLabel();
            stmt1->gen(b, label);
            emitlabel(label);
            stmt2->gen(label, a);
        }
    }
};

class Break : public Stmt {
public:
    Stmt *stmt;

    Break()
    {
        stmt = Stmt::enclosing;
    }

    void gen(int b, int a) override
    {
        emit("goto L" + std::to_string(stmt->after));
    }
};

#endif