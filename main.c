#include "headers.h"

int main()
{
    char home[100];
    getcwd(home, 100);
    char process[100] = "\0";
    char old_wd[100];
    ll bgp = (ll)malloc(sizeof(node));
    bgp->name = (char *)malloc(sizeof(char) * 100);
    while (1)
    {
        prompt(home, process);
        process[0] = '\0';
        char input[4096];
        fgets(input, 4096, stdin);
        int status;
        while (1)
        {
            int pid = waitpid(-1, &status, WNOHANG);
            char name[100];
            if (pid > 0)
            {
                ll curr = bgp;
                ll prev = NULL;
                while (curr != NULL)
                {
                    if (curr->pid == pid)
                    {
                        strcpy(name, curr->name);
                        if (prev != NULL)
                        {
                            prev->next = curr->next;
                            ll delete = curr;
                            free(delete);
                        }
                        else
                        {
                            ll delete = curr;
                            bgp = curr->next;
                            free(delete);
                        }
                        break;
                    }
                    else
                    {
                        curr = curr->next;
                        prev = curr;
                    }
                }
                printf("%s exited normally (%d)\n", name, pid);
            }
            else
                break;
        }
        if (strcmp(input, "\n") == 0)
            continue;
        int ns = 0;
        while (input[ns] == ' ')
            ns++;
        if (strncmp("exit", input + ns, 4) == 0)
        {
            char **arr = (char **)malloc(sizeof(char *) * 1);
            arr[0] = (char *)malloc(sizeof(char) * 5);
            strcpy(arr[0], "exit");
            pastevents(arr, 1, home, &bgp);
            break;
        }
        else if (strncmp("warp", input + ns, 4) == 0)
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
            pastevents(instruc, c, home, &bgp);
        }
        else if (strncmp("peek", input + ns, 4) == 0)
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
            pastevents(full_peek_instr, c, home, &bgp);
        }
        else if (strncmp("proclore", input + ns, 8) == 0)
        {
            proclore(input);
            char *token;
            char *instruc[10];
            int c = 0;
            token = strtok(input, " \n");
            while (token != NULL)
            {
                instruc[c++] = token;
                token = strtok(NULL, " \n");
            }
            instruc[c] = NULL;
            pastevents(instruc, c, home, &bgp);
        }
        else if (strncmp("seek", input + ns, 4) == 0)
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
            seek(instruc, c, home);
            pastevents(instruc, c, home, &bgp);
        }
        else
            tokenize(input, process, &bgp, home);
    }
}