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

typedef struct score{
    char user[50];
    int score;
}score;

int main(int argc, char **argv)
{
    score users[50]; 
    Treasure tr;
    int user_count = 0;
    
    struct dirent *entry;
    char path[300];

    DIR *d = opendir(".");
    if (!d) 
    {
        perror("opendir");
        return 0;
    }
    //max 10 hunts
    while ((entry = readdir(d)) != NULL) 
    {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) 
        {
            snprintf(path, sizeof(path), "%s/treasures", entry->d_name);
            //printf("%s", entry->d_name);
            if( strcmp(entry->d_name,"hunt1")==0 || strcmp(entry->d_name,"hunt2")==0 || strcmp(entry->d_name,"hunt3")==0 || strcmp(entry->d_name,"hunt4")==0 || strcmp(entry->d_name,"hunt5")==0 || strcmp(entry->d_name,"hunt6")==0 || strcmp(entry->d_name,"hunt7")==0 || strcmp(entry->d_name,"hunt8")==0 || strcmp(entry->d_name,"hunt9")==0 || strcmp(entry->d_name,"hunt10")==0)
            {
                //printf("%s", path);
                int fd = open(path, O_RDONLY);

                if (fd >= 0) 
                {
                    while ((read(fd, &tr, sizeof(Treasure))) == sizeof(Treasure)) 
                    {
                        int user_found = 0;

                        for (int i = 0; i < user_count; i++) 
                        {
                            if (strcmp(users[i].user, tr.user) == 0) 
                            {
                                users[i].score += tr.value;
                                user_found = 1;
                                break;
                            }
                        }

                        if (!user_found) 
                        {
                            strncpy(users[user_count].user, tr.user, sizeof(users[user_count].user) - 1);
                            users[user_count].user[sizeof(users[user_count].user) - 1] = '\0';
                            users[user_count].score = tr.value;
                            user_count++;
                        }
                    }
                    close(fd);
                }
                if (user_count == 0) 
                {
                    printf("No users found in %s\n", entry->d_name);
                }
                else
                {
                    printf("Scores for all users in %s:\n", entry->d_name);
                    for (int i = 0; i < user_count; i++) 
                    {
                        printf("User: %s, Score: %d\n", users[i].user, users[i].score);
                        users[i].score=0;
                    }
                    printf("\n");
                    user_count=0;
                }
            }
        }

    }
    return 0;
}
