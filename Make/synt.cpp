#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <fstream>
#include "lex.h"
#include "synt.h"
#include "run.h"
#include "global.h"

#define N 512

using namespace std;

int counter = 0;
dollar_state CS;
vector<lexeme*>::iterator lex_it;

void Target::print()
{
    cout << target << " : ";
    vector<char*>::iterator my_it;
    for (my_it = sub_targets.begin(); my_it != sub_targets.end(); my_it++)
        cout << *my_it << " ";
    cout << endl;
    for (my_it = commands.begin(); my_it != commands.end(); my_it++)
        cout << *my_it << endl;
    cout << endl;
}

void S()
{
    bool perc = false;

    if (counter == 0) {
        lex_it = Global::mfile.begin();
    }

    counter++;
    lex_it++;

    if (((*lex_it)->get_type()) == LEX_WORD || ((*lex_it)->get_type()) == LEX_WORD_PERCENT) {
        if (((*lex_it)->get_type()) == LEX_WORD_PERCENT) {
            perc = true;
        }
        lex_it++;

        if ((*lex_it)->get_type() == LEX_EQ) {
            EQ();
        }
        else if ((*lex_it)->get_type() == LEX_COLON) {
            lex_it--;
            Global::targets.push_back(new Target((*lex_it)->get_val()));
            if (perc) {
                Global::targets.back()->percent = true;
            }
            lex_it++;
            COL();
        }
        else {
            cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
            throw "Unexpected lexeme";
        }
    }
    else if ((*lex_it)->get_type() == LEX_TAB) {
        char* current_command = new char[N];
        current_command[0] = '\0';
        TAB_F(current_command);
    }
    else if ((*lex_it)->get_type() == LEX_DOLLAR) {
        CS = TARGET;
        DOLLAR_F();
    }
    else if ((*lex_it)->get_type() == LEX_NEW_LINE) {
        S();
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        return;
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "Unexpected lexeme";
    }
}

void TAB_F(char* current_command)
{
    lex_it++;
    if ((*lex_it)->get_type() == LEX_TAB) {
        TAB_F(current_command);
    }
    else if ((*lex_it)->get_type() == LEX_WORD || ((*lex_it)->get_type()) == LEX_WORD_PERCENT) {
        COMMAND_WORD(current_command);
        Global::targets.back()->add_command(current_command);
        lex_it--;
        S();
    }
    else if ((*lex_it)->get_type() == LEX_NEW_LINE) {
        Global::targets.back()->add_command(current_command);
        lex_it--;
        S();
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        Global::targets.back()->add_command(current_command);
        return;
    }
    else if (((*lex_it)->get_type()) == LEX_DOLLAR) {
        CS = RULE;
        DOLLAR_F(current_command);
        lex_it++;
        COMMAND_WORD(current_command);
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "Unexpexted lexeme in command line";
    }
}

void COMMAND_WORD(char* current_command)
{
    if ((*lex_it)->get_type() == LEX_WORD || ((*lex_it)->get_type()) == LEX_WORD_PERCENT) {
        strcat(current_command, (*lex_it)->get_val());
        int len = strlen(current_command);
        current_command[len] = ' ';
        current_command[len + 1] = '\0';
        lex_it++;
        COMMAND_WORD(current_command);
    }
    else if (((*lex_it)->get_type()) == LEX_NEW_LINE) {
        return;
    }
    else if (((*lex_it)->get_type()) == LEX_DOLLAR) {
        CS = RULE;
        DOLLAR_F(current_command);
        lex_it++;
        COMMAND_WORD(current_command);
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        return;
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "Unexpected lexeme in command line";
    }
}

void EQ()
{
    lex_it--;
    char* name = (*lex_it)->get_val();
    if (isdigit(name[0])) {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "wrong variable name";
    }
    char* value = new char[N];
    value[0] = '\0';
    lex_it += 2;

    if (((*lex_it)->get_type()) == LEX_WORD) {
        VAR_VAL(value);
        value[strlen(value) - 1] = '\0';
        Global::variables.insert(pair<char*, char*>(name, value));
        S();
    }
    else if (((*lex_it)->get_type()) == LEX_NEW_LINE) {
        if (strlen(value) > 0)
            value[strlen(value) - 1] = '\0';
        Global::variables.insert(pair<char*, char*>(name, value));
        lex_it--;
        S();
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        value[strlen(value) - 1] = '\0';
        Global::variables.insert(pair<char*, char*>(name, value));
        return;
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "ERROR_EQ";
    }
}

