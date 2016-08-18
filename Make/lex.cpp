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

char gc(fstream& file)
{
    char c = file.get();
    return c;
}

int line_counter = 0;

void parser(const char* filename)
{
    char c;
    char buf[N];
    int counter = 0;
    states cur_state = H;

    FILE* file1 = fopen(filename, "r");
    if (file1 == NULL) {
        return;
    }
    else {
        fclose(file1);
    }
    fstream file(filename, fstream::in);
    Global::mfile.push_back(new lexeme(LEX_NEW_LINE, "\n", line_counter));

    while (true) {
        switch (cur_state) {
        case H:
            c = gc(file);

            if (c == '#') {
                while (c != '\n' && !file.eof())
                    c = gc(file);
                line_counter++;
                if (file.eof())
                    cur_state = END;
            }
            else if (c == '$') {
                cur_state = DOLLAR;
            }
            else if (isalpha(c) || isdigit(c) || c == '.' || c == '*' || c == '-' || c == '_' || c == '+' || c == '@' || c == '/') {
                cur_state = WORD;
                buf[counter++] = c;
                buf[counter] = '\0';
            }
            else if (c == '%') {
                cur_state = WORD_PERCENT;
                buf[counter++] = c;
                buf[counter] = '\0';
            }
            else if (c == '\\') {
                c = gc(file);
            }
            else if (c == '\n') {
                line_counter++;
                Global::mfile.push_back(new lexeme(LEX_NEW_LINE, "\n", line_counter));
                cur_state = H;
            }
            else if (file.eof()) {
                cur_state = END;
            }
            else if (c == '=') {
                Global::mfile.push_back(new lexeme(LEX_EQ, "=", line_counter));
                cur_state = H;
            }
            else if (c == ':') {
                Global::mfile.push_back(new lexeme(LEX_COLON, ":", line_counter));
                cur_state = H;
            }
            else if (c == ')') {
                Global::mfile.push_back(new lexeme(LEX_RIGHT_BRACKET, ")", line_counter));
                cur_state = H;
            }
            else if (c == ' ') {
                cur_state = SPACE;
            }
            else if (c == '\t') {
                cur_state = TAB;
            }

            break;

        case WORD:
            c = gc(file);
            if (isalpha(c) || isdigit(c) || c == '.' || c == '*' || c == '-' || c == '_' || c == '+' || c == '/') {
                cur_state = WORD;
                buf[counter++] = c;
                buf[counter] = '\0';
            }
            else if (c == '%') {
                cur_state = WORD_PERCENT;
                buf[counter++] = c;
                buf[counter] = '\0';
            }
            else {
                counter = 0;
                file.seekg(-1, file.cur);
                cur_state = H;
                if (!strcmp(".PHONY", buf)) {
                    Global::mfile.push_back(new lexeme(LEX_PHONY, buf, line_counter));
                    cur_state = H;
                }
                else if (!strcmp("include", buf)) {
                    c = gc(file);
                    while (c == ' ')
                        c = gc(file);
                    cur_state = FILENAME;
                }
                else {
                    Global::mfile.push_back(new lexeme(LEX_WORD, buf, line_counter));
                }
            }

            break;

        case WORD_PERCENT:
            c = gc(file);
            if (isalpha(c) || isdigit(c) || c == '.' || c == '*' || c == '-' || c == '_' || c == '+' || c == '/') {
                cur_state = WORD_PERCENT;
                buf[counter++] = c;
                buf[counter] = '\0';
            }
            else if (c == '%') {
                cur_state = ERR;
            }
            else {
                file.seekg(-1, file.cur);
                counter = 0;
                Global::mfile.push_back(new lexeme(LEX_WORD_PERCENT, buf, line_counter));
                cur_state = H;
            }
            break;

        case DOLLAR:
            Global::mfile.push_back(new lexeme(LEX_DOLLAR, "$", line_counter));
            c = gc(file);
            switch (c) {
            case '(':
                cur_state = H;
                Global::mfile.push_back(new lexeme(LEX_LEFT_BRACKET, "(", line_counter));
                break;

            case '^':
                cur_state = H;
                Global::mfile.push_back(new lexeme(LEX_RIGHT_RULE, "^", line_counter));
                break;

            case '@':
                cur_state = H;
                Global::mfile.push_back(new lexeme(LEX_LEFT_RULE, "@", line_counter));
                break;

            default:
                cur_state = ERR;
            }

            break;

        case ERR:
            throw "ERROR";
            break;

        case SPACE:
            cur_state = H;
            break;

        case TAB:
            Global::mfile.push_back(new lexeme(LEX_TAB, "\t", line_counter));
            cur_state = H;
            break;

        case END:
            Global::mfile.push_back(new lexeme(LEX_END, "\n", line_counter));
            return;

        case FILENAME:
            counter = 0;

            while (c != '\n') {
                buf[counter++] = c;
                buf[counter] = '\0';
                c = gc(file);
            }

            parser(buf);
            Global::mfile.push_back(new lexeme(LEX_NEW_LINE, "\n", line_counter));
            line_counter++;
            counter = 0;
            cur_state = H;
            break;
        }
    }
}
