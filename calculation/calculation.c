//
// Created by lixh1 on 24-12-15.
//
#include "calculation.h"
#include <string.h>
#include <ctype.h>

bool IsDigit(char *str) {
    bool result = true;
    while (*str != '\0') {
        if (!isdigit(*str)) {
            result = false;
        }
        str++;
    }
    return result;
}

bool IsOperator(char *str) {
    bool result = false;
    for (int i = 0; i < 7; i++) {
        if (*str == opt[i]) {
            result = true;
            break;
        }
    }
    return result;
}

bool IsVariable(char *str) {
    bool result = true;
    if (!isalpha(str[1]) && str[1] != '_')
        result = false;
    else {
        while (*str != '\0') {
            if (!isalnum(*str) && *str != '_') {
                result = false;
                break;
            }
            str++;
        }
    }
    return result;
}

bool process(Token token) {
    if (IsDigit(token.str))
        token.type = INTEGER;
    else if (IsOperator(token.str))
        token.type = OPERATOR;
    else if (IsVariable(token.str))
        token.type = VARIABLE;
    else
        return false;
    return true;
}

bool MorghJudge(char *str,Token tokens[]) {
    int i = 0;
    for (char *pos = strchr(str,' '); pos != NULL; pos = pos + 1) {
        *pos = '\0';
        strcpy(tokens[i].str,str);
        if (!process(tokens[i]))
            return false;
    }
    return true;
}

bool GramJudge(Token tokens[]) {

}

int Calculate(Token tokens[]) {


    return 0;
}

void Assign(Token tokens[], Variable vars[], int vars_num) {

}
