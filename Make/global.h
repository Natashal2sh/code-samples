#ifndef H_GLOBAL
#define H_GLOBAL

#include <vector>
#include "lex.h"
#include "synt.h"


using namespace std;

struct Global
{
    static vector <Target *> targets;
    static vector <lexeme *> mfile;
    static map <char *, char *> variables;
    static string type_of_lex_str [15];
};

#endif
