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
        int tokens_num = 0;

        if (MorghJudge(str,tokens,&tokens_num)) {
            // printf("%d\n", tokens_num);

            if (!IsAssignment(tokens) && !IsOutput(tokens[0],tokens_num)) {
                // printf("yes\n");
                printOut(Calculate(tokens,tokens + tokens_num - 1,vars,vars_num));
            }
            else if (IsOutput(tokens[0],tokens_num)) {
                output(tokens[0],vars,vars_num);
            }
            else {
                int mem = Assign(tokens,vars,&vars_num,tokens_num);
                if (mem != -1) {
                    printOut(vars[mem].output);
                }
                else {
                    printf("Error\n");
                }
            }
        }

        else {
        printf("Error\n");
        }
    }

    return 0;
}

