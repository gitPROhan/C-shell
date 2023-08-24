#include "headers.h"

void warp(char *path_input, char *home, char *old_wd)
{
    if (path_input[0] == '-')
    {
        char new_path[200];
        snprintf(new_path, 200, "%s%s", old_wd, path_input + 1);
        getcwd(old_wd, 200);
        chdir(new_path);
    }
    else if (path_input[0] == '~')
    {
        char new_path[200];
        snprintf(new_path, 200, "%s%s", home, path_input + 1);
        getcwd(old_wd, 200);
        chdir(new_path);
    }
    else
    {
        getcwd(old_wd, 200);
        chdir(path_input);
    }
    char cwd[200];
    getcwd(cwd, 200);
    printf("%s\n", cwd);
}