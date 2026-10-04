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

#define ITEM_NAME_EMPTY   "Пусто"
#define ITEM_NAME_WOOD    "Дерево"
#define ITEM_NAME_STONE   "Камень"
#define ITEM_NAME_SEEDS   "Семена"
#define ITEM_NAME_CARROT  "Морковь"
#define ITEM_NAME_WATER   "Вода"
#define ITEM_NAME_APPLE   "Яблоко"
#define ITEM_NAME_HAY     "Сено"
#define ITEM_NAME_AXE     "Топор"
#define ITEM_NAME_RAKE    "Грабли"

#define GET_ITEM_NAME(item_id) \
    ((item_id) == ITEM_EMPTY  ? ITEM_NAME_EMPTY  : \
     (item_id) == ITEM_WOOD   ? ITEM_NAME_WOOD   : \
     (item_id) == ITEM_STONE  ? ITEM_NAME_STONE  : \
     (item_id) == ITEM_SEEDS  ? ITEM_NAME_SEEDS  : \
     (item_id) == ITEM_CARROT ? ITEM_NAME_CARROT : \
     (item_id) == ITEM_WATER  ? ITEM_NAME_WATER  : \
     (item_id) == ITEM_APPLE  ? ITEM_NAME_APPLE  : \
     (item_id) == ITEM_HAY    ? ITEM_NAME_HAY    : \
     (item_id) == ITEM_AXE    ? ITEM_NAME_AXE    : \
     (item_id) == ITEM_RAKE   ? ITEM_NAME_RAKE   : \
     "Неизвестный предмет")

int read_int_from_user(const char* prompt) {
    int num;
    int valid_input;
    do {
        printf("%s", prompt);
        valid_input = (scanf("%d", &num) == 1);
        if (!valid_input) {
            printf("Ошибка! Нужно ввести число.\n");
            while (getchar() != '\n');
        }
    } while (!valid_input);
    
    return num;
}


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
      int c;
      while ((c = getchar()) != '\n' && c != EOF); 
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
        do {
            hours_worked = read_int_from_user("Введите количество отработанных часов: ");
            if (hours_worked <= 0) {
                printf("Это число не подходит, введите положительное число.\n");
            }
        } while (hours_worked <= 0); 

        current_hour += hours_worked;
        current_day += current_hour / HOURS_IN_DAY;
        current_hour = current_hour % HOURS_IN_DAY;
    
        printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
        break;
    }
    case 3:
        for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] %s\n", i, inventory[i], GET_ITEM_NAME(inventory[i]));
        }
        break;  
    case 4:
    
    do {
        item_id = read_int_from_user("Введите ID предмета: ");
        if (item_id < 0 || item_id > INVENTORY_SIZE) {
            printf("Неверный ID предмета, введите верный ID\n");
        }
        
    } while (item_id < 0 || item_id > INVENTORY_SIZE);
    do {
        slot_index = read_int_from_user("Введите номер слота ");
        if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
            printf("Неверный индекс слота! Попробуйте снова.\n");
        }
    } while (slot_index < 0 || slot_index >= INVENTORY_SIZE);
    inventory[slot_index] = item_id;
    printf("Предмет [%d] %s добавлен в слот %d\n",
            item_id, GET_ITEM_NAME(item_id), slot_index);
    break;
   case 5:
        do {
        slot_index = read_int_from_user("Введите номер слота ");
        if (slot_index < 0 || slot_index >= INVENTORY_SIZE) {
            printf("Неверный индекс слота! Попробуйте снова.\n");
        }
        } while (slot_index < 0 || slot_index >= INVENTORY_SIZE);
        inventory[slot_index] = 0;
            printf("Предмет из слота %d выброшен\n", slot_index);
        break;
   case 6:
        printf("    Инвентарь ДО инверсии   \n");
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            printf("Слот %d: [%d] %s\n", i, inventory[i], GET_ITEM_NAME(inventory[i]));
        }
        int temp; 
        for (int i = 0, j = INVENTORY_SIZE - 1; i < j; i++, j--) {
        temp = inventory[i];
        inventory[i] = inventory[j];
        inventory[j] = temp;
        }
        printf("\n  Инвентарь ПОСЛЕ инверсии  \n");
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            printf("Слот %d: [%d] %s\n", i, inventory[i], GET_ITEM_NAME(inventory[i]));
        }
    break;

    default:
        printf("Неверный пункт меню!\n");
        break;
}
    }  while (choose != 0);

     return 0;
}