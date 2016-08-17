#ifndef H_WORD_LIST
#define H_WORD_LIST

#include <cstdio>
#include <fstream>
#include <cstring>

class Abstract_text; 

char * read_line (std::fstream & file, bool & abstract_end);
char * delete_spaces (char * line);
int symbols (char * line, int & position, int depth);
int get_number_of_digits (int n);

struct line_list 
{
    int length;
    
    struct abstract_node 
    {
        abstract_node * next;
        virtual unsigned long int length () const = 0;
        virtual unsigned long int count_words () const = 0;
        virtual void print (int depth, int & i) const = 0;
        virtual void print (int depth) const = 0; 
        virtual ~abstract_node () {}
    };
    
    struct common_node : abstract_node  
    {
        char * line;
        unsigned long int length () const { return strlen (line); }
        unsigned long int count_words () const;
        void print (int depth, int & i) const;
        void print (int depth) const;        
        ~common_node () { if (line) delete [] line; }
    };
    
    struct list_node : abstract_node
    {
        Abstract_text * line;
        unsigned long int length () const;
        unsigned long int count_words () const;
        void print (int depth, int & i) const;
        void print (int depth) const; 
        ~list_node ();        
    };    
    
    abstract_node * head;
    void append (char * line);
    void append (Abstract_text * line);
    line_list (): length (0) {}
    ~line_list ();
};

#endif
