#include "Tokens.hpp"

Word Word::_and   = Word("&&", Tag::AND); 
Word Word::_or    = Word("||", Tag::OR); 
Word Word::_eq    = Word("==", Tag::EQ); 
Word Word::_ne    = Word("!=", Tag::NE); 
Word Word::_le    = Word("<=", Tag::LE); 
Word Word::_ge    = Word(">=", Tag::GE); 
Word Word::_minus = Word("minus", Tag::MINUS); 
Word Word::_true  = Word("true", Tag::TRUE); 
Word Word::_false = Word("false", Tag::FALSE); 
Word Word::_temp  = Word("temp", Tag::TEMP); 


Type * Type::_int   = new Type("int",   Tag::BASIC, 4);
Type * Type::_float = new Type("float", Tag::BASIC, 8);
Type * Type::_char  = new Type("char",  Tag::BASIC, 1);
Type * Type::_bool  = new Type("bool",  Tag::BASIC, 1);
