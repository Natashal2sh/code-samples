#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <fstream>
#include <cstring>
#include "lex.h"
#include "synt.h"
#include "run.h"
#include "global.h"

#define N 512

using namespace std;

int main(int argc, char** argv)
{
    const char* filename = "Makefile3";
    fstream file(filename, fstream::in);
    vector<lexeme*>::iterator my_it;

    if (argc == 2 && !strcmp(argv[1], "only_lex")) {
        try {
            parser(filename);
        }

        catch (const char* s) {
            cout << s << endl;
            return 1;
        }

        for (my_it = Global::mfile.begin(); my_it != Global::mfile.end(); my_it++) {
            (*my_it)->print();
            cout << " " << Global::type_of_lex_str[(*my_it)->get_type()] << endl;
            delete (*my_it);
        }

        return 0;
    }

    char* target = new char[N];

    if (argc == 1) {
        sprintf(target, "all");
    }
    else {
        strcpy(target, argv[1]);
    }

    try {
        parser(filename);
        S();
        run(target);
    }

    catch (const char* s) {
        cout << s << endl;
        return 1;
    }

    catch (const std::exception& ex) {
        cout << "file does not exist" << endl;
        return 1;
    }

    vector<Target*>::iterator my_it2;

    for (my_it = Global::mfile.begin(); my_it != Global::mfile.end(); my_it++) {
        delete (*my_it);
    }

    for (my_it2 = Global::targets.begin(); my_it2 != Global::targets.end(); my_it2++) {
        vector<char*>::iterator str_it;
        for (str_it = ((*my_it2)->commands).begin(); str_it != ((*my_it2)->commands).end(); str_it++)
            delete[](*str_it);
        delete (*my_it2);
    }

    map<char*, char*>::iterator var_it;
    for (var_it = Global::variables.begin(); var_it != Global::variables.end(); var_it++) {
        delete[] var_it->second;
    }

    delete[] target;

    return 0;
}