void VAR_VAL(char* value)
{
    strcat(value, (*(lex_it))->get_val());
    int len = strlen(value);
    value[len] = ' ';
    value[len + 1] = '\0';
    lex_it++;

    if (((*lex_it)->get_type()) == LEX_WORD) {
        VAR_VAL(value);
    }
    else if (((*lex_it)->get_type()) == LEX_NEW_LINE) {
        return;
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        return;
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "ERROR_VAR_VAL";
    }
}

void DOLLAR_F(char* current_command)
{
    lex_it++;

    if (((*lex_it)->get_type()) == LEX_LEFT_BRACKET) {
        bool var_found = false;
        lex_it++;
        if (((*lex_it)->get_type()) == LEX_WORD || ((*lex_it)->get_type()) == LEX_WORD_PERCENT) {
            map<char*, char*>::iterator var_it;
            for (var_it = Global::variables.begin();
                 var_it != Global::variables.end(); var_it++) {
                if (!strcmp(var_it->first, (*lex_it)->get_val())) {
                    if (CS == TARGET) {
                        Global::targets.push_back(new Target(var_it->second));
                    }
                    else if (CS == SUBTARGET) {
                        Global::targets.back()->add_sub_target(var_it->second);
                    }
                    else if (CS == RULE) {
                        strcat(current_command, var_it->second);
                        int len = strlen(current_command);
                        current_command[len] = ' ';
                        current_command[len + 1] = '\0';
                    }
                    var_found = true;
                    break;
                }
            }

            if (!var_found) {
                cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line())
                     << endl;
                throw "Nonexisting variable";
            }
            else {
                lex_it++;
                if (((*lex_it)->get_type()) == LEX_RIGHT_BRACKET) {
                    lex_it++;
                    if (CS == TARGET) {
                        if (((*lex_it)->get_type()) == LEX_COLON) {
                            COL();
                        }
                        else {
                            cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line())
                                 << endl;
                            throw "Missing ':' after target";
                        }
                    }
                    else if (CS == SUBTARGET) {
                        SUB_TARGET();
                    }
                    else {
                        COMMAND_WORD(current_command);
                    }
                }
                else {
                    cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line())
                         << endl;
                    throw "Missing ')' after variable name";
                }
            }
        }
        else {
            cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
            throw "Unexpected lexeme after left brecket";
        }
    }
    else if (((*lex_it)->get_type()) == LEX_LEFT_RULE) {
        if (CS != RULE) {
            cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
            throw "Wrong variable";
        }
        strcat(current_command, Global::targets.back()->target);
        int len = strlen(current_command);
        current_command[len] = ' ';
        current_command[len + 1] = '\0';
        return;
    }
    else if (((*lex_it)->get_type()) == LEX_RIGHT_RULE) {
        if (CS != RULE) {
            cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
            throw "Wrong variable";
        }
        vector<char*>::iterator st_it;
        for (st_it = (Global::targets.back()->sub_targets).begin();
             st_it != (Global::targets.back()->sub_targets).end(); st_it++) {
            strcat(current_command, *st_it);
            int len = strlen(current_command);
            current_command[len] = ' ';
            current_command[len + 1] = '\0';
            return;
        }
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        return;
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "Unexpected lexeme after dollar sign";
    }
}

void COL()
{
    lex_it++;
    if (((*lex_it)->get_type()) == LEX_NEW_LINE) {
        lex_it--;
        S();
    }
    else if (((*lex_it)->get_type()) == LEX_WORD || ((*lex_it)->get_type()) == LEX_WORD_PERCENT) {
        SUB_TARGET();
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        return;
    }
    else if ((*lex_it)->get_type() == LEX_DOLLAR) {
        CS = SUBTARGET;
        DOLLAR_F();
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "Unexpected lexeme in subtargets line";
    }
}

void SUB_TARGET()
{
    if (((*lex_it)->get_type()) == LEX_WORD || ((*lex_it)->get_type()) == LEX_WORD_PERCENT) {
        Global::targets.back()->add_sub_target((*lex_it)->get_val());
        lex_it++;
        SUB_TARGET();
    }
    else if ((*lex_it)->get_type() == LEX_DOLLAR) {
        CS = SUBTARGET;
        DOLLAR_F();
    }
    else if (((*lex_it)->get_type()) == LEX_NEW_LINE) {
        lex_it--;
        S();
    }
    else if ((*lex_it)->get_type() == LEX_END) {
        return;
    }
    else {
        cout << ((*lex_it)->get_val()) << " " << ((*lex_it)->get_line()) << endl;
        throw "Unexpected lexeme in subtargets list";
    }
}


