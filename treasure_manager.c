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

Treasure create() 
{
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
    char OGlog[200];
    char logfile[200];
    char target[500];

    sprintf(OGlog, "logging_file-%s.txt", id);
    int new = 0;
    if(!dir) 
    {
        new = 1;
        sprintf(newpath,"./%s",id);
        sprintf(logfile, "%s/logging_file.txt", newpath);
        int dir= mkdir(newpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if( dir < 0) 
        {
            perror("Could not create hunt\n");
            return;
        }
    }

    sprintf(logfile, "%s/logging_file.txt", id);
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    sprintf(target, "%s/%s", cwd, logfile);
    FILE *log = fopen(logfile, "at");
    if(!log) 
    {
        perror("Logfile could not open\n");
        return;
    }
    if(new) 
    {
        dir = opendir(newpath);
        if(!dir) 
        {
            perror("Current directory could not be opened\n");
            return;
        }

        if(symlink(logfile, OGlog) < 0 ) 
        {
            perror("Symlink error");
            fclose(log);
            return;
        }
    }

    char fp[200];
    if(new) 
    {
        sprintf(fp, "%s/%s", newpath, "treasures");
    } 
    else 
    {
        sprintf(fp, "%s/treasures", id);    
    }
    FILE *f = fopen(fp, "ab+");
    if(!f) 
    {
        perror("Error creating file\n");
        return;
    }
    Treasure treasure = create();
    if(fwrite(&treasure, sizeof(Treasure), 1, f) != 1) 
    {
        perror("Error writing in file\n");
        return;
    }
    fprintf(log, "Treasure %d added\n", treasure.TreasureId);

    if(closedir(dir) < 0) 
    {
        perror("Error closind direcotory\n");
        return;
    }
    if(fclose(f)) 
    {
        perror("Error closing file\n");
        return;
    }
    if(fclose(log)) 
    {
        perror("Error closing log file\n");
        return;
    }
}

void printTreasure(Treasure *treasure) 
{
    printf("id: %d\n", treasure->TreasureId);
    printf("username: %s\n", treasure->user);
    printf("longitude and latitude: (%f , %f)\n", treasure->coordonates.longitude,treasure->coordonates.latitude);
    printf("clue: %s\n",treasure->clue);
    printf("value: %d\n\n", treasure->value);

}

void list(char *id) 
{
    char fp[200];
    char logfile[200];
    sprintf(fp, "%s/treasures", id);
    sprintf(logfile, "%s/logging_file.txt", id);
    FILE *log= fopen(logfile, "at");

    if(!log) 
    {
        perror("Logfile could not open\n");
        return;
    }
    FILE *f = fopen(fp, "rb");
    if(!f) 
    {
        perror("Hunt not made\n");
        return;
    }
    Treasure treasure;
    printf("The list of tresures for %s:\n", id);
    while(fread(&treasure, sizeof(Treasure), 1, f)) 
    {
        printTreasure(&treasure);
    }
    fprintf(log, "Hunt %s shown.\n", id);

    if(fclose(f)) 
    {
        perror("Error closing file\n");
        return;
    }
    if(fclose(log)) 
    {
        perror("Error closing log file\n");
        return;
    }
}

void view(char *id,int TreasureId)
{
    char fp[200];
    char logfile[200];
    int OK=0;
    sprintf(fp, "%s/treasures", id);
    sprintf(logfile, "%s/logging_file.txt", id);
    FILE *log= fopen(logfile, "at");

    if(!log) 
    {
        perror("Logfile could not open\n");
        return;
    }
    FILE *f = fopen(fp, "rb");
    if(!f) 
    {
        perror("Hunt not made\n");
        return;
    }
    Treasure treasure;
    while(fread(&treasure, sizeof(Treasure), 1, f)) 
    {
        if(treasure.TreasureId == TreasureId) 
        {
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

    if(fclose(f)) 
    {
        perror("Error closing file\n");
        return;
    }
    if(fclose(log)) 
    {
        perror("Error closing log file\n");
        return;
    }

}

void remove_treasure(char *id, int TreasureId)
{
    char logfile[200];
    char fp[200];
    char tmp[200];
    Treasure treasure;
    int OK = 0;
    sprintf(logfile, "%s/logging_file.txt", id);
    sprintf(fp, "%s/treasures", id);

    FILE *log = fopen(logfile, "at");
    if(!log) 
    {
        perror("Could not open log file");
        return;
    }
    FILE *f = fopen(fp, "rb+");
    if(!f) 
    {
        perror("Could not open file");
        return;
    }
    sprintf(tmp, "%s/tmp.txt", id);
    FILE *fis = fopen(tmp, "wb+");
    if(!fis) 
    {
        perror("Error opening file");
        fclose(f);
        return;
    }

    while(fread(&treasure, sizeof(Treasure), 1, f)) 
    {
        if(treasure.TreasureId == TreasureId) 
        {
            OK = 1;
            continue;
        }
        fwrite(&treasure, sizeof(Treasure), 1, fis);
    }
    if(!OK) 
    {
        printf("Treasure %d does not exist\n", TreasureId);
        fprintf(log, "Treasure %d does not exist\n", TreasureId);
        remove("fis");
        return;
    }
    else
    {
        fprintf(log, "Treasure %d was deleted\n", TreasureId);
    }

    if(fclose(f)) 
    {
        perror("Error closing file");
        fclose(fis);
        return;
    }
    if(fclose(fis)) 
    {
        perror("Error closing file");
        return;
    }
    if(fclose(log)) 
    {
        perror("Error colsing log file");
        return;
    }
    if(remove(fp) < 0) 
    {
        perror("Error removing file");
        return;
    }
    if(rename(tmp, fp) < 0) 
    {
        perror("Error renaming file");
        return;
    }
}

void remove_hunt(char *id)
{
    char fp[200];
    char logfile[200];
    char OGlog[200];
    sprintf(fp, "%s/treasures", id);
    sprintf(logfile, "%s/logging_file.txt", id);
    sprintf(OGlog, "logging_file-%s.txt", id);
    int unlk=unlink(OGlog);

    if(unlk < 0) 
    {
        perror("Symbloic link could not be broken");
        return;
    }
    if(remove(fp) < 0) 
    {
        perror("Error deleting file");
        return;
    }
    if(remove(logfile) < 0) 
    {
        perror("Error deleting log file");
        return;
    }
    if(rmdir(id) < 0) 
    {
        perror("Error deleting directory");
        return;
    }
}

int main(int argc,char **argv) 
{
    if(argc < 2) 
    {
        printf("Utilisation: %s <command> [<parameter>]\n", argv[0]);
        return 1;
    }
    if(!strcmp(argv[1], "add")) 
    {
        if(argc < 3) 
        {
            printf("Utilisation: %s add <hunt>\n", argv[0]);
            return 1;
        }
        add(argv[2]);
    }
    else if(!strcmp(argv[1], "list")) 
    {
        if(argc < 3) 
        {
            printf("Utilisation: %s list <hunt>\n", argv[0]);
            return 1;
        }
        list(argv[2]);
    } 
    else if(!strcmp(argv[1], "view")) 
    {
        if(argc < 4) 
        {
            printf("Utilisation: %s view <hunt> <id>\n", argv[0]);
            return 1;
        }
        view(argv[2], atoi(argv[3]));
    }
    else if(!strcmp(argv[1], "remove_treasure")) 
    {
    if(argc < 4) 
    {
        printf("Utilizare: %s remove_treasure <hunt> <id>\n", argv[0]);
        return 1;
    }
    remove_treasure(argv[2], atoi(argv[3]));
    }
    else if(!strcmp(argv[1], "remove_hunt")) 
    {
            if(argc < 3)
            {
                printf("Utilisation: %s remove_hunt <hunt>\n", argv[0]);
                return 1;
            }
            remove_hunt(argv[2]);
    } 
    else 
    {
        printf("Unknown command: %s\n", argv[1]);
        return 1;
    }
    return 0;
}

