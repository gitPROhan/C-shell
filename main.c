#include "headers.h"

int main()
{
    char home[100];
    getcwd(home, 100);
    char process[100] = "\0";
    char prev_background_processes[200] = "\0";
    char old_wd[100];
    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(home, process);
        process[0] = '\0';
        int flag = 0;
        char input[4096];
        fgets(input, 4096, stdin);
        if (strncmp(input, "exit", 4) == 0)
        {
            FILE *ptr;
            ptr = fopen("history.txt", "a");
            fprintf(ptr, "exit\n");
            fclose(ptr);
            break;
        }
        if (strlen(prev_background_processes) != 0)
            printf("%s", prev_background_processes);
        prev_background_processes[0] = '\0';
        if (strncmp("warp", input, 4) == 0)
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
            for (int i = 1; i < c; i++)
            {
                warp(instruc[i], home, old_wd);
            }
        }
        else if (strncmp("peek", input, 4) == 0)
        {
            char *token;
            char *full_peek_instr[5];
            int c = 0;
            token = strtok(input, " \n");
            while (token != NULL)
            {
                full_peek_instr[c++] = token;
                token = strtok(NULL, " \n");
            }
            full_peek_instr[c] = NULL;
            peek(full_peek_instr, c, old_wd, home);
        }
        else if (strncmp(input, "proclore", 8) == 0)
            proclore(input);
        else if (strncmp(input, "seek", 4) == 0)
            seek(input);
        else
            tokenize(input, process, flag, prev_background_processes);
    }
}