#include "abstract_text.h"
#include <fstream>
#include <iostream>

using namespace std;

#ifndef H_HEADER
#define H_HEADER

class Header : public Abstract_text 
{
        int depth;
    public:
        Header () : depth (0) {}
        Header (int i, std::fstream &in);
        unsigned long int count_symbols () const;
        unsigned long int count_words () const;
        void print () const;
};

#endif
