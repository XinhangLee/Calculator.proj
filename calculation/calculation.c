//
// Created by lixh1 on 24-12-15.
//
#include "calculation.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

const char opt[7] = {'+','-','*','/','=','(',')'};

bool IsDigit(char *str) {
    bool result = true;
    if (*str == '0' && *(str + 1) != '\0') {
        result = false;
    }else {
        while (*str != '\0') {
            if (!isdigit(*str)) {
                result = false;
            }
            str++;
        }
    }
    return result;
}

int IsOperator(char *str) {
    int result = -1;
    for (int i = 0; i < 7; i++) {
        if (*str == opt[i]) {
            result = i;
            break;
        }
    }
    return result;
}

bool IsVariable(char *str) {
    bool result = true;
    if (!isalpha(str[0]) && str[0] != '_')
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

bool process(Token *token) {
    if (IsDigit(token->str))
        token->type = INTEGER;
    else if (IsOperator(token->str) != -1)
        token->type = IsOperator(token->str);
    else if (IsVariable(token->str))
        token->type = VARIABLE;
    else
        return false;
    return true;
}

bool MorghJudge(char *str,Token tokens[], int *i) {
    for (char *pos = strchr(str,' '); pos != NULL; str = pos + 1, pos = strchr(str, ' ')) {
        *pos = '\0';
        strcpy(tokens[*i].str,str);
        if (!process(&tokens[(*i)++]))
            return false;
    }
    strcpy(tokens[*i].str,str);
    if (!process(&tokens[(*i)++]))
        return false;
    // Print(tokens,*i);
    return true;
}

bool IsAssignment(Token tokens[]) {
    return tokens[0].type == VARIABLE && tokens[1].str[0] == '=';
}

int ToInt(char *str) {
    int value = 0;
    while (*str != '\0') {
        value = (value * 10) + (*str - '0');
        str++;
    }
    return value;
}

int check_parentheses(Token *left,Token *right) {
    int result = 1;//合法且想要。
    int count = 0;
    Token *l = left;
    while (l <= right) {
        if (l -> type == '(')
            count++;
        else if (l -> type == ')')
            count--;
        if (count < 0)
            result = -1;//不合语法，直接OUT。
        l++;
    }
    if (count != 0)
        result = -1;//不合语法，OUT。
    else if (left->type != '(' || right->type != ')')
        result = 0;//合语法但不是我想要的。
    return result;
}

Token *FindMainOperator(Token *left,Token *right) {
    Token *l = left;
    Token *key = l;//目标指针。
    int count = 0;
    while (l <= right) {
        if (l -> type == LEFT)
            count++;
        else if (l -> type == RIGHT)
            count--;
        if (count != 0)
            continue;
        if (key -> type != ADD && key -> type != SUB) {
            if (l -> type == ADD || l -> type == SUB || l -> type == MUL || l -> type == DIV) {
                key = l;
            }
        }
        else {
            if (l -> type == ADD || l -> type == SUB) {
                key = l;
            }
        }
        l++;
    }
    return key;
}

int Calculate(Token *left, Token *right) {
    int output = 0;
    if (left > right) {
        output = -1;
    }
    else if (left == right) {
        output = ToInt(left->str);
    }
    else if (check_parentheses(left,right) == 1) {
        return Calculate(left + 1,right - 1);
    }
    else if (check_parentheses(left,right) == -1) {
        output = -1;
    }
    else {
        Token *main = FindMainOperator(left,right);
        int val1 = Calculate(left,main - 1);
        int val2 = Calculate(main + 1,right);

        switch (main->type) {
            case ADD:
                output = val1 + val2;break;
            case SUB:
                output = val1 - val2;break;
            case MUL:
                output = val1 * val2;break;
            case DIV:
                output = val1 / val2;break;
            default:
                output = -1;break;
        }
    }
//output:   -1 : 违法表达式。
//     具体的值  ： 输出。
    return output;
}

void Assign(Token tokens[],Variable vars[],int vars_num) {

}

void Print(Token tokens[], int tokens_num) {
    for (int i = 0; i < tokens_num; i++) {
        printf("%d %s\n",tokens[i].type,tokens[i].str);
    }
}

