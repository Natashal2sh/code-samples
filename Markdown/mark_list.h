#ifndef H_MARK_LIST
#define H_MARK_LIST

#include "abstract_text.h"
#include <fstream>

class Mark_list : public Abstract_text 
{
        int depth;
    public:
        Mark_list (int i, std :: fstream & file);
        unsigned long int count_symbols () const;
        unsigned long int count_words () const;
        void print () const;
};

#endif

