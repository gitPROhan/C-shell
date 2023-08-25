#include "headers.h"

void find(int target_path_length, char *path, char *target, int file, int directory, int *c, char *execute_path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
        return;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        char child_path[512];
        snprintf(child_path, 600, "%s/%s", path, entry->d_name);
        struct stat st;
        if (stat(child_path, &st) == -1)
            continue;
        if (S_ISDIR(st.st_mode))
        {
            if (strcmp(entry->d_name, target) == 0 && directory)
            {
                char found_path[100];
                snprintf(found_path, 100, "%s", child_path + target_path_length);
                printf(BLUE ".%s\n" RESET, found_path);
                *c += 1;
                strcpy(execute_path, child_path);
            }
            find(target_path_length, child_path, target, file, directory, c, execute_path);
        }
        else if (S_ISREG(st.st_mode) && file)
        {
            if (strncmp(entry->d_name, target, strlen(target)) == 0)
            {
                char found_path[100];
                snprintf(found_path, 100, "%s", child_path + target_path_length);
                printf(GREEN ".%s\n" RESET, found_path);
                strcpy(execute_path, child_path);
                *c += 1;
            }
        }
    }
    closedir(dir);
}

void seek(char **instruc, int len, char *home)
{
    int file = 0;
    int directory = 0;
    int execute = 0;
    int target_recieved = 0;
    char target[100];
    char path[200] = "\0";
    for (int i = 1; i < len; i++)
    {
        int flag = 0;
        if (strcmp(instruc[i], "-f") == 0)
            file = 1;
        else if (strcmp(instruc[i], "-d") == 0)
            directory = 1;
        else if (strcmp(instruc[i], "-e") == 0)
            execute = 1;
        else
        {
            if (target_recieved == 0)
            {
                target_recieved = 1;
                strcpy(target, instruc[i]);
            }
            else
                strcpy(path, instruc[i]);
        }
    }
    if (strlen(path) == 0)
        getcwd(path, 200);
    if (file == 1 && directory == 1)
    {
        printf("Invalid flags!\n");
        return;
    }
    if (!file && !directory)
    {
        file = 1;
        directory = 1;
    }
    int c = 0;
    char execute_path[200];
    if (path)
        find(strlen(path), path, target, file, directory, &c, execute_path);
    if (c == 0)
        printf("No match found!\n");
    else if (c == 1 && execute == 1)
    {
        struct stat st;
        if (stat(execute_path, &st) == -1)
        {
            printf("Missing permissions for task!\n");
            return;
        }
        if (S_ISDIR(st.st_mode))
            chdir(execute_path);
        else if (S_ISREG(st.st_mode))
        {
            FILE *ptr;
            ptr = fopen(execute_path, "r");
            if (ptr == NULL)
            {
                printf("Missing permissions for task!\n");
                return;
            }
            char str[100];
            while (fgets(str, 100, ptr) != NULL)
            {
                printf("%s", str);
            }
            fclose(ptr);
        }
    }
}
