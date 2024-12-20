//
// Created by lixh1 on 24-12-15.
//
#include "calculation.h"

#include <complex.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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
    int op = IsOperator(token->value.str);
    if (IsDigit(token->value.str)) {
        token->type = INTEGER;
        ToNum(token);
    }
    else if (op != -1){
        token->type = op;
    }
    else if (IsVariable(token->value.str)) {
        token->type = VARIABLE;
    }
    else
        return false;
    return true;
}

bool MorghJudge(char *str,Token tokens[], int *i) {
    for (char *pos = strchr(str,' '); pos != NULL; str = pos + 1, pos = strchr(str, ' ')) {
        *pos = '\0';
        strcpy(tokens[*i].value.str,str);
        if (!process(&tokens[(*i)++]))
            return false;
    }
    strcpy(tokens[*i].value.str,str);
    if (!process(&tokens[(*i)++]))
        return false;
    // Print(tokens,*i);
    return true;
}

bool Assignment(Token *left,Token *right,Variable vars[],int vars_num) {
    Token *l = left;
    while (l <= right) {
        if (l->type == VARIABLE) {
            int found = 0;
            for (int i = 0; i < vars_num; i++) {
                if (strcmp(l->value.str,vars[i].name) == 0) {
                    l->value.num = vars[i].value;
                    l->type = INTEGER;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        l++;
    }
    return true;
}

void ToNum(Token *tokens) {
    int temp = atoi(tokens->value.str);
    tokens->value.num = temp;
}

int check_parentheses(Token *left,Token *right) {
    int result = 1;//合法且想要。
    int count = 0;
    Token *l = left;
    while (l <= right) {
        if (l -> type == LEFT)
            count++;
        else if (l -> type == RIGHT)
            count--;
        if (count < 0)
            result = -1;//不合语法，直接OUT。
        l++;
    }
    if (count != 0)
        result = -1;//不合语法，OUT。
    else if (left->type != LEFT || right->type != RIGHT)
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
        if (count != 0) {
            l++;
            continue;
        }
        if (key -> type == ADD || (key -> type == SUB && ((key - 1) -> type == VARIABLE || (key - 1) -> type == INTEGER))) {
            if (l -> type == ADD || (l -> type == SUB && ((l - 1) -> type == VARIABLE || (l - 1) -> type == INTEGER))) {
                key = l;
            }
        }
        else {
            if (l -> type == ADD || (l -> type == SUB && ((l - 1) -> type == VARIABLE || (l - 1) -> type == INTEGER)) || l -> type == MUL || l -> type == DIV) {
                key = l;
            }
        }
        l++;
    }
    return key;
}

int IsNeg(Token *left,Token *right) {
    int SubNum = 0;
    Token *l = left;
    while (l -> type == SUB) {
        SubNum++;
        l++;
    }
    if (!(l == right && (l -> type == INTEGER || l -> type == VARIABLE)) && !(l != right && check_parentheses(l,right))) {
        SubNum = 0;
    }
    return SubNum;
}

int Calculate(Token *left, Token *right,int *check,Variable vars[],int vars_num) {
    int output = 0;
    if (Assignment(left,right,vars,vars_num)) {
        if (left > right) {
            *check = 0;
            return output;
        }
        if (left == right) {
            output = left ->value.num;
        }
        else if (check_parentheses(left,right) == 1) {
            return Calculate(left + 1,right - 1,check,vars,vars_num);
        }
        else if (check_parentheses(left,right) == -1) {
            *check = 0;
            return output;
        }
        else if (IsNeg(left,right)) {
            int SubNum = IsNeg(left,right);
            output = Calculate(left + SubNum,right,check,vars,vars_num);
            while (SubNum--) {
                output = -output;
            }
        }
        else {
            Token *main = FindMainOperator(left,right);
            if (main->type != ADD && main->type != SUB && main->type != MUL && main->type != DIV) {
                *check = 0;
                return output;
            }
            int val1 = Calculate(left,main - 1,check,vars,vars_num);
            int val2 = Calculate(main + 1,right,check,vars,vars_num);
            switch (main->type) {
                case ADD:
                    output = val1 + val2;break;
                case SUB:
                    output = val1 - val2;break;
                case MUL:
                    output = val1 * val2;break;
                case DIV:
                    if (val2 == 0) {
                        *check = 0;
                        break;
                    }
                output = val1 / val2;break;
                default:
                    *check = 0;break;
            }
        }
    }
    else {
        *check = 0;
    }
    return output;
}

bool IsOutput(Token tokens,int tokens_num) {
    return tokens_num == 1 && tokens.type == VARIABLE;
}

void output(Token tokens,Variable vars[],int vars_num) {
    for (int i = 0; i < vars_num; i++) {
        if (strcmp(vars[i].name,tokens.value.str) == 0) {
            printf("%d\n",vars[i].value);
            return;
        }
    }
    printf("Error\n");
}

bool IsAssignment(Token *tokens) {
    return tokens[0].type == VARIABLE && tokens[1].value.str[0] == '=';
}

void Assign(Token *tokens,Variable vars[],int *vars_num, int tokens_num,int *check) {
    if (!IsAssignment(tokens + 2)) {
        strcpy(vars[*vars_num].name, tokens[0].value.str);
        vars[*vars_num].value = Calculate(tokens + 2,tokens + tokens_num - 1,check,vars,*vars_num);
        *vars_num += 1;
    }
    else {
        Assign(tokens + 2,vars,vars_num,tokens_num - 2,check);
        strcpy(vars[*vars_num].name, tokens[0].value.str);
        vars[*vars_num].value = vars[*vars_num - 1].value;
        *vars_num += 1;
    }
}

void Print(Token tokens[], int tokens_num) {
    for (int i = 0; i < tokens_num; i++) {
        printf("%d %s\n",tokens[i].type,tokens[i].value.str);
    }
}

