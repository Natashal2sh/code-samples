#ifndef H_ABSTRACT
#define H_ABSTRACT

#include "abstract_text.h"
#include <fstream>

class Abstract : public Abstract_text 
{
    public:
        Abstract (std :: fstream & file);
        unsigned long int count_symbols () const;
        unsigned long int count_words () const;
        void print () const;
};

#endif
