#ifndef H_SYNT
#define H_SYNT

#include <iostream>
#include <vector>

using namespace std;

struct Target 
{
        bool visited;
        bool percent;
        char * target;
        vector <char *> sub_targets;
        vector <char *> commands;
        Target (char * target_) : visited (false), percent (false), target (target_) {}
        void add_sub_target (char * sub_target) { sub_targets.push_back (sub_target); }
        void add_command (char * command) { commands.push_back (command); }
        void print ();  
};

enum dollar_state 
{
        TARGET,
        SUBTARGET,
        RULE
};

void S ();
void EQ ();
void COL ();
void VAR_VAL (char * value);
void TAB_F (char * current_command);
void DOLLAR_F (char * current_command = 0);
void SUB_TARGET ();
void COMMAND_WORD (char * current_command);

#endif
