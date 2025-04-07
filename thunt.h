#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdio.h>   
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>

void add(char *id);

void list(char *id);

void view(char *id, int TreasureId);

void remove_treasure(char *id, int TreasureId);

void remove_hunt (char *id);