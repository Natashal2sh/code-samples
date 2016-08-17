#include "abstract_list.h"
#include "header.h"
#include "num_list.h"
#include "mark_list.h"
#include "abstract.h"
#include <typeinfo>
#include <string.h>
#include <iostream>

using namespace std;

void abstract_list :: append (Abstract_text * elem)
{
    if (length == 0)
    {
        head = new node;
        head -> elem = elem;
        head -> next = NULL;
    }
    else 
    {
        node * tmp = head;
        for (int i = 0; i < length - 1; i++)
            tmp = tmp -> next;
            
        tmp -> next = new node;
        tmp -> next -> elem = elem;
        tmp -> next -> next = NULL;    
    }
        
    length++;
}

abstract_list :: node :: ~node ()
{
    delete elem;
} 

abstract_list :: ~abstract_list ()
{
    if (length > 1)
    {
        node * tmp = head -> next;
        node * tmp2;
        if (head) delete head;
        for (int i = 0; i < length- 1; i++)
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
