#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>


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

FILE* open_file_for_reading(const char* filename);
FILE* open_file_for_writing(const char* filename);

void remove_newline(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n' ||  str[i] == '\r')  {
            str[i] = '\0';
            break;
        }
    }
}

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
 
void load_item_names(char item_names[INVENTORY_SIZE][32]) {
    FILE *file = fopen("items.txt", "r");
    if (file == NULL) {
        printf("Предупреждение: файл items.txt не найден! Используются стандартные названия.\n");
        strcpy(item_names[0], "Пусто");
        strcpy(item_names[1], "Дерево");
        strcpy(item_names[2], "Камень");
        strcpy(item_names[3], "Семена");
        strcpy(item_names[4], "Морковь");
        strcpy(item_names[5], "Вода");
        strcpy(item_names[6], "Яблоко");
        strcpy(item_names[7], "Сено");
        strcpy(item_names[8], "Топор");
        strcpy(item_names[9], "Грабли");
        return;
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        fgets(item_names[i], 32, file);
        remove_newline(item_names[i]);
    }
    
    fclose(file);
}


int main() { 
     setlocale(LC_ALL, "ru_RU.UTF-8");
     system("chcp 65001 > nul");
     int current_day = START_DAY;
     int current_hour = START_HOUR;
     int inventory[INVENTORY_SIZE] = {0};
     char farmer_name[33];
     char item_names[INVENTORY_SIZE][32]; 
     
 
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
     printf("Как вас зовут, фермер? \n");
     fgets(farmer_name, sizeof(farmer_name), stdin);
     remove_newline(farmer_name);
     printf("Добро пожаловать, %s!\n", farmer_name);
    load_item_names(item_names); 

     
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
          printf("[7] Поиск предмета в рюкзаке\n");
          printf("[8] Записать состояние в дневник\n");
          printf("[9] Расшифровать старые записи (Задание по варианту)\n");
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
        printf("Слот %d: [%d] - %s\n", i, inventory[i], item_names[inventory[i]]);
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
   case 7: {
        char search_name[32];
        while (getchar() != '\n');
        printf("Введите название предмета для поиска: ");
        fgets(search_name, sizeof(search_name), stdin);
        remove_newline(search_name);
        int found_id = -1; 
        for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (strcmp(item_names[i], search_name) == 0) { 
        found_id = i;
        break;
        }
        }
        if (found_id == -1) {
            printf("Предмет не найден");
            break;
        }
        printf("Предмет '%s' (ID: %d) найден в следующих слотах:\n", search_name, found_id);
        int found_in_inventory = 0;
        for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == found_id) {
            printf("Слот %d\n", i);
        found_in_inventory = 1;
        }
    }
        if (!found_in_inventory) {
            printf("Предмет есть в каталоге, но отсутствует в инвентаре.\n");
        }
        break;
   }    
   case 8: {
        FILE *diary = fopen("diary.txt", "a");
        if (diary == NULL) {
            printf("Ошибка: не удалось открыть diary.txt для записи!\n");
            break;
        }
        fprintf(diary, "    Дневник фермера %s    \n", farmer_name);
        fprintf(diary, "День: %d, Время: %02d:00\n", current_day, current_hour);
        fprintf(diary, "Инвентарь:\n");
        for (int i = 0; i < INVENTORY_SIZE; i++) {
            fprintf(diary, "  Слот %d: %s\n", i, item_names[inventory[i]]);
        }
        fprintf(diary, "==========================\n\n");
        fclose(diary);
        printf("Запись успешно добавлена в diary.txt!\n");
        break;
    }
                case 9: {
                char search_word[64];
                char search_word_cap[64];
                char line[256];
                char result[512]; // Увеличенный буфер для новой строки
                int line_number = 0;
                int found_count = 0;

                while (getchar() != '\n');
                printf("Введите поисковое слово: ");
                fgets(search_word, sizeof(search_word), stdin);
                remove_newline(search_word);

                // Создаём вариант с заглавной первой буквой (UTF-8 кириллица)
                strcpy(search_word_cap, search_word);
                int sw_len = strlen(search_word_cap);
                if (sw_len >= 2 && (unsigned char)search_word_cap[0] == 0xD0 &&
                    (unsigned char)search_word_cap[1] >= 0xB0 && (unsigned char)search_word_cap[1] <= 0xBF) {
                    search_word_cap[1] -= 0x20;
                } else if (sw_len >= 2 && (unsigned char)search_word_cap[0] == 0xD1 &&
                           (unsigned char)search_word_cap[1] >= 0x80 && (unsigned char)search_word_cap[1] <= 0x8F) {
                    search_word_cap[0] = 0xD0;
                    search_word_cap[1] += 0x10;
                }

                FILE *input = open_file_for_reading("input.txt");
                if (input == NULL) {
                    break;
                }

                FILE *output = open_file_for_writing("output.txt");
                if (output == NULL) {
                    fclose(input);
                    break;
                }

                printf("\n=== Результаты поиска ===\n");
                fprintf(output, "=== Результаты поиска ===\n");

                while (fgets(line, sizeof(line), input) != NULL) {
                    remove_newline(line);
                    line_number++;

                    char *found = strstr(line, search_word);
                    if (found == NULL) {
                        found = strstr(line, search_word_cap);
                    }

                    if (found != NULL) {
                        found_count++;
                        int word_pos = found - line;
                        int word_len = strlen(search_word);

                        // 1. Копируем часть строки ДО найденного слова
                        strncpy(result, line, word_pos);
                        result[word_pos] = '\0'; // Гарантируем завершение строки

                        // 2. Добавляем открывающую скобку
                        strcat(result, "[");

                        // 3. Добавляем само найденное слово
                        strncat(result, found, word_len);

                        // 4. Добавляем закрывающую скобку
                        strcat(result, "]");

                        // 5. Добавляем остаток строки ПОСЛЕ слова
                        strcat(result, found + word_len);

                        // Выводим и записываем готовую строку
                        printf("%d. %s\n", line_number, result);
                        fprintf(output, "%d. %s\n", line_number, result);
                    }
                }

                printf("\nНайдено статей: %d\n", found_count);
                fprintf(output, "\nНайдено статей: %d\n", found_count);

                fclose(input);
                fclose(output);
                break;
            } // <-- Эта скобка закрывает case 9
    default:
        printf("Неверный пункт меню!\n");
        break;
    }  
    }  while (choose != 0);
        return 0;
     }

FILE* open_file_for_reading(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка: файл %s не найден!\n", filename);
    }
    return file;
}

FILE* open_file_for_writing(const char* filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка: не удалось создать файл %s!\n", filename);
    }
    return file;
}