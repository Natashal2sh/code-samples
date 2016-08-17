#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <typeinfo>
#include <fstream>

#include "abstract_text.h"
#include "abstract.h"
#include "num_list.h"
#include "mark_list.h"
#include "word_list.h"
#include "header.h"
#include "abstract_list.h"
#include "text_viewer.h"
#include "config.h"

#define DEF_M '*'
#define DEF_SP 2
#define DEF_RL 4
#define DEF_W 80

using namespace std;

int main (int argc, char ** argv)
{
    bool h_flag = false, v_flag = false, f_flag = false;
    int opt;
    char * filename = NULL;
    
    Configuration :: width = DEF_W;
    Configuration :: n_spaces = DEF_SP;
    Configuration :: red_line = DEF_RL;
    Configuration :: marker = DEF_M;
    
    try 
    {
        while ((opt = getopt (argc, argv, "f:w:t:m:r:hv")) != -1) {
            switch (opt) 
            {
                case 'f':    filename = optarg;
                             f_flag = true;
                             break;
                             
                case 'w':    Configuration :: width = atoi (optarg);
                             if (Configuration :: width <= 0) throw "Wrong width";
                             break;
                             
                case 't':    Configuration :: n_spaces = atoi (optarg);
                             if (Configuration :: n_spaces < 0) throw "Wrong number of spaces";
                             break;
                             
                case 'm':    Configuration :: marker = optarg[0];
                             break;
                             
                case 'r':    Configuration :: red_line = atoi (optarg);
                             if (Configuration :: red_line < 0) throw "Wrong red line";
                             break;
                             
                case 'h':    h_flag = true;
                             break;
                             
                case 'v':    v_flag = true;
                             break;                         
                                     
                case '?':    throw "Wrong arguments";
                             break;
            }
        }
        
        if (h_flag)
        {
            cout << "HELP" << endl;
            if (v_flag)
            {
                cout << "version 1.0" << endl;
            }
            
            return 0;
        }
        
        if (f_flag)
        {
            fstream file (filename, fstream :: in);
            Text_viewer my_text(file);
        
            my_text.print ();
            
            cerr << endl << "Number of symbols: " << my_text.count_symbols () << endl;
            cerr << "Number of words: " << my_text.count_words () << endl;
        }
    }
    
    catch (const char * str)
    {
        cout << endl << str << endl;
        //delete [] str;
        return 1;
    }
        
    
    if (v_flag)
    {
        cout << "version 1.0" << endl;
    }
    
    return 0;    
}
