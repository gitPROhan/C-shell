#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <math.h>
#include <dirent.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>

#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define WHITE "\x1b[37m"
#define RESET "\x1b[0m"

#include "prompt.h"
#include "tokenize.h"
#include "warp.h"
#include "peek.h"
#include "patsevents.h"
#include "syscom.h"
#include "proclore.h"
#include "seek.h"

typedef struct node
{
    char *name;
    int pid;
    struct node *next;
} node;

typedef node *ll;

#endif