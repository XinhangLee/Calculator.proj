//
// Created by lixh1 on 24-12-15.
//

#ifndef CALCULATION_H
#define CALCULATION_H

#include <stdbool.h>

//define the structs.

enum tokens {VARIABLE,INTEGER,OPERATOR};

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
bool IsOperator(char *str);
bool IsVariable(char *str);

bool process(Token *token);
bool MorghJudge(char *str,Token tokens[]);

bool IsAssignment(Token tokens[]);

bool GramJudge(Token tokens[]);
int Calculate(Token tokens[]);

void Assign(Token tokens[],Variable vars[],int vars_num);

void Print(Token tokens[], int vars_num);

#endif //CALCULATION_H
