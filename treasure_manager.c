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
    int log = open(logfile, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(log==-1) 
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
            close(log);
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
    int f = open(fp, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(f==-1) 
    {
        perror("Error creating file\n");
        return;
    }
    Treasure treasure = create();
    
    if(write(f, &treasure, sizeof(Treasure)) != sizeof(Treasure)) 
    {
        perror("Error writing in file\n");
        return;
    }

    char logbuff[256];
    sprintf(logbuff, "Treasure %d added\n", treasure.TreasureId),
    write(log, &logbuff, strlen(logbuff));

    if(closedir(dir) < 0) 
    {
        perror("Error closind direcotory\n");
        return;
    }
    if(close(f)) 
    {
        perror("Error closing file\n");
        return;
    }
    if(close(log)) 
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
    int log = open(logfile, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(log==-1) 
    {
        perror("Logfile could not open\n");
        return;
    }
    int f = open(fp, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(f==-1) 
    {
        perror("Hunt not made\n");
        return;
    }
    Treasure treasure;
    printf("The list of tresures for %s:\n", id);
    while(read(f, &treasure, sizeof(Treasure))) 
    {
        printTreasure(&treasure);
    }
    
    char logbuff[256];
    sprintf(logbuff, "Hunt %s shown.\n", id),
    write(log, &logbuff, strlen(logbuff));

    if(close(f)) 
    {
        perror("Error closing file\n");
        return;
    }
    if(close(log)) 
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
    char logbuff[256];
    sprintf(fp, "%s/treasures", id);
    sprintf(logfile, "%s/logging_file.txt", id);

    int log = open(logfile, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(log==-1) 
    {
        perror("Logfile could not open\n");
        return;
    }
    int f = open(fp, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(f==-1) 
    {
        perror("Hunt not made\n");
        return;
    }

    Treasure treasure;
    while(read(f, &treasure, sizeof(Treasure))) 
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
        sprintf(logbuff, "Treasure %d found\n", TreasureId),
        write(log, &logbuff, strlen(logbuff));
    }
    else
    {
        sprintf(logbuff, "Treasure %d not found\n", TreasureId),
        write(log, &logbuff, strlen(logbuff));
        printf("Treasure %d not found\n", TreasureId);
    }

    if(close(f)) 
    {
        perror("Error closing file\n");
        return;
    }
    if(close(log)) 
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
    char logbuff[256];
    Treasure treasure;
    int OK = 0;
    sprintf(logfile, "%s/logging_file.txt", id);
    sprintf(fp, "%s/treasures", id);

    int log = open(logfile, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(!log) 
    {
        perror("Could not open log file");
        return;
    }
    int f = open(fp, O_RDWR | O_CREAT | O_APPEND, 0644);
    if(!f) 
    {
        perror("Could not open file");
        return;
    }
    sprintf(tmp, "%s/tmp.txt", id);
    int fis = open(tmp, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(!fis) 
    {
        perror("Error opening file");
        close(f);
        return;
    }

    while(read(f, &treasure, sizeof(Treasure)) == sizeof(Treasure)) 
    {
        if(treasure.TreasureId == TreasureId) 
        {
            OK = 1;
            continue;
        }
        write(fis, &treasure, sizeof(Treasure));
    }
    if(!OK) 
    {
        printf("Treasure %d does not exist\n", TreasureId);
        sprintf(logbuff, "Treasure %d does not exist\n", TreasureId),
        write(log, &logbuff, strlen(logbuff));
        remove("fis");
        return;
    }
    else
    {
        sprintf(logbuff, "Treasure %d was deleted\n", TreasureId),
        write(log, &logbuff, strlen(logbuff));
    }

    if(close(f)) 
    {
        perror("Error closing file");
        close(fis);
        return;
    }
    if(close(fis)) 
    {
        perror("Error closing file");
        return;
    }
    if(close(log)) 
    {
        perror("Error colsing log file");
        return;
    }
    if(remove(fp) < 0) 
    {
        perror("Error removing file");
        return;
    }
    if(rename(tmp, fp) !=0) 
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

    if(unlink(OGlog) < 0) 
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
    if(!strcmp(argv[1], "--add")) 
    {
        if(argc < 3) 
        {
            printf("Utilisation: %s --add <hunt>\n", argv[0]);
            return 1;
        }
        add(argv[2]);
    }
    else if(!strcmp(argv[1], "--list")) 
    {
        if(argc < 3) 
        {
            printf("Utilisation: %s --list <hunt>\n", argv[0]);
            printf("%s", argv[2]);
            return 1;
        }
        list(argv[2]);
    } 
    else if(!strcmp(argv[1], "--view")) 
    {
        if(argc < 4) 
        {
            printf("Utilisation: %s --view <hunt> <id>\n", argv[0]);
            return 1;
        }
        view(argv[2], atoi(argv[3]));
    }
    else if(!strcmp(argv[1], "--remove_treasure")) 
    {
    if(argc < 4) 
    {
        printf("Utilisation: %s --remove_treasure <hunt> <id>\n", argv[0]);
        return 1;
    }
    remove_treasure(argv[2], atoi(argv[3]));
    }
    else if(!strcmp(argv[1], "--remove_hunt")) 
    {
            if(argc < 3)
            {
                printf("Utilisation: %s --remove_hunt <hunt>\n", argv[0]);
                return 1;
            }
            remove_hunt(argv[2]);
    }
    else if(!strcmp(argv[1], "--help")) 
    {
            printf("List of comands:\n");
            printf("For addind new hunts/treasure: %s --add <hunt>\n", argv[0]);
            printf("For listing all treasures from a hunt: %s --list <hunt>\n", argv[0]);
            printf("For viewing a specific treasure from a hunt: %s --view <hunt> <id>\n", argv[0]);
            printf("For removing a specific treasure from a hunt: %s --remove_treasure <hunt> <id>\n", argv[0]);
            printf("For removing an entire hunt: %s --remove_hunt <hunt>\n", argv[0]);
            return 0;
    } 
    else
    {
        //printf("%ld", sizeof(Treasure));
        printf("Unknown command: %s\n", argv[1]);
        return 1;
    }
    return 0;
}

