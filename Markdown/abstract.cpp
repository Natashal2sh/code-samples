#include "abstract.h"
#include <stdio.h>
#include <iostream>
#include "config.h"
#include <cstring>
using namespace std;

#define N 512

Abstract :: Abstract (std :: fstream & file)
{
    char * line;
    bool abstract_end = false;
    
    file.seekg(-1, file.cur);
    
    while (!abstract_end)
    {
        line = read_line (file, abstract_end);
        line = delete_spaces (line);
        abstract.append (line);
    }
}

unsigned long int Abstract :: count_symbols () const
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

unsigned long int Abstract :: count_words () const
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

void Abstract :: print () const
{
    line_list :: common_node * tmp = dynamic_cast <line_list :: common_node *> (abstract.head);
    
    char * big_line = new char [N * abstract.length];
    big_line[0] = '\0';
    
    
    
    strcpy (big_line, tmp -> line);
    
    for (int i = 1; i < abstract.length; i++)
    {
        int len = strlen (big_line);
        tmp = dynamic_cast <line_list :: common_node *> (tmp -> next);
        big_line[len] = ' ';
        big_line[len + 1] = '\0';
        strcat (big_line, tmp -> line);
    }
    
    int position = 0;
    int counter = 0;
    
    while (position >= 0)
    {
        int prev_pos = position;
        counter++;
        int num = 0;
        
        if (counter == 1)
        {
            num = symbols (big_line, position, Configuration :: red_line / 2 + 1);
            for (int i = 0; i < Configuration :: red_line; i++) cout << ' ';             
        } 
        else num = symbols (big_line, position, 0); 
        for (int i = 0; i < num; i++) cout << big_line[prev_pos + i];
        cout << endl;
              
    }
    
    cout << endl;
    delete [] big_line;    
}  

