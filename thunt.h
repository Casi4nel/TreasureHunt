#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>

#define MAXSIZE 20

void add(char *id);

void list(char *id);

void view(char *id, int TreasureId);

void remove_treasure(char *id, int TreasureId);

void remove_hunt (char *id);