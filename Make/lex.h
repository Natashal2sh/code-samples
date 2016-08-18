#ifndef H_LEX
#define H_LEX

#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

#define N 512

void parser (const char * filename);

enum type_of_lex
{
    LEX_VAR,
    LEX_VAR_VAL,
    LEX_WORD_PERCENT,
    LEX_WORD,
    LEX_NEW_LINE,
    LEX_TAB,
    LEX_COLON,
    LEX_EQ,
    LEX_DOLLAR,
    LEX_RIGHT_BRACKET,
    LEX_LEFT_BRACKET,
    LEX_LEFT_RULE,
    LEX_RIGHT_RULE,
    LEX_PHONY,
    LEX_END
};

class lexeme  
{
        type_of_lex lex_type;
        char * lex_val;
        int str;
    public:
        lexeme (type_of_lex a, const char * b, int line_counter = 0) : lex_type (a), str (line_counter) { lex_val = new char [N]; strcpy (lex_val, b); }
        ~lexeme () { delete [] lex_val; }
        void print () { cout << lex_val; }
        type_of_lex get_type () { return lex_type; }
        char * get_val () { return lex_val; }
        int get_line () { return str; }
        void set_type (const type_of_lex type) { lex_type = type; }
};

enum states 
{
    H, 
    WORD,
    DOLLAR,
    WORD_PERCENT,
    ERR,
    TAB,
    FILENAME,
    SPACE,
    END
};



#endif
