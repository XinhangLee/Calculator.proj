//
// Created by lixh1 on 24-12-15.
//
#include <ctype.h>

#include "calculation/calculation.h"
#include <stdio.h>
#include <string.h>

int main() {
    char str[1030];
    Variable vars[100];
    int vars_num = 0;
    while (fgets(str, sizeof(str), stdin)) {
        // printf("%s\n", str);
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        }

        Token tokens[150];

        if (MorghJudge(str,tokens)) {
            if (IsAssignment(tokens)) {
                // printf("yes\n");
                if (GramJudge(tokens)) {
                    printf("%d",Calculate(tokens));
                } else {
                    printf("Error\n");
                }
            } else {
                Assign(tokens,vars,vars_num);
            }
        } else {
            printf("Error\n");
        }
    }
    return 0;
}
