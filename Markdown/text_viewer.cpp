#include "text_viewer.h"
#include "abstract_list.h"
#include "num_list.h"
#include "mark_list.h"
#include "abstract_text.h"
#include "abstract.h"
#include "header.h"
#include "config.h"
#include <cstring>
#include <typeinfo>
#include <iostream>

using namespace std;

Text_viewer :: Text_viewer (fstream & in)
{
    Abstract_text * txt;
    
    if (!in.eof ())
    {
        char c = in.get ();
        while (c == '\n') c = in.get ();
        in.seekg (-1, in.cur);
    }
    
    while ((txt = get_abstract (in)) != NULL) {
        text.append (txt);
    }
}

Abstract_text * Text_viewer :: get_abstract (fstream & in)
{
    char c;
    int depth = 0;    
    int counter = 0;
    
    Abstract_text * new_abstract;
    
    if (in.eof ()) 
    {
        return NULL;
    }
    
    c = in.get ();
    
    if (in.eof ()) 
    {
        return NULL;
    }
    
    if (c == ' ')
    {
        depth++;
        while (c == ' ') 
        {
            c = in.get();
            depth++;
        }
        
        depth /= 2;
        in.seekg (-1, in.cur);
    }

    if (c == '#') // header
    {
        while (c == '#') 
        {
            c = in.get ();
            depth++;
        }
        
        Header * new_header = new Header (depth, in);
        new_abstract = new_header;
        return new_abstract;
    }
    
    else if (c - '0' >= 0 && c - '9' <= 0) // num list
    {
        counter++;
        while (c - '0' >= 0 && c - '9' <= 0)
        {
            c = in.get ();
            counter++;  
        }
        
        if (c == '.')
        {
            c = in.get ();
            Num_list * new_num_list = new Num_list (depth, in);
            new_abstract = new_num_list;
            return new_abstract; 
        }
        else in.seekg (-counter, in.cur);
        Abstract * new_abs = new Abstract (in);
        new_abstract = new_abs;
        return new_abstract;   
    }
    
    else if (c == '*') // list
    {
        c = in.get ();
        Mark_list * new_mark_list = new Mark_list (depth, in);
        new_abstract = new_mark_list;
        return new_abstract;
    }
    
    // just abstract
    Abstract * new_abs = new Abstract (in);
    new_abstract = new_abs;
    return new_abstract;            
}

void Text_viewer :: print ()
{
    text.reset ();
    text.reset_num ();
    Abstract_text * tmp = text.get ();
    
    for (int i = 0; i < text.length; i++)
    {
        tmp -> print ();
        tmp = text.get ();
    }
}

unsigned long int Text_viewer :: count_symbols () const
{
    unsigned long int n_symbols = 0;
    abstract_list :: node * tmp = text.head;
    
    for (int i = 0; i < text.length; i++)
    {
        n_symbols += tmp -> elem -> count_symbols ();
        tmp = tmp -> next;        
    }
    
    return n_symbols;
}

unsigned long int Text_viewer :: count_words () const
{
    unsigned long int n_words = 0;
    abstract_list :: node * tmp = text.head;
    
    for (int i = 0; i < text.length; i++)
    {
        n_words += tmp -> elem -> count_words ();
        tmp = tmp -> next;        
    }
    
    return n_words;
} 


