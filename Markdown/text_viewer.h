#include <cstdio>
#include <fstream>

#include "abstract_list.h"
#include "abstract_text.h"

#ifndef H_TEXT_VIEWER
#define H_TEXT_VIEWER

class Text_viewer 
{
        abstract_list text;
    public:
        Text_viewer (std :: fstream & in);
        Abstract_text * get_abstract (std :: fstream & in);
        void print ();
        unsigned long int count_symbols () const;
        unsigned long int count_words () const;        
};

#endif
