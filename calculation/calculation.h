//
// Created by lixh1 on 24-12-15.
//

#ifndef CALCULATION_H
#define CALCULATION_H

#include <stdbool.h>

//define the structs.

enum tokens {INTEGER,FLOATNUM,VARIABLE,ADD,SUB,MUL,DIV,EQU,LEFT,RIGHT};

typedef union out{
    int iOut;
    double fOut;
} Out;

typedef struct output {
    enum {
        INT,
        FLOAT,
        ERROR
    } outType;
    Out out;
}Output;

typedef union value {
    char str[32];
    Out out;
} Value;

typedef struct token {
    int type;
    Value value;
} Token;

typedef struct variable {
    char name[32];
    Output output;
} Variable;




//define the functions.

bool IsDigit(char *str);
bool IsFloat (char *str);
int IsOperator(char *str);
bool IsVariable(char *str);

bool process(Token *token);
bool MorghJudge(char *str,Token tokens[],int *i);

bool IsAssignment(Token *tokens);
bool IsOutput(Token tokens,int tokens_num);

bool Assignment(Token *left,Token *right,Variable vars[],int vars_num);
void ToNum(Token *tokens);
int check_parentheses(Token *left,Token *right);
Token *FindMainOperator(Token *left,Token *right);
int IsNeg(Token *left,Token * right);
Output meetValue(int a,Output val1,Output val2);
Output Calculate(Token *left, Token *right,Variable vars[],int vars_num);

bool IsOutput(Token tokens,int tokens_num);
void output(Token tokens,Variable vars[],int vars_num);

void Assign(Token *tokens,Variable vars[],int *vars_num, int tokens_num);

void printOut(Output output);

#endif //CALCULATION_H
