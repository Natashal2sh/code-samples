#include "header.h"
#include "word_list.h"
#include "config.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdio>

#define N 256

using namespace std;

Header :: Header (int i, fstream & in) 
{
    bool abstract_end = false;
    char * line = NULL;
    
    depth = i;
    abstract.length = 0;
    
    line = read_line (in, abstract_end);    
    line = delete_spaces (line);
    abstract.append (line);
    if (!abstract_end) throw "Wrong data";            
}


unsigned long int Header :: count_symbols () const
{
    return abstract.head -> length ();   
}

unsigned long int Header :: count_words () const 
{        
    return abstract.head -> count_words ();
}

void Header :: print () const 
{
    int * to_print = new int [N];
    int position = 0;
    int counter = 0;
    int max = 0;
    line_list :: common_node * tmp = dynamic_cast <line_list :: common_node *> (abstract.head);
    
    while (position >= 0)
    {
        int num = symbols (tmp -> line, position, depth);
        to_print[counter++] = num;
        if (num > max) max = num;           
    }
    
    int spaces = (Configuration :: width - max - depth * 2) / 2;
    for (int i = 0; i < spaces; i++) cout << ' ';
    for (int i = 0; i < max + depth * 2; i++) cout << '#';
    for (int i = 0; i < spaces; i++) cout << ' ';
    cout << endl;
    
    int cur_pos = 0;
    for (int i = 0; i < counter; i++)
    {
        for (int j = 0; j < spaces; j++) cout << ' ';
        for (int j = 0; j < depth; j++) cout << '#';
        for (int j = 0; j < (max - to_print[i]) / 2; j++) cout << ' ';
        for (int j = cur_pos; j < cur_pos + to_print[i]; j++) cout << (tmp -> line)[j];
        for (int j = 0; j < (max - to_print[i]) / 2; j++) cout << ' ';
        if ((max - to_print[i]) % 2 != 0) cout << ' ';
        for (int j = 0; j < depth; j++) cout << '#';
        for (int j = 0; j < spaces; j++) cout << ' ';
        cur_pos += to_print[i];
        cout << endl;
    }
    
    for (int i = 0; i < spaces; i++) cout << ' ';
    for (int i = 0; i < max + depth * 2; i++) cout << '#';
    for (int i = 0; i < spaces; i++) cout << ' ';
    cout << endl;
    cout << endl;
    delete [] to_print;    
} 
