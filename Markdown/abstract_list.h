#include "abstract_text.h"
#include <iostream>
using namespace std;
#ifndef H_ABSTRACT_LIST
#define H_ABSTRACT_LIST

struct abstract_list
{
    int length;
    int current_num;
    struct node 
    {
        Abstract_text * elem;
        node * next; 
        ~node ();  
    };
    node * head;
    node * current;
    
    void reset_num () { current_num = 0; }
    void reset () { current = head; }
    Abstract_text * get () { if (length != 0 && current != NULL) { node * c = current; current = current -> next; return c -> elem; } else return NULL; }
    void append (Abstract_text * elem);
    abstract_list (const abstract_list & other);
    abstract_list () : length (0) {}
    ~abstract_list ();    
};

#endif
