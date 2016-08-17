#include "word_list.h"
#include "config.h"
#include "abstract_text.h"
#include <cstring>
#include <iostream>
#include <typeinfo>

using namespace std;

#define N 512

void line_list :: common_node :: print (int depth) const 
{
    for (int j = 0; j < depth * 2; j++) cout << ' ';

    cout << Configuration :: marker;
             
    for (int j = 0; j < Configuration :: n_spaces - 1; j++) cout << ' ';
        
    int position = 0;
    int counter = 0;
    
    while (position >= 0)
    {
        int prev_pos = position;
        if (counter > 0) for (int j = 0; j < Configuration :: n_spaces + 2 * depth; j++) cout << ' ';
                
        int num = symbols (line_list :: common_node :: line, position, Configuration :: n_spaces / 2 + 1 + depth);
        counter++;

        for (int i = 0; i < num; i++) cout << (line_list :: common_node :: line)[prev_pos + i];
        cout << endl;                          
    }
}

void line_list :: list_node :: print (int depth) const 
{
    line_list :: list_node :: line -> print ();
}

void line_list :: common_node :: print (int depth, int & i) const
{
    for (int j = 0; j < depth * 2; j++) cout << ' ';
 
    cout << i + 1 << ".";        
    for (int j = 0; j < Configuration :: n_spaces - get_number_of_digits (i + 1) - 1; j++) cout << ' ';
        
    int position = 0;
    int counter = 0;
    
    while (position >= 0)
    {
        int prev_pos = position;
        if (counter > 0) for (int j = 0; j < Configuration :: n_spaces + 2 * depth; j++) cout << ' ';
                
        int num = symbols (line_list :: common_node :: line, position, Configuration :: n_spaces / 2 + 1 + depth);
        counter++;

        for (int i = 0; i < num; i++) cout << (line_list :: common_node :: line)[prev_pos + i];
        cout << endl;                          
    }
    
    i++;
}

void line_list :: list_node :: print (int depth, int & i) const 
{
    line_list :: list_node :: line -> print ();
}

unsigned long int line_list :: list_node :: count_words () const 
{
    return line -> count_words ();
}

unsigned long int line_list :: common_node :: count_words () const 
{
    unsigned long int n_words = 1;
    
    for (int i = 0; i < (int) strlen (line_list :: common_node :: line); i++)
    {
        if (line_list :: common_node :: line[i] == ' ') n_words++;
    }
    
    return n_words;
}

unsigned long int line_list :: list_node :: length () const 
{ 
    return line -> count_symbols (); 
}

int get_number_of_digits (int n)
{
    int result = 0;
    
    while (n > 0)
    {
        result++;
        n /= 10;
    }
    
    return result;
}

int symbols (char * line, int & position, int depth)
{
   int result = 0;
   int counter = 0;
    
   while (true) // words
   {
       int cur = 0;
       
       while (line[position++] != ' ' && line[position] != '\0') cur++;
       counter++;
       
       if (counter == 1)
       {
           if (cur > Configuration :: width - 2 * depth)
           {
               throw "Too long word";
           }
           else 
           {
               result += cur;
               if (line[position] == '\0')
               {
                   position = -1;
                   return result + 1;
               }
           }
           
       }
       else 
       {
           if (result + cur + 1 > Configuration :: width - 2 * depth) 
           {
               position -= (cur + 1);
               return result;
           }
           else 
           {
               result += (cur + 1);
               if (line[position] == '\0')
               {
                   position = -1;
                   return result + 1;
               }
           }
       }
   } 
}

char * read_line (std::fstream & in, bool & abstract_end)
{
    char * line = new char [N];
    int counter = 0;
    char c;
    
    if (in.eof())
    {
        return 0; 
    }
    
    c = in.get ();
    
    //if (c == '\n' || in.eof())
    //{
        
    //}
    
    while (c != '\n' && !in.eof())
    {
        line[counter++] = c;
        c = in.get ();
    }
    
    if (c == '\n')
    {
        c = in.get ();
        if (c == '\n') 
        {
            abstract_end = true;
            while (c == '\n') c = in.get ();
        }
    }
    
    if (in.eof())
    {
        abstract_end = true;
    }
    else 
    {
        in.seekg(-1, in.cur);
    }
    
    line[counter] = '\0';
    if (counter == 0) line[1] = ' ';
    //cout << "counter " << endl;
    
    return line; 
}

char * delete_spaces (char * line)
{
    bool space = false;
    char * new_line = new char [N];
    int counter = 0;
    int i = 0; 
    int len = strlen (line);
    
    while (line[i++] == ' ');
    --i;
    
    for (; i < len; i++)
    {
        if (line[i] != ' ' && line[i] != '\t') 
        {
            new_line[counter++] = line[i];
            space = false;
        }    
        else if ((line[i] == ' ' || line[i] == '\t')  && !space) 
        {
            new_line[counter++] = ' ';
            space = !space;
        }
    }
    
    if (counter > 0 && new_line[counter - 1] == ' ') counter--;
    
    if (counter == 0) 
    {
        new_line[0] = ' ';
        new_line[1] = '\0';
    }
    else 
    {
        new_line[counter] = '\0';
    }
    
    delete [] line;    
    return new_line;
}

void line_list :: append (char * line)
{
    if (length == 0)
    {
        common_node * tmp = new common_node;
        tmp -> next = NULL;
        tmp -> line = line;
        head = tmp;        
    }
    else 
    {
        abstract_node * tmp = head;
        for (int i = 0; i < length - 1; i++)
            tmp = tmp -> next;
        common_node * tmp2 = new common_node;
        tmp2 -> next = NULL;
        tmp2 -> line = line;
        tmp -> next = tmp2;
    }
    
    length++;
}

void line_list :: append (Abstract_text * line)
{
    if (length == 0)
    {
        list_node * tmp = new list_node;
        tmp = new list_node;
        tmp -> line = line;
        tmp -> next = NULL;
        head = tmp;        
    }
    else 
    {
        abstract_node * tmp = head;
        for (int i = 0; i < length - 1; i++)
            tmp = tmp -> next;
        list_node * tmp2 = new list_node;
        tmp2 -> line = line;
        tmp2 -> next = NULL;
        tmp -> next = tmp2;
    }
    
    length++;
}

line_list :: list_node :: ~list_node ()
{
    if (line) delete line;
}

line_list :: ~line_list ()
{   
    if (length > 1)
    {
        abstract_node * tmp = head -> next;
        abstract_node * tmp2;
        if (head) delete head;       
        for (int i = 0; i < length - 1; i++)
        {
            tmp2 = tmp -> next;           
            if (tmp) delete tmp;
            tmp = tmp2;
        }
    }
    else if (length == 1)
    {
        if (head) delete head;
    }
}
