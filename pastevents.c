#include "headers.h"

void pastevents(char **instr, int l, char *home, ll *bgp)
{
    char path_of_history[100];
    snprintf(path_of_history, 100, "%s/history.txt", home);
    if (strcmp(instr[0], "pastevents") == 0)
    {
        if (l > 1 && strcmp(instr[1], "purge") == 0)
        {
            FILE *ptr;
            ptr = fopen(path_of_history, "w");
            fclose(ptr);
        }
        else if (l > 2 && strcmp(instr[1], "execute") == 0)
        {
            int line_number = atoi(instr[2]);
            FILE *ptr;
            ptr = fopen(path_of_history, "r");
            char buff[100];
            int x = 0;
            while (fgets(buff, 100, ptr) != NULL)
            {
                x++;
            }
            fclose(ptr);
            if (line_number > x)
            {
                printf("Invalid line number!\n");
                return;
            }
            line_number = x + 1 - line_number;
            ptr = fopen(path_of_history, "r");
            char str[100];
            int c = 1;
            while (c != line_number + 1 && fgets(str, 100, ptr) != NULL)
            {
                c++;
            }
            fclose(ptr);
            tokenize(str, NULL, bgp, home);
        }
        else
        {
            FILE *ptr;
            ptr = fopen(path_of_history, "r");
            char str[100];
            while (fgets(str, 100, ptr) != NULL)
            {
                printf("%s", str);
            }
            fclose(ptr);
        }
    }
    else
    {
        char next_command[100] = "\0";
        int i = 0;
        while (i < l)
        {
            strcat(next_command, instr[i]);
            strcat(next_command, " ");
            i++;
        }
        strcat(next_command, "\n");
        char **events = (char **)malloc(sizeof(char *) * 16);
        for (int i = 0; i < 16; i++)
        {
            events[i] = (char *)malloc(sizeof(char) * 100);
        }
        FILE *ptr;
        ptr = fopen(path_of_history, "r");
        char str[100];
        int c = 0;
        while (fgets(str, 100, ptr) != NULL)
        {
            strcpy(events[c++], str);
        }
        fclose(ptr);
        if (c == 0 || strncmp(next_command, events[c - 1], strlen(events[c - 1])) != 0)
        {
            if (c >= 15)
            {
                ptr = fopen(path_of_history, "w");
                fclose(ptr);
                strcpy(events[c], next_command);
                ptr = fopen(path_of_history, "w");
                for (int i = 1; i <= c; i++)
                {
                    fprintf(ptr, "%s", events[i]);
                }
                fclose(ptr);
            }
            else
            {
                ptr = fopen(path_of_history, "a");
                fprintf(ptr, "%s", next_command);
                fclose(ptr);
            }
        }
    }
}