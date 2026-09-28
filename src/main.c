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

     int choose;
     
     do {
          printf("\n    МЕНЮ   \n");
          printf("[0] Выход\n");
          printf("[1] Посмотреть на часы\n");
          printf("[2] Промотать время\n");
          printf("[3] Посмотреть инвентарь\n");
          printf("[4] Положить предмет в слот\n");
          printf("[5] Выбросить предмет\n");
          printf("[6] Выполнить задание по варианту\n");
          printf("Выберите пункт: "); 
     }  while (choose = !0);

     if (scanf("%d", &choose) != 1) {
      printf("Ошибка ввода! Введите число.\n");
      while (getchar() != '\n'); 
      choose = -1; 
     }


     switch (choose) {
    case 0:
        printf("Выход из игры...\n");
        break; // break прерывает switch, а условие while прервет цикл
        
    case 1:
        printf("Пункт 1: Часы (в разработке)\n");
        break;
        
    case 2:
        printf("Пункт 2: Время (в разработке)\n");
        break;
    case 3:
        printf("Пункт 3: Время (в разработке)\n");
        break;  
    case 4:
        printf("Пункт 4: Время (в разработке)\n");
        break;
   case 5:
        printf("Пункт 5: Время (в разработке)\n");
        break;
     
    
    default:
        printf("Неверный пункт меню!\n");
        break;
}
     return 0;
}