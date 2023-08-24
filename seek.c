#include "headers.h"

void handle_single_result(const char *path, int flags)
{
    struct stat st;
    if (stat(path, &st) == 0)
    {
        if ((flags & 0x01) && S_ISDIR(st.st_mode))
        {
            if (flags & 0x08)
            {
                if (access(path, X_OK) == 0)
                {
                    chdir(path);
                    printf("Changed current directory to: %s\n", path);
                }
                else
                {
                    printf("Missing permissions for task!\n");
                }
            }
            else
            {
                printf("%s/\n", path);
            }
        }
        else if ((flags & 0x02) && S_ISREG(st.st_mode))
        {
            if (flags & 0x08)
            {
                FILE *file = fopen(path, "r");
                if (file)
                {
                    char buffer[256];
                    while (fgets(buffer, sizeof(buffer), file))
                    {
                        printf("%s", buffer);
                    }
                    fclose(file);
                }
                else
                {
                    printf("Missing permissions for task!\n");
                }
            }
            else
            {
                printf("%s\n", path);
            }
        }
    }
}

void seek(const char *args)
{
    int flags = 0;
    const char *search = NULL;
    const char *target_dir = NULL;

    // Parse flags
    if (args[0] == '-')
    {
        for (int i = 1; args[i] != ' ' && args[i] != '\0'; i++)
        {
            if (args[i] == 'd')
            {
                flags |= 0x01;
            }
            else if (args[i] == 'f')
            {
                flags |= 0x02;
            }
            else if (args[i] == 'e')
            {
                flags |= 0x08;
            }
            else
            {
                printf("Invalid flags!\n");
                return;
            }
        }
    }

    // Parse search and target directory
    char *arg_copy = strdup(args);
    char *token = strtok(arg_copy, " ");
    if (token)
    {
        search = token;
        token = strtok(NULL, " ");
        if (token)
        {
            target_dir = token;
        }
    }
    free(arg_copy);

    // Set target directory if not provided
    if (!target_dir)
    {
        target_dir = ".";
    }

    // Open target directory
    DIR *dir = opendir(target_dir);
    if (!dir)
    {
        printf("No match found!\n");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
            strstr(entry->d_name, search) != NULL)
        {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", target_dir, entry->d_name);
            handle_single_result(path, flags);
        }
    }

    closedir(dir);
}
