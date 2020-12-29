#ifndef PARSER
#define PARSER

#include "Lexer.hpp"
#include "Inter.hpp"
#include "Env.hpp"

class Parser {
private:
    Lexer *lex;         // lexical analyzer for this parser
    Token *look{};      // lookahead tagen
    Env *top = nullptr; // current or top symbol table 
    int used = 0;       // storage used for declarations
public:
    Parser(Lexer *l)
    {
        lex = l;
        move();
    }

    void move()
    {
        look = lex->scan();
    }

    static void error(const std::string &s)
    {
        throw std::logic_error("near line " + std::to_string(Lexer::line) + ": " + s);
    }

    void match(int t)
    {
        if ((int) look->tag == t) {
            move();
        } else {
            error("syntax error");
        }
    }

    void match(Tag t)
    {
        if (look->tag == t) {
            move();
        } else {
            error("syntax error");
        }
    }

    void program()
    {
        Stmt *s = block();
        int begin = Stmt::newLabel();
        int after = Stmt::newLabel();
        Stmt::emitlabel(begin);
        s->gen(begin, after);
        Stmt::emitlabel(after);
    }

    Stmt *block();
    void decls();
    Type *type();
    Type *dims(Type *p);
    Stmt *stmts();
    Stmt *stmt();
    Stmt *assign();
    Expr *boolean();
    Expr *join();
    Expr *equality();
    Expr *rel();
    Expr *expr();
    Expr *term();
    Expr *unary();
    Expr *factor();
    Access *offset(Id *a);
};

Stmt *Parser::block()
{
    match('{');
    Env *savedEnv = top;
    top = new Env(top);
    decls();
    Stmt *s = stmts();
    match('}');
    top = savedEnv;
    return s;
}

void Parser::decls()
{
    while (look->tag == Tag::BASIC) {   // D -> type ID ; 
        Type *p = type();
        Token *tok = look;
        match(Tag::ID);
        match(';');
        Id *id = new Id((Word *) tok, p, used);
        top->put(tok, id);
        used = used + p->width;
    }
}

Type *Parser::type()
{
    Type *p = (Type *) look;  // expect look.tag == Tag.BASIC
    match(Tag::BASIC);
    if ((int) look->tag != '[') return p;  // T -> basic
    else return dims(p);                 // return array type
}

Type *Parser::dims(Type *p)
{
    match('[');
    Token *tok = look;
    match(Tag::NUM);
    match(']');
    if ((int) look->tag == '[')
        p = dims(p);
    return new Array(((Num *) tok)->value, p);
}

Stmt *Parser::stmts()
{
    if ((int) look->tag == '}') {
        return Stmt::_null;
    }
    else {
        return new Seq(stmt(), stmts());
    }
}

Stmt *Parser::stmt()
{
    Expr *x;
    Stmt *s1, *s2;
    Stmt *savedStmt;      // save enclosing loop for breaks

    switch ((int) look->tag) {
        case ';': {
            move();
            return Stmt::_null;
        }
        case (int) Tag::IF: {
            match(Tag::IF);
            match('(');
            x = boolean();
            match(')');
            s1 = stmt();
            if (look->tag != Tag::ELSE)
                return new If(x, s1);
            match(Tag::ELSE);
            s2 = stmt();
            return new Else(x, s1, s2);
        }
        case (int) Tag::WHILE: {
            auto *whilenode = new While();
            savedStmt = Stmt::enclosing;
            Stmt::enclosing = whilenode;
            match(Tag::WHILE);
            match('(');
            x = boolean();
            match(')');
            s1 = stmt();
            whilenode->init(x, s1);
            Stmt::enclosing = savedStmt; // reset Stmt::enclosing
            return whilenode;
        }
        case (int) Tag::DO: {
            Do *donode = new Do();
            savedStmt = Stmt::enclosing;
            Stmt::enclosing = donode;
            match(Tag::DO);
            s1 = stmt();
            match(Tag::WHILE);
            match('(');
            x = boolean();
            match(')');
            match(';');
            donode->init(s1, x);
            Stmt::enclosing = savedStmt; // reset Stmt::enclosing
            return donode;
        }
        case (int) Tag::BREAK: {
            match(Tag::BREAK);
            match(';');
            return new Break();
        }
        case '{':
            return block();
        default:
            return assign();
    }
}

