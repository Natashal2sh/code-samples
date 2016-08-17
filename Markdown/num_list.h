#ifndef H_NUM_LIST
#define H_NUM_LIST

#include "abstract_text.h"
#include <fstream>

class Num_list : public Abstract_text 
{      
        int depth;
    public:
        Num_list (int i, std :: fstream & file);
        unsigned long int count_symbols () const;
        unsigned long int count_words () const;
        void print () const; 
};

#endif
