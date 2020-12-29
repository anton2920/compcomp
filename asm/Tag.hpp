#ifndef TAG

enum class Tag {
    AND   = 256, ASGN  = 257, BASIC = 258, BREAK = 259, DO   = 260, 
    ELSE  = 261, EQ    = 262, FALSE = 263, GE    = 264, ID   = 265, 
    IF    = 266, INDEX = 267, LE    = 268, MINUS = 269, NE   = 270, 
    NUM   = 271, OR    = 272, REAL  = 273, TEMP  = 274, TRUE = 275, 
    THEN  = 276, WHILE = 277,

    LOAD    = 278, STORE   = 279,
    GOTO    = 280, CMP     = 281,
    JE      = 282, JLE     = 283,
    JGE     = 284, JMP     = 285,
    JNE     = 286, JG      = 287,
    MPY     = 288, DIV     = 289,
    SUB     = 290, ADD     = 291
};

#endif