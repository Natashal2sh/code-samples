#include "num_list.h"
#include "mark_list.h"
#include "config.h"
#include <iostream>
#include <cstring>

#define N 512

using namespace std;

Num_list :: Num_list (int i, std::fstream &file)
{
    bool abstract_end = false;    
    char * line;
    char c = 0;
    depth = i;
    
    line = read_line (file, abstract_end);
    line = delete_spaces (line);
    abstract.append (line);    
        
    while (true)
    {
        if (file.eof()) break;
        
        bool ret = false;
        
        if (abstract_end) 
        {
            break;
        }
              
        for (int j = 0; j < depth * 2; j++) 
        {
            c = file.get ();
            if (c != ' ') 
            {
                ret = true;
            }
        }
        
        if (ret)
        {
            file.seekg (-depth * 2, file.cur);
            break;
        }
        
        c = file.get ();
        
        if (c == ' ')
        {
            c = file.get ();
            
            if (c != ' ') throw "Wrong indent";
            
            Abstract_text * new_abstract;
            
            c = file.get ();
            
            if (c == '*')
            {
                c = file.get ();
                
                new_abstract = new Mark_list (depth + 1, file);
                abstract.append (new_abstract);
            }            
            else if (c - '0' >= 0 && c - '9' <= 0)
            {
                while (c - '0' >= 0 && c - '9' <= 0) c = file.get ();
                
                c = file.get ();
                
                new_abstract = new Num_list (depth + 1, file);
                abstract.append (new_abstract);
            }
            else throw "Wrong data";           
        }        
        else 
        {            
            while (c - '0' >= 0 && c - '9' <= 0) c = file.get ();
            c = file.get ();
            
            if (c == '\n' || file.eof ())
            {
                cout << "here" << endl;
                char * line1 = new char [N];
                line1[0] = ' ';
                line1[1] = '\0';
                abstract.append (line1);
            }
            else 
            {
                line = read_line (file, abstract_end);
                line = delete_spaces (line);
                abstract.append (line); 
            }           
            
            
        }
    }
}

unsigned long int Num_list :: count_symbols () const
{
    unsigned long int n_symbols = 0;
    line_list :: abstract_node * tmp = abstract.head;
    
    for (int i = 0; i < abstract.length; i++) 
    {
        n_symbols += tmp -> length ();
        tmp = tmp -> next;
    }
    
    return n_symbols;
}

unsigned long int Num_list :: count_words () const
{
    unsigned long int n_words = 0;
    line_list :: abstract_node * tmp = abstract.head;
    
    for (int j = 0; j < abstract.length; j++)
    {
        n_words += tmp -> count_words ();        
        tmp = tmp -> next;
    }
    
    return n_words;
}

void Num_list :: print () const
{
    line_list :: abstract_node * tmp = abstract.head;
 
    int i = 0;    
    while (tmp != NULL) 
    {
        tmp -> print (depth, i);        
        tmp = tmp -> next;
    }
    
    if (depth == 0) cout << endl;
    
}  
