#include "Inter.hpp"


Constant * Constant::_true  = new Constant(&Word::_true,  Type::_bool);
Constant * Constant::_false = new Constant(&Word::_false, Type::_bool);


Stmt* Stmt::_null = new Stmt();
Stmt* Stmt::enclosing = Stmt::_null;

int Temp::count = 0;

int Node::labels = 0;
