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
    if ((*str == '0' && *(str + 1) != '\0') || *str == '\0') {
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

bool IsFloat (char *str) {
    int pos = 0,count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            pos = i;
            count++;
        }
    }
    if (count != 1)
        return false;
    str[pos] = '\0';
    if (!IsDigit(str)) {
        return false;
    }
    if (str[pos + 1] == '\0')
        return false;
    char *sign = str + pos + 1;
    while (*sign != '\0') {
        if (!isdigit(*sign)) {
            return false;
        }
        sign++;
    }
    str[pos] = '.';
    return true;
}

int IsOperator(char *str) {
    int result = -4;
    for (int i = 0; i < 7; i++) {
        if (*str == opt[i]) {
            result = i;
            break;
        }
    }
    return result + 3;
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
    else if (IsFloat(token->value.str)) {
        token->type = FLOATNUM;
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
                    l->value.out = vars[i].output.out;
                    l->type = vars[i].output.outType;
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
    if (tokens->type == INTEGER) {
        int temp = atoi(tokens->value.str);
        tokens->value.out.iOut = temp;
    }
    else if (tokens->type == FLOATNUM) {
        double temp = atof(tokens->value.str);
        tokens->value.out.fOut = temp;
    }
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
        if (l != right && count == 0)
            result = 0;
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
        if (key -> type == ADD || (key -> type == SUB && ((key - 1) -> type == VARIABLE || (key - 1) -> type == INTEGER || (key - 1) -> type == FLOATNUM || (key - 1) -> type == RIGHT))) {
            if (l -> type == ADD || (l -> type == SUB && ((l - 1) -> type == VARIABLE || (l - 1) -> type == INTEGER || (l - 1) -> type == FLOATNUM || (l - 1) -> type == RIGHT))) {
                key = l;
            }
        }
        else {
            if (l -> type == ADD || (l -> type == SUB && ((l - 1) -> type == VARIABLE || (l - 1) -> type == INTEGER || (l - 1) -> type == FLOATNUM || (l - 1) -> type == RIGHT)) || l -> type == MUL || l -> type == DIV) {
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
    if (!(l == right && (l -> type == INTEGER || l -> type == VARIABLE || l -> type == FLOATNUM)) && !(l != right && check_parentheses(l,right))) {
        SubNum = 0;
    }
    return SubNum;
}

Output meetValue(int a,Output val1,Output val2) {
    Output output = {ERROR};
    if (val1.outType == ERROR || val2.outType == ERROR) {
        return (Output){ERROR};
    }
    if (val1.outType == INT && val2.outType == INT) {
        output.outType = INT;
        switch (a) {
            case ADD:
                output.out.iOut = val1.out.iOut + val2.out.iOut;break;
            case SUB:
                output.out.iOut = val1.out.iOut - val2.out.iOut;break;
            case MUL:
                output.out.iOut = val1.out.iOut * val2.out.iOut;break;
            case DIV:
                output.out.iOut = val1.out.iOut / val2.out.iOut;break;
            default:
                output.outType = ERROR;
        }
    }
    else {
        if (val1.outType != val2.outType) {
            if (val1.outType == INTEGER) {
                double temp = val1.out.iOut;
                val1.out.fOut = temp;
            }
            else {
                double temp = val2.out.iOut;
                val2.out.fOut = temp;
            }
        }
        output.outType = FLOAT;
        switch (a) {
            case ADD:
                output.out.fOut = val1.out.fOut + val2.out.fOut;break;
            case SUB:
                output.out.fOut = val1.out.fOut - val2.out.fOut;break;
            case MUL:
                output.out.fOut = val1.out.fOut * val2.out.fOut;break;
            case DIV:
                output.out.fOut = val1.out.fOut / val2.out.fOut;break;
            default:
                output.outType = ERROR;
        }
    }

    return output;
}

Output Calculate(Token *left, Token *right,Variable vars[],int vars_num) {
    Output output = {ERROR};
    if (Assignment(left,right,vars,vars_num)) {
        if (left > right) {
            return (Output){ERROR};
        }
        if (left == right) {
            output.out = left ->value.out;
            output.outType = left->type;
        }
        else if (check_parentheses(left,right) == 1) {
            return Calculate(left + 1,right - 1,vars,vars_num);
        }
        else if (check_parentheses(left,right) == -1) {
            return (Output){ERROR};
        }
        else if (IsNeg(left,right)) {
            int SubNum = IsNeg(left,right);
            output = Calculate(left + SubNum,right,vars,vars_num);
            while (SubNum--) {
                if (output.outType == INT)
                    output.out.iOut = -output.out.iOut;
                else
                    output.out.fOut = -output.out.fOut;
            }
        }
        else {
            Token *main = FindMainOperator(left,right);
            if (main->type != ADD && main->type != SUB && main->type != MUL && main->type != DIV) {
                return (Output){ERROR};
            }
            Output val1 = Calculate(left,main - 1,vars,vars_num);
            Output val2 = Calculate(main + 1,right,vars,vars_num);
            return meetValue(main->type,val1,val2);
        }
    }
    else {
        return (Output){ERROR};
    }
    return output;
}

bool IsOutput(Token tokens,int tokens_num) {
    return tokens_num == 1 && tokens.type == VARIABLE;
}

void output(Token tokens,Variable vars[],int vars_num) {
    for (int i = 0; i < vars_num; i++) {
        if (strcmp(vars[i].name,tokens.value.str) == 0) {
            if (vars[i].output.outType == INT)
                printf("%d\n",vars[i].output.out.iOut);
            else
                printf("%.6lf\n",vars[i].output.out.fOut);
            return;
        }
    }
    printf("Error\n");
}

bool IsAssignment(Token *tokens) {
    return tokens[0].type == VARIABLE && tokens[1].value.str[0] == '=';
}

int FindSame(Variable vars[],int vars_num,char *str) {
    int result = -1;
    for (int i = 0; i < vars_num; i++) {
        if (strcmp(vars[i].name,str) == 0) {
            result = i;
            break;
        }
    }
    return result;
}

int Assign(Token *tokens,Variable vars[],int *vars_num, int tokens_num) {
    if (!IsAssignment(tokens + 2)) {
        int index = FindSame(vars,*vars_num,tokens[0].value.str);
        if (index == -1) {
            strcpy(vars[*vars_num].name, tokens[0].value.str);
            vars[*vars_num].output = Calculate(tokens + 2,tokens + tokens_num - 1,vars,*vars_num);
            if (vars[*vars_num].output.outType != ERROR) {
                *vars_num += 1;
                return *vars_num - 1;
            }
            return -1;
        }
        vars[index].output = Calculate(tokens + 2,tokens + tokens_num - 1,vars,*vars_num);
        if (vars[index].output.outType != ERROR)
            return index;
        return -1;
    }
    if (Assign(tokens + 2,vars,vars_num,tokens_num - 2) != -1) {
        int index = FindSame(vars,*vars_num,tokens[0].value.str);
        if (index == -1) {
            strcpy(vars[*vars_num].name, tokens[0].value.str);
            vars[*vars_num].output = vars[*vars_num - 1].output;
            *vars_num += 1;
            return *vars_num - 1;
        }
        vars[index].output = vars[Assign(tokens + 2,vars,vars_num,tokens_num - 2)].output;
        return index;
    }
    return -1;
}

void printOut(Output output) {
    if (output.outType == ERROR) {
        printf("Error\n");
    }
    if (output.outType == INT) {
        printf("%d\n",output.out.iOut);
    }
    if (output.outType == FLOAT) {
        printf("%.6lf\n",output.out.fOut);
    }
}


