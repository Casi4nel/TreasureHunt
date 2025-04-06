#include "thunt.h"

typedef struct gps{
    float longitude;
    float latitude;
}gps;

typedef struct Treasure{
    int TreasureId;
    char user[50];
    gps coordonates;
    char clue[100];
    int value;
}Treasure;

Treasure create() {
    printf("Enter id:\n");
    Treasure treasure;
    scanf("%d", &treasure.TreasureId);
    printf("Enter username:\n");
    scanf("%s", treasure.user);
    printf("Enter coordonates:\n");
    scanf("%f", &treasure.coordonates.longitude);
    scanf("%f", &treasure.coordonates.latitude);
    printf("Enter clue:\n");
    scanf("%s", treasure.clue);
    printf("Enter value:\n");
    scanf("%d", &treasure.value);
    return treasure;
}

void getter(char *cwd, char *id)
{
    getcwd(cwd, 100);
    strcat(cwd, "/");
    strcat(cwd, id);
}

void add(char *id)
{

}

void printTreasure(Treasure *treasure) {
    printf("id: %d\n", treasure->TreasureId);
    printf("username: %s\n", treasure->user);
    printf("longitude and latitude: (%f , %f)\n", treasure->coordonates.longitude,treasure->coordonates.latitude);
    printf("clue: %s\n",treasure->clue);
    printf("value: %d\n\n", treasure->value);

}

void list(char *id) {
    char fp[150];
    char logfile[150];
    sprintf(fp, "%s/treasures", id);
    sprintf(logfile, "%s/logfile.txt", id);
    FILE *log= fopen(logfile, "at");
    if(!log) {
        perror("Logfile could not open");
        return;
    }
    FILE *f = fopen(fp, "rb");
    if(!f) {
        perror("Hunt not made");
        return;
    }
    Treasure treasure;
    printf("The list of tresures %s:\n", id);
    while(fread(&treasure, sizeof(Treasure), 1, f)) {
        printTreasure(&treasure);
    }
    fprintf(log, "Hunt %s shown.\n", id);

    if(fclose(f)) {
        perror("Error closing file");
        return;
    }
    if(fclose(log)) {
        perror("Error closing log file");
        return;
    }
}

void view(char *id,int TreasureId)
{
    char fp[150];
    char logfile[150];
    int OK=0;
    sprintf(fp, "%s/treasures", id);
    sprintf(logfile, "%s/logfile.txt", id);
    FILE *log= fopen(logfile, "at");
    if(!log) {
        perror("Logfile could not open");
        return;
    }
    FILE *f = fopen(fp, "rb");
    if(!f) {
        perror("Hunt not made");
        return;
    }
    Treasure treasure;
    while(fread(&treasure, sizeof(Treasure), 1, f)) {
        if(treasure.TreasureId == TreasureId) {
            printTreasure(&treasure);
            OK = 1;
            break;
        }
    }

    if(OK)
    {
        fprintf(log, "%d treasure found", TreasureId);
    }
    else
    {
        fprintf(log, "%d treasure not found", TreasureId);
        printf("%d treasure not found", TreasureId);
    }

    if(fclose(f)) {
        perror("Error closing file");
        return;
    }
    if(fclose(log)) {
        perror("Error closing log file");
        return;
    }

}

int main(int argc,char **argv) {
    if(argc < 2) {
        printf("Utilisation: %s <command> [<parameter>]\n", argv[0]);
        return 1;
    }
    if(!strcmp(argv[1], "add")) {
        if(argc < 3) {
            printf("Utilisation: %s add <hunt>\n", argv[0]);
            return 1;
        }
        add(argv[2]);
    } else if(!strcmp(argv[1], "list")) {
        if(argc < 3) {
            printf("Utilisation: %s list <hunt>\n", argv[0]);
            return 1;
        }
        list(argv[2]);
    } else if(!strcmp(argv[1], "view")) {
        if(argc < 4) {
            printf("Utilisation: %s view <hunt> <id>\n", argv[0]);
            return 1;
        }
        view(argv[2], atoi(argv[3]));
    } else {
        printf("Unknown command: %s\n", argv[1]);
        return 1;
    }
    return 0;
}