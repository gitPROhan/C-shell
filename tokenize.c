#include "headers.h"

void tokenize(char *input, char *process, ll *bgp, char *home)
{
    char *token;
    char *instruc[100];
    int c = 0;
    token = strtok(input, " \n");
    while (token != NULL)
    {
        instruc[c++] = token;
        token = strtok(NULL, " \n");
    }
    instruc[c] = NULL;
    int arr_it = 0;
    char **command = (char **)malloc(sizeof(char *) * 50);
    for (int i = 0; i < 50; i++)
    {
        command[i] = (char *)malloc(sizeof(char) * 20);
    }
    int cmd_it = 0;
    while (arr_it < c)
    {
        if (strcmp(instruc[arr_it], "&") == 0)
        {
            command[cmd_it] = NULL;
            syscom(command, cmd_it, process, bgp, 0, home);
            command[cmd_it] = (char *)malloc(sizeof(char) * 20);
            cmd_it = 0;
        }
        else if (strcmp(instruc[arr_it], ";") == 0)
        {
            command[cmd_it] = NULL;
            syscom(command, cmd_it, process, bgp, 1, home);
            command[cmd_it] = (char *)malloc(sizeof(char) * 20);
            cmd_it = 0;
        }
        else
        {
            if (arr_it == c - 1)
            {
                strcpy(command[cmd_it], instruc[arr_it]);
                cmd_it++;
                command[cmd_it] = NULL;
                syscom(command, cmd_it, process, bgp, 1, home);
                command[cmd_it] = (char *)malloc(sizeof(char) * 20);
                cmd_it = 0;
            }
            else
            {
                strcpy(command[cmd_it], instruc[arr_it]);
                cmd_it++;
            }
        }
        arr_it++;
    }
}