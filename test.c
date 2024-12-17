//
// Created by lixh1 on 24-12-15.
//
#include "calculation/calculation.h"
#include <stdio.h>

int main() {
    char str[1030];
    Variable vars[100];
    int vars_num = 0;
    while (fgets(str, sizeof(str), stdin)) {

        Token tokens[150];

        MorghJudge(str,tokens);

        GramJudge(tokens);
        Calculate(tokens);

        Assign(tokens,vars,vars_num);
    }
    return 0;
}