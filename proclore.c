#include "headers.h"

void proclore(char *input)
{
    char *token;
    char *command[100];
    token = strtok(input, " \n");
    int c = 0;
    while (token != NULL)
    {
        command[c++] = token;
        token = strtok(NULL, " \n");
    }
    command[c] = NULL;
    pid_t pid;
    if (c == 1)
        pid = getpid();
    else
        pid = atoi(command[1]);
    char path[100];
    snprintf(path, 100, "/proc/%d/status", pid);
    FILE *ptr = fopen(path, "r");
    int processGroup, virtualMemory;
    char executablePath[100], state;
    while (fscanf(ptr, "State:\t%c\n", &state) != EOF)
    {
        fscanf(ptr, "VmSize:\t%d kB\n", &virtualMemory);
        fscanf(ptr, "PPid:\t%d\n", &processGroup);
        fgets(executablePath, sizeof(executablePath), ptr);
    }
    fclose(ptr);
    snprintf(path, 100, "/proc/%d/exe", pid);
    ssize_t len = readlink(path, executablePath, sizeof(executablePath) - 1);
    if (len != -1)
        executablePath[len] = '\0';
    else
        strcpy(executablePath, "Unknown");
    printf("pid : %d\n", pid);
    printf("process status : %c", state);
    int cur_pid = getpid();
    printf((cur_pid == pid) ? "+\n" : "\n");
    printf("Process Group : %d\n", processGroup);
    printf("Virtual memory : %d\n", virtualMemory);
    printf("executable path : %s\n", executablePath);
}