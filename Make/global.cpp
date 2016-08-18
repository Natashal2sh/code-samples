#include <vector>
#include <map>
#include "lex.h"
#include "synt.h"
#include "run.h"
#include "global.h"

using namespace std;

#define N 512

vector <Target *> Global :: targets;
vector <lexeme *> Global :: mfile;
map <char *, char *> Global :: variables;

string Global :: type_of_lex_str [15] 
{
    "VARIABLE",
    "VARIABLE_VALUE",
    "WORD_PERCENT",
    "WORD",
    "NEW_LINE",
    "TAB",
    "COLON",
    "EQ",
    "DOLLAR",
    "RIGHT_BRACKET",
    "LEFT_BRACKET",
    "LEFT_RULE",
    "RIGHT_RULE",
    "PHONY",
    "END"  
};
