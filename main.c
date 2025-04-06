#include <stdio.h>

typedef struct gps{
    float longitude;
    float latitude;
}gps;

typedef struct Treasure{
    int TreasureId;
    char User[50];
    gps coordonates;
    char clue[100];
    int value;
}Treasure;

int main()
{
    printf("Sall");
    return 0;
}