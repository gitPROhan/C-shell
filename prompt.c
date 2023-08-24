#include "headers.h"

void prompt(char *home, char *process)
{
    char hostname[100];
    gethostname(hostname, 100);
    char dirname[100];
    getcwd(dirname, 100);
    struct passwd *username = getpwuid(getuid());
    printf("<%s@%s:", username->pw_name, hostname);
    if (strncmp(home, dirname, strlen(home)) == 0)
    {
        printf("~");
        char remaining_name[100];
        strncpy(remaining_name, dirname + strlen(home), 100);
        printf("%s", remaining_name);
    }
    else
        printf("%s", dirname);
    if (strlen(process) != 0)
        printf("%s", process);
    printf(">");
}
