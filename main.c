#include "thunt.h"
#include <dirent.h>

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

void add(char *id)
{
    DIR *dir = opendir(id);
    char newpath[100];
    char OGlog[150];
    char logfile[150];
    char target[500];

    sprintf(OGlog, "logging-file-%s.txt", id);
    int new = 0;
    if(!dir) {
        new = 1;
        sprintf(newpath,"./%s",id);
        sprintf(logfile, "%s/logging-file.txt", newpath);
        #ifdef __linux__
            int dirok= mkdir(name, 777); /* Or what parameter you need here ... */
        #else
            int dirok= mkdir(newpath);
        #endif
        if( dirok < 0) {
            perror("Could not create hunt\n");
            return;
        }
    }

    sprintf(logfile, "%s/logging-file.txt", id);
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    sprintf(target, "%s/%s", cwd, logfile);
    FILE *log = fopen(logfile, "at");
    if(!log) {
        perror("Logfile could not open\n");
        return;
    }

    if(new) {
        dir = opendir(newpath);
        if(!dir) {
            perror("Current direcotry could not be opened\n");
            return;
        }
        if(CreateSymbolicLinkA(logfile, OGlog, 0) < 0) {
            perror("Symlink error");
            fclose(log);
            return;
        }
    }

    char filepath[150];
    if(new) {
        sprintf(filepath, "%s/%s", newpath, "treasures");
    } else {
        sprintf(filepath, "%s/treasures", id);    
    }
    FILE *f = fopen(filepath, "ab+");
    if(!f) {
        perror("eroare la deschiderea sau crearea fisierului treasures\n");
        return;
    }
    Treasure treasure = create();
    if(fwrite(&treasure, sizeof(Treasure), 1, f) != 1) {
        perror("Error reading from file\n");
        return;
    }
    fprintf(log, "Treasure %d added\n", treasure.TreasureId);

    if(closedir(dir) < 0) {
        perror("Error closind direcotory\n");
        return;
    }
    if(fclose(f)) {
        perror("Error closing file\n");
        return;
    }
    if(fclose(log)) {
        perror("Error closing log file\n");
        return;
    }
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
        perror("Logfile could not open\n");
        return;
    }
    FILE *f = fopen(fp, "rb");
    if(!f) {
        perror("Hunt not made\n");
        return;
    }
    Treasure treasure;
    printf("The list of tresures %s:\n", id);
    while(fread(&treasure, sizeof(Treasure), 1, f)) {
        printTreasure(&treasure);
    }
    fprintf(log, "Hunt %s shown.\n", id);

    if(fclose(f)) {
        perror("Error closing file\n");
        return;
    }
    if(fclose(log)) {
        perror("Error closing log file\n");
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
        perror("Logfile could not open\n");
        return;
    }
    FILE *f = fopen(fp, "rb");
    if(!f) {
        perror("Hunt not made\n");
        return;
    }
    Treasure treasure;
    while(fread(&treasure, sizeof(Treasure), 1, f)) {
        if(treasure.TreasureId == TreasureId) {
            printf("Treasure %d found: \n", TreasureId);
            printTreasure(&treasure);
            OK = 1;
            break;
        }
    }

    if(OK)
    {
        fprintf(log, "Treasure %d found\n", TreasureId);
    }
    else
    {
        fprintf(log, "Treasure %d not found\n", TreasureId);
        printf("Treasure %d not found\n", TreasureId);
    }

    if(fclose(f)) {
        perror("Error closing file\n");
        return;
    }
    if(fclose(log)) {
        perror("Error closing log file\n");
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