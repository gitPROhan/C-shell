#include "headers.h"

void syscom(char **instr, int l, char *process, int flag, int k, int j, char *prev_background_processes)
{
    if (strcmp(instr[0], "pastevents") == 0)
        pastevents(instr, l);
    else if (k == j - 1 && flag == 0)
    {
        pastevents(instr, l);
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
        pastevents(instr, l);
        pid_t pid = fork();
        if (pid == 0)
            execvp(instr[0], instr);
        else
        {
            printf("%d\n", pid);
            int status;
            char back_proc[100];
            waitpid(pid, &status, WNOHANG);
            if (WIFEXITED(status))
                snprintf(back_proc, 100, "%s exited normally (%d)\n", instr[0], pid);
            else if (WIFSIGNALED(status))
                snprintf(back_proc, 100, "%s exited abnormally (%d)\n", instr[0], pid);
            strcat(prev_background_processes, back_proc);
        }
    }
}