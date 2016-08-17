#ifndef H_ABSTRACT_TEXT
#define H_ABSTRACT_TEXT

#include "word_list.h"

class Abstract_text 
{
    public:
        line_list abstract;
        virtual unsigned long int count_symbols () const = 0;
        virtual unsigned long int count_words () const = 0;
        virtual void print () const = 0;
        virtual ~Abstract_text () {}
};

#endif
