#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void add(char *id);

void list(char *id);

void view(char *id, int TreasureId);

void remove_treasure(char *id, int TreasureId);

void remove_hunt (char *id);