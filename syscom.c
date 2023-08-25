#include "headers.h"

void syscom(char **instr, int l, char *process, ll *bgp, int flag, char *home)
{
    if (strcmp(instr[0], "pastevents") == 0)
        pastevents(instr, l, home, bgp);
    else if (flag == 1)
    {
        pastevents(instr, l, home, bgp);
        pid_t pid = fork();
        if (pid == 0)
            execvp(instr[0], instr);
        else
        {
            struct timeval start_time, end_time;
            gettimeofday(&start_time, NULL);
            int status;
            waitpid(pid, &status, 0);
            gettimeofday(&end_time, NULL);
            double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                                  (end_time.tv_usec - start_time.tv_usec) / 1e6;
            if (elapsed_time > 2.0)
            {
                int final_time = (int)(elapsed_time);
                snprintf(process, 200, " %s : %ds", instr[0], final_time);
            }
        }
    }
    else
    {
        pastevents(instr, l, home, bgp);
        pid_t pid = fork();
        if (pid == 0)
        {
            int ret = execvp(instr[0], instr);
            if (ret == -1)
                printf("%s exited abnormally.\n", instr[0]);
        }
        else
        {
            printf("%d\n", pid);
            ll newnode = (ll)malloc(sizeof(node));
            newnode->name = (char *)malloc(sizeof(char) * 100);
            strcpy(newnode->name, instr[0]);
            newnode->pid = pid;
            newnode->next = *bgp;
            *bgp = newnode;
        }
    }
}