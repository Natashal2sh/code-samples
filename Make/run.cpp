#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include "lex.h"
#include "synt.h"
#include "run.h"
#include "global.h"

#define N 512

void run(char* target)
{
    vector<Target*>::iterator my_it;
    vector<Target*>::iterator my_it_found;
    bool found_target = false;

    for (my_it = Global::targets.begin(); my_it != Global::targets.end(); my_it++) {
        if (!strcmp(target, (*my_it)->target)) {
            found_target = true;
            my_it_found = my_it;

            if ((*my_it)->visited == true) {
                throw "Targets cycle";
            }

            (*my_it)->visited = true;

            if (((*my_it)->sub_targets).size() == 0) {
                vector<char*>::iterator my_it2;
                for (my_it2 = ((*my_it)->commands).begin(); my_it2 != ((*my_it)->commands).end(); my_it2++) {
                    system(*my_it2);
                    cout << *my_it2 << endl;
                }
                return;
            }
            else {
                vector<char*>::iterator my_it2;

                for (my_it2 = ((*my_it)->sub_targets).begin(); my_it2 != ((*my_it)->sub_targets).end(); my_it2++) {
                    int pos = 0;
                    while (pos >= 0) {
                        struct stat buf2;
                        char* result = get_word(*my_it2, pos);

                        run(result);

                        delete[] result;
                    }
                }
                break;
            }
        }
    }

    bool ok = false;
    bool found = false;
    vector<Target*>::iterator my_it2;
    int left_border = 0;
    int right_border = strlen(target) - 1;

    if (!found_target) {

        {
            for (my_it = Global::targets.begin(); my_it != Global::targets.end(); my_it++) {
                if (!((*my_it)->percent))
                    continue;

                int i = 0;

                while (true) {
                    if (((*my_it)->target)[i] == '%') {
                        ok = true;
                        break;
                    }
                    if (((*my_it)->target)[i] == target[i++]) {
                        left_border++;
                        continue;
                    }
                    else
                        break;
                }

                if (ok) {
                    ok = false;
                    int j = strlen(((*my_it)->target)) - 1;
                    i = strlen(target) - 1;

                    while (true) {
                        if (((*my_it)->target)[j] == '%') {
                            if (!found) {
                                my_it2 = my_it;
                                ok = true;
                                found = true;
                                break;
                            }
                            else {
                                throw "Two identical targets";
                            }
                        }
                        if (((*my_it)->target)[j--] == target[i--]) {
                            right_border--;
                            continue;
                        }
                        else
                            break;
                    }
                }
            }
        }

        if (found) {
            int counter = 0;
            vector<char*>::iterator s_it;

            for (s_it = ((*my_it2)->sub_targets).begin(); s_it != ((*my_it2)->sub_targets).end(); s_it++) {
                char* new_subtarg = new char[N];
                for (int i = 0; i < (int)strlen(*s_it); i++) {
                    if ((*s_it)[i] != '%') {
                        new_subtarg[counter++] = (*s_it)[i];
                    }
                    else {
                        for (int j = left_border; j <= right_border; j++) {
                            new_subtarg[counter++] = target[j];
                        }
                    }
                }

                new_subtarg[counter] = '\0';
                run(new_subtarg);
                delete[] new_subtarg;
            }

            counter = 0;
            vector<char*>::iterator s_it2;

            Target* tmp = *my_it2;

            for (s_it2 = (tmp->commands).begin(); s_it2 != (tmp->commands).end(); s_it2++) {
                char* new_command = new char[N];
                for (int i = 0; i < (int)strlen(*s_it2); i++) {
                    if ((*s_it2)[i] != '%') {
                        new_command[counter++] = (*s_it2)[i];
                    }
                    else {
                        for (int j = left_border; j <= right_border; j++) {
                            new_command[counter++] = target[j];
                        }
                    }
                }

                new_command[counter] = '\0';
                cout << new_command << endl;
                system(new_command);
                delete[] new_command;
            }
        }
        else {
            FILE* file = fopen(target, "r");
            if (file == NULL)
                throw "Missing file in the directory";
            else
                fclose(file);
            return;
        }
    }
    else {
        vector<char*>::iterator c_it;
        for (c_it = (*my_it_found)->commands.begin(); c_it != ((*my_it_found)->commands.end()); c_it++) {
            cout << *c_it << endl;
            system(*c_it);
        }
        return;
    }
}

char* get_word(char* word, int& pos)
{
    char* result = new char[N];
    int counter = 0;

    for (int i = pos;; i++) {
        if (word[i] != ' ' && word[i] != '\0') {
            result[counter++] = word[i];
        }
        else {
            if (word[i] == '\0') {
                pos = -1;
                break;
            }
            else {
                while (word[i++] == ' ' || word[i++] == '\t')
                    ;
                pos = i - 2;
                break;
            }
        }
    }

    result[counter] = '\0';

    return result;
}
