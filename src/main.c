#include <stdio.h>
#include <locale.h>

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
#define ITEM_APPLE 6     
#define ITEM_HAY 7     
#define ITEM_AXE 8       
#define ITEM_RAKE 9  

int main() {
     setlocale(LC_ALL, "ru_RU.UTF-8");
     int current_day = START_DAY;
     int current_hour = START_HOUR;
     int inventory[INVENTORY_SIZE] = {0};
 
     inventory[0] = ITEM_EMPTY;
     inventory[1] = ITEM_WOOD;
     inventory[2] = ITEM_STONE;
     inventory[3] = ITEM_SEEDS;
     inventory[4] = ITEM_CARROT;
     inventory[5] = ITEM_WATER;
     inventory[6] = ITEM_APPLE;
     inventory[7] = ITEM_HAY;
     inventory[8] = ITEM_AXE;
     inventory[9] = ITEM_RAKE;

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
     
     if (scanf("%d", &choose) != 1) {
      printf("Ошибка ввода! Введите число.\n");
      while (getchar() != '\n'); 
      choose = -1; 
     }

    int slot_index, item_id;
     switch (choose) {
    case 0:
        printf("Выход из игры...\n");
        break; 
        
    case 1:
        printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
        break;
        
    case 2: {
        int hours_worked;
        printf("Сколько ты часов играл: ");
        scanf("%d", &hours_worked);
        current_hour += hours_worked;
        current_day += current_hour / HOURS_IN_DAY;
        current_hour = current_hour % HOURS_IN_DAY;
        printf("Текущее время: День %d, %02d:00",current_day,current_hour);
        break;
    }
    case 3:
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            printf("Слот %d: [%d]\n", i, inventory[i]);
        }
        break;  
    case 4:
        printf("Введите ID предмета: ");
        scanf("%d", &item_id);
        printf("Введите номер слота: ");
        scanf("%d", &slot_index);
        if (slot_index >= 0 && slot_index < INVENTORY_SIZE) {
            inventory[slot_index] = item_id;
            printf("Предмет %d добавлен в слот %d\n", item_id, slot_index);       
        } else {
            printf("Неверный индекс слота!");
        }
        break;
   case 5:
        printf("Введите индекс слота -  ");
        scanf("%d",slot_index);
        if (slot_index >= 0 && slot_index < INVENTORY_SIZE) {
            inventory[slot_index] = 0;
            printf("Предмет из слота %d выброшен\n", slot_index);
        }
        break;
   case 6:
        printf("    Инвентарь ДО инверсии   \n");
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            printf("Слот %d: [%d]\n", i, inventory[i]);
        }
        int temp; 
        for (int i = 0, j = INVENTORY_SIZE - 1; i < j; i++, j--) {
        temp = inventory[i];
        inventory[i] = inventory[j];
        inventory[j] = temp;
        }
        printf("\n  Инвентарь ПОСЛЕ инверсии  \n");
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            printf("Слот %d: [%d]\n", i, inventory[i]);
        }
    break;

    default:
        printf("Неверный пункт меню!\n");
        break;
}
    }  while (choose != 0);

     return 0;
}