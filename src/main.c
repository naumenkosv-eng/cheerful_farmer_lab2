#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "utils.h"

typedef enum {
    MENU_EXIT = 0,
    MENU_CREATE = 1,
    MENU_CARD = 2,
    MENU_BUY_OIL = 3,
    MENU_TIME = 4,      
    MENU_ADVENTURE = 10
} MenuItem;

void show_time_menu(Player *p, GameTime *t) {
    int choice;
    do {
        printf("\n--- УПРАВЛЕНИЕ ВРЕМЕНЕМ ---\n");
        printf("Сейчас: День %d, %02d:00\n", t->current_day, t->current_hour);
        printf("[1] Посмотреть на часы\n");
        printf("[2] Пропустить время (Ждать)\n");
        printf("[3] Лечь спать (Отдых)\n");
        printf("[0] Назад в главное меню\n");
        
        choice = read_int_safe("Выберите действие: ", 0, 3);

        switch (choice) {
            case 1: 
                time_show(t); 
                break;
            case 2: 
                time_wait(p, t); 
                break;
            case 3: 
                time_sleep(p, t); 
                break;
            case 0: 
                printf("Возврат в главное меню...\n"); 
                break;
        }
    } while (choice != 0);
}
int main(void) {
    Player *hero = NULL; 
    int choice;
    GameTime game_time = { .current_day = 1, .current_hour = 8 }; 

    printf("=== Добро пожаловать в Веселого Фермера! ===\n");

    do {
        printf("\n    МЕНЮ   \n");
          printf("[0] Выход\n");
          printf("[1] Создать персонажа\n");
          printf("[2] Характеристики героя\n");
          printf("[3] Купить масло для фонаря\n");
          printf("[4] Управление временем\n");
          printf("[5] Выбросить предмет\n");
          printf("[6] Выполнить задание по варианту\n");
          printf("[7] Поиск предмета в рюкзаке\n");
          printf("[8] Записать состояние в дневник\n");
          printf("[9] Расшифровать старые записи (Задание по варианту)\n");
          printf("[10] Купить масло для фонаря\n");
          printf("[11] Вход в подземелье \n");
          printf("Выберите пункт: "); 
        
        choice = read_int_safe("Выберите действие: ", 0, 11);

        switch (choice) {
            case MENU_CREATE:
                if (hero != NULL) {
                    int confirm = read_int_safe("Персонаж уже существует. Удалить его и создать нового? (1-Да, 0-Нет): ", 0, 1);
                    if (confirm == 1) {
                        free_player(hero); 
                        hero = create_player(); 
                    }
                } else {
                    hero = create_player();
                }
                break;

            case MENU_CARD:
                if (hero == NULL) {
                    printf("Сначала создайте персонажа!\n");
                } else {
                    print_player_card(hero, &game_time);
                }
                break;

            case MENU_BUY_OIL:
                if (hero == NULL) {
                    printf("Сначала создайте персонажа!\n");
                } else {
                    buy_oil(hero);
                }
                break;
            case MENU_TIME:
                if (hero == NULL) {
                    printf("Сначала создайте персонажа!\n");
                } else {
                    show_time_menu(hero, &game_time);
                }
                break;
            case MENU_ADVENTURE:
                if (hero == NULL) {
                    printf("Сначала создайте персонажа!\n");
                } else {
                    // Сюда мы вызовем функцию из map.c
                    printf("Функция приключения будет здесь.\n");
                }
                break;

            case MENU_EXIT:
                printf("Выход из игры...\n");
                break;

            default:
                printf("Неверный пункт меню.\n");
                break;
        }

    } while (choice != MENU_EXIT);

    // Очистка памяти при выходе!
    if (hero != NULL) {
        free_player(hero);
        hero = NULL;
    }

    return 0;
}