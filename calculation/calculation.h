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

char *ToChar(int number,char *result);
bool process(Token *token,Variable vars[],int vars_num);
bool MorghJudge(char *str,Token tokens[],int *i,Variable vars[],int vars_num);

bool IsAssignment(Token tokens[]);

bool Assignment(Token tokens[],Variable vars[],int tokens_num,int vars_num);
int ToInt(char *str);
int check_parentheses(Token *left,Token *right);
Token *FindMainOperator(Token *left,Token *right);
bool IsNeg(Token *left,Token * right);
int Calculate(Token *left, Token *right,int *check);

bool IsOutput(Token tokens,int tokens_num);
void output(Token tokens,Variable vars[],int vars_num);

void Assign(Token tokens[],Variable vars[],int vars_num, int tokens_num,int *check);

void Print(Token tokens[], int vars_num);

#endif //CALCULATION_H
