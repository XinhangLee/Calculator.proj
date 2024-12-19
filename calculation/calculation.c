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

char *ToChar(int number,char *result) {
    int i;
    for (i = 0; number != 0; i++, number /= 10) {
        result[i] = number % 10 + '0';
    }
    i--;
    for (int j = 0; j < i; i--,j++) {
        char temp = result[j];
        result[j] = result[i];
        result[i] = temp;
    }
    return result;
}

bool process(Token *token,Variable vars[],int vars_num) {
    int op = IsOperator(token->str);
    if (IsDigit(token->str))
        token->type = INTEGER;
    else if (op != -1){
        token->type = op;
    }
    else if (IsVariable(token->str)) {
        token->type = VARIABLE;
    }
    else
        return false;
    return true;
}

bool MorghJudge(char *str,Token tokens[], int *i,Variable vars[],int vars_num) {
    for (char *pos = strchr(str,' '); pos != NULL; str = pos + 1, pos = strchr(str, ' ')) {
        *pos = '\0';
        strcpy(tokens[*i].str,str);
        if (!process(&tokens[(*i)++],vars,vars_num))
            return false;
    }
    strcpy(tokens[*i].str,str);
    if (!process(&tokens[(*i)++],vars,vars_num))
        return false;
    // Print(tokens,*i);
    return true;
}

bool Assignment(Token tokens[],Variable vars[],int tokens_num,int vars_num) {
    for (int j = 0; j < tokens_num; j++) {
        if (tokens[j].type == VARIABLE) {
            int found = 0;
            for (int i = 0; i < vars_num; i++) {
                if (strcmp(tokens[j].str,vars[i].name) == 0) {
                    char temp[50] = {'\0'};
                    strcpy(tokens[j].str, ToChar(vars[i].value,temp));
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    return true;
}

int ToInt(char *str) {
    int value = 0;
    while (*str != '\0') {
        if (isdigit(*str)) {
            value = (value * 10) + (*str - '0');
        }
        else if (*str == '-') {
            value = -value;
        }
        str++;
    }
    return value;
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

bool IsNeg(Token *left,Token *right) {
    bool result = true;
    Token *l = left;
    if (l -> type != SUB)
        result = false;
    while (result && l < right) {
        if (l -> type != SUB) {
            result = false;
            break;
        }
        l++;
    }
    if (right -> type != INTEGER && right -> type != VARIABLE)
        result = false;
    return result;
}

int Calculate(Token *left, Token *right,int *check) {
    int output = 0;
    if (left > right) {
        *check = 0;
        return output;
    }
    if (left == right) {
        output = ToInt(left->str);
    }
    else if (check_parentheses(left,right) == 1) {
        return Calculate(left + 1,right - 1,check);
    }
    else if (check_parentheses(left,right) == -1) {
        *check = 0;
        return output;
    }
    else if (IsNeg(left,right)) {
        strcat((left + 1) -> str,left ->str);
        output = Calculate(left + 1,right,check);
    }
    else {
        Token *main = FindMainOperator(left,right);
        if (main->type != ADD && main->type != SUB && main->type != MUL && main->type != DIV) {
            *check = 0;
            return output;
        }
        int val1 = Calculate(left,main - 1,check);
        int val2 = Calculate(main + 1,right,check);
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
    return output;
}

bool IsOutput(Token tokens,int tokens_num) {
    return tokens_num == 1 && tokens.type == VARIABLE;
}

void output(Token tokens,Variable vars[],int vars_num) {
    for (int i = 0; i < vars_num; i++) {
        if (strcmp(vars[i].name,tokens.str) == 0) {
            printf("%d\n",vars[i].value);
            return;
        }
    }
    printf("Error\n");
}

bool IsAssignment(Token tokens[]) {
    return tokens[0].type == VARIABLE && tokens[1].str[0] == '=';
}

void Assign(Token tokens[],Variable vars[],int vars_num, int tokens_num,int *check) {
    strcpy(vars[vars_num].name, tokens[0].str);
    vars[vars_num].value = Calculate(tokens + 2,tokens + tokens_num - 1,check);

}

void Print(Token tokens[], int tokens_num) {
    for (int i = 0; i < tokens_num; i++) {
        printf("%d %s\n",tokens[i].type,tokens[i].str);
    }
}

