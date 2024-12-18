//
// Created by lixh1 on 24-12-15.
//

#ifndef CALCULATION_H
#define CALCULATION_H

#include <stdbool.h>

//define the structs.

enum tokens {ADD,SUB,MUL,DIV,EQU,LEFT,RIGHT,VARIABLE,INTEGER};

typedef struct token {
    int type;
    char str[32];
} Token;

typedef struct variable {
    char name[32];
    int value;
} Variable;

//define the functions.

bool IsDigit(char *str);
int IsOperator(char *str);
bool IsVariable(char *str);

bool process(Token *token);
bool MorghJudge(char *str,Token tokens[], int *i);

bool IsAssignment(Token tokens[]);

int ToInt(char *str);
int check_parentheses(Token *left,Token *right);
Token *FindMainOperator(Token *left,Token *right);
int Calculate(Token *left, Token *right);

void Assign(Token tokens[],Variable vars[],int vars_num);

void Print(Token tokens[], int vars_num);

#endif //CALCULATION_H
