#include <stdio.h>

#define INVENTORY_SIZE 10
#define HOURS_IN_DAY 24
#define START_DAY 1
#define START_HOUR 8

#define ITEM_EMPTY 0
#define ITEM_WOOD 1
#define ITEM_STONE 2
#define ITEM_SEEDS 3
#define ITEM_CARROT 4
#define ITEM_WATER 5

int main() {
     int current_day = START_DAY;
     int current_hour = START_HOUR;
     int inventory[INVENTORY_SIZE] = {0};
 
     inventory[0] = ITEM_EMPTY;
     inventory[1] = ITEM_STONE;
     inventory[2] = ITEM_SEEDS;
     inventory[8] = ITEM_CARROT;
     inventory[9] = ITEM_WATER;

     printf("Игра 'Веселый фермер' запущена!\n");
     printf("Начальное время: День %d, %02d:00\n", current_day, current_hour);

     return 0;
}