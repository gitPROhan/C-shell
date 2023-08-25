#include "headers.h"

int comparator(const void *a, const void *b)
{
    return strcasecmp(*(const char **)a, *(const char **)b);
}

void with_details(const char *path, const struct stat *stat_info)
{
    struct passwd *user_info = getpwuid(stat_info->st_uid);
    struct group *group_info = getgrgid(stat_info->st_gid);

    printf("%s", (S_ISDIR(stat_info->st_mode)) ? "d" : "-");
    printf((stat_info->st_mode & S_IRUSR) ? "r" : "-");
    printf((stat_info->st_mode & S_IWUSR) ? "w" : "-");
    printf((stat_info->st_mode & S_IXUSR) ? "x" : "-");
    printf((stat_info->st_mode & S_IRGRP) ? "r" : "-");
    printf((stat_info->st_mode & S_IWGRP) ? "w" : "-");
    printf((stat_info->st_mode & S_IXGRP) ? "x" : "-");
    printf((stat_info->st_mode & S_IROTH) ? "r" : "-");
    printf((stat_info->st_mode & S_IWOTH) ? "w" : "-");
    printf((stat_info->st_mode & S_IXOTH) ? "x" : "-");

    printf(" %2lu ", stat_info->st_nlink);
    printf("%s %s ", user_info->pw_name, group_info->gr_name);
    printf("%6lld ", (long long)stat_info->st_size);

    char time_buff[20];
    strftime(time_buff, sizeof(time_buff), "%b %d %H:%M", localtime(&stat_info->st_mtime));
    printf("%s ", time_buff);
}

void peek(char **full_peek_instr, int c, char *old_wd, char *home)
{
    char path[100];
    int hidden = 0;
    int details = 0;
    if (c == 4)
    {
        hidden = 1;
        details = 1;
        strcpy(path, full_peek_instr[c - 1]);
    }
    else if (c == 3)
    {
        if (strcmp(full_peek_instr[2], "-l") == 0 || strcmp(full_peek_instr[2], "-a") == 0)
        {
            details = 1;
            hidden = 1;
            char cwd[100];
            getcwd(cwd, 100);
            strcpy(path, cwd);
        }
        else if (strcmp(full_peek_instr[1], "-al") == 0 || strcmp(full_peek_instr[1], "-la") == 0)
        {
            hidden = 1;
            details = 1;
            strcpy(path, full_peek_instr[2]);
        }
        else if (strcmp(full_peek_instr[1], "-l") == 0)
        {
            details = 1;
            strcpy(path, full_peek_instr[2]);
        }
        else if (strcmp(full_peek_instr[1], "-a") == 0)
        {
            hidden = 1;
            strcpy(path, full_peek_instr[2]);
        }
    }
    else if (c == 2)
    {
        if (strcmp(full_peek_instr[c - 1], "-l") == 0)
        {
            details = 1;
            char cwd[100];
            getcwd(cwd, 100);
            strcpy(path, cwd);
        }
        else if (strcmp(full_peek_instr[c - 1], "-a") == 0)
        {
            hidden = 1;
            char cwd[100];
            getcwd(cwd, 100);
            strcpy(path, cwd);
        }
        else if ((strcmp(full_peek_instr[c - 1], "-al") == 0) || (strcmp(full_peek_instr[c - 1], "-la") == 0))
        {
            details = 1;
            hidden = 1;
            char cwd[100];
            getcwd(cwd, 100);
            strcpy(path, cwd);
        }
        else
            strcpy(path, full_peek_instr[c - 1]);
    }
    else
    {
        char cwd[100];
        getcwd(cwd, 100);
        strcpy(path, cwd);
    }
    if (path[0] == '-')
    {
        char new_path[200];
        snprintf(new_path, 200, "%s%s", old_wd, path + 1);
        strcpy(path, new_path);
    }
    else if (path[0] == '~')
    {
        char new_path[200];
        snprintf(new_path, 200, "%s%s", home, path + 1);
        strcpy(path, new_path);
    }

    DIR *dir = opendir(path);
    if (!dir)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    char **file_instances = NULL;
    int num_file_instances = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (!hidden && entry->d_name[0] == '.')
            continue;
        char entry_path[600];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);
        struct stat stat_info;
        if (lstat(entry_path, &stat_info) == -1)
        {
            perror("lstat");
            continue;
        }
        if (!file_instances)
            file_instances = (char **)malloc(sizeof(char *));
        else
            file_instances = (char **)realloc(file_instances, (num_file_instances + 1) * sizeof(char *));
        file_instances[num_file_instances] = strdup(entry->d_name);
        num_file_instances++;
    }

    qsort(file_instances, num_file_instances, sizeof(char *), comparator);

    // if (details)
    // {
    //     struct stat dir_stat;
    //     if (stat(path, &dir_stat) == -1)
    //         perror("stat");
    //     off_t total_block_size = dir_stat.st_blocks * 512;
    //     printf("total %ld\n", total_block_size);
    // }

    for (int i = 0; i < num_file_instances; i++)
    {
        char entry_path[600];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, file_instances[i]);
        struct stat stat_info;
        if (lstat(entry_path, &stat_info) == -1)
        {
            perror("lstat");
            continue;
        }
        if (details)
            with_details(entry_path, &stat_info);
        if (S_ISDIR(stat_info.st_mode))
            printf(BLUE "%s" RESET, file_instances[i]);
        else if (stat_info.st_mode & S_IXUSR)
            printf(GREEN "%s" RESET, file_instances[i]);
        else
            printf(WHITE "%s" RESET, file_instances[i]);
        if (details)
            printf("\n");
        else
            printf("  ");
        free(file_instances[i]);
    }
    if (!details)
        printf("\n");
    free(file_instances);
    closedir(dir);
}