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
    int vars_num = 0,check = 1;

    while (fgets(str, sizeof(str), stdin)) {
        // printf("%s\n", str);

        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        }

        Token tokens[150];
        int tokens_num = 0;

        if (MorghJudge(str,tokens,&tokens_num,vars,vars_num)) {
            // printf("%d\n", tokens_num);

            if (!IsAssignment(tokens) && !IsOutput(tokens[0],tokens_num) &&
                Assignment(tokens,vars,tokens_num,vars_num)) {
                // printf("yes\n");
                int result = Calculate(tokens,tokens + tokens_num - 1,&check);
                if (check == 0) {
                    printf("Error\n");
                }
                else {
                    printf("%d\n", result);
                }
            }
            else if (IsOutput(tokens[0],tokens_num)) {
                output(tokens[0],vars,vars_num);
            }
            else {
                Assign(tokens,vars,vars_num,tokens_num,&check);
                if (check == 0) {
                    printf("Error\n");
                }else {
                    printf("%d\n", vars[vars_num].value);
                    vars_num++;
                }
            }
        }

        else {
        printf("Error\n");
        }
    }

    return 0;
}