Stmt *Parser::assign()
{
    Stmt *stmt;
    Token *t = look;
    match(Tag::ID);
    Id *id = top->get(t);
    if (id == nullptr)
        error(t->toString() + " undeclared");
    if ((int) look->tag == '=') {                // S -> id = E ;
        move();
        stmt = new Set(id, boolean());
    } else {                                      // S -> L = E ;
        Access *x = offset(id);
        match('=');
        stmt = new SetElem(x, boolean());
    }
    match(';');
    return stmt;
}


Expr *Parser::boolean()
{
    Expr *x = join();
    while (look->tag == Tag::OR) {
        Token *tok = look;
        move();
        x = new Or(tok, x, join());
    }
    return x;
}

Expr *Parser::join()
{
    Expr *x = equality();
    while (look->tag == Tag::AND) {
        Token *tok = look;
        move();
        x = new And(tok, x, equality());
    }
    return x;
}

Expr *Parser::equality()
{
    Expr *x = rel();
    while (look->tag == Tag::EQ || look->tag == Tag::NE) {
        Token *tok = look;
        move();
        x = new Rel(tok, x, rel());
    }
    return x;
}


Expr *Parser::rel()
{
    Expr *x = expr();

    switch ((int) look->tag) {
        case '<':
        case (int) Tag::LE:
        case (int) Tag::GE:
        case '>': {
            Token *tok = look;
            move();
            return new Rel(tok, x, expr());
        }
        default:
            return x;
    }
}

Expr *Parser::expr()
{
    Expr *x = term();
    while ((int) look->tag == '+' || (int) look->tag == '-') {
        Token *tok = look;
        move();
        x = new Arith(tok, x, term());
    }
    return x;
}

Expr *Parser::term()
{
    Expr *x = unary();
    while ((int) look->tag == '*' || (int) look->tag == '/') {
        Token *tok = look;
        move();
        x = new Arith(tok, x, unary());
    }
    return x;
}

Expr *Parser::unary()
{
    if ((int) look->tag == '-') {
        move();
        return new Unary(&Word::_minus, unary());
    } else if ((int) look->tag == '!') {
        Token *tok = look;
        move();
        return new Unary(tok, unary());
    } else return factor();
}

Expr *Parser::factor()
{
    Expr *x;
    std::string s;
    switch ((int) look->tag) {
        case '(': {
            move();
            x = boolean();
            match(')');
            return x;
        }
        case (int) Tag::NUM: case (int) Tag::ROMAN:
            x = new Constant(look, Type::_int);
            move();
            return x;
        case (int) Tag::REAL:
            x = new Constant(look, Type::_float);
            move();
            return x;
        case (int) Tag::TRUE: {
            x = Constant::_true;
            move();
            return x;
        }
        case (int) Tag::FALSE:
            x = Constant::_false;
            move();
            return x;
        case (int) Tag::ID: {
            s = look->toString();
            Id *id = top->get(look);
            if (id == nullptr)
                error(s + " undeclared");
            move();
            if ((int) look->tag != '[') return id;
            else return offset(id);
        }
        default:
            throw std::logic_error("syntax error in factor()");
    }
}

Access *Parser::offset(Id *a)
{      // I -> [E] | [E] I
    Expr *i, *w, *t1, *t2, *loc;      // inherit id
    Type *type = a->type;
    match('[');
    i = boolean();
    match(']');   // first index, I -> [ E ]
    type = ((Array *) type)->of;
    w = new Constant(type->width);
    t1 = new Arith(new Token((Tag) (int) '*'), i, w);
    loc = t1;
    while ((int) look->tag == '[') {       // multi-dimensional I -> [ E ] I
        match('[');
        i = boolean();
        match(']');
        type = ((Array *) type)->of;
        w = new Constant(type->width);
        t1 = new Arith(new Token((Tag) (int) '*'), i, w);
        t2 = new Arith(new Token((Tag) (int) '+'), loc, t1);
        loc = t2;
    }
    return new Access(a, loc, type);
}

#endif // PARSER
