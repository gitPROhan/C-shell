#include "headers.h"

void tokenize(char *input, char *process, int flag, ll *bgp)
{
    char *token;
    char *instruc[100];
    int c = 0;
    token = strtok(input, ";\n");
    while (token != NULL)
    {
        instruc[c++] = token;
        token = strtok(NULL, ";\n");
    }
    instruc[c] = NULL;
    if (instruc[c - 1][strlen(instruc[c - 1]) - 1] == '&')
        flag = 1;
    int i = 0;
    while (i < c)
    {
        char *individual_instruc[200];
        char *token2;
        int j = 0;
        token2 = strtok(instruc[i], "&\n");
        while (token2 != NULL)
        {
            individual_instruc[j++] = token2;
            token2 = strtok(NULL, "&\n");
        }
        individual_instruc[j] = NULL;
        for (int k = 0; k < j; k++)
        {
            char *instr[200];
            char *token3;
            int l = 0;
            token3 = strtok(individual_instruc[k], " \n");
            while (token3 != NULL)
            {
                instr[l++] = token3;
                token3 = strtok(NULL, " \n");
            }
            instr[l] = NULL;
            syscom(instr, l, process, flag, k, j, bgp);
        }
        i++;
    }
}