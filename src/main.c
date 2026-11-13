#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#include "player.h"
#include "utils.h"
#include "map.h"

// Функция для вывода главного меню
void show_main_menu(void) {
    printf("\n    ГЛАВНОЕ МЕНЮ   \n");
    printf("[0] Выход\n");
    printf("[1] Создать / пересоздать персонажа\n");
    printf("[2] Карточка персонажа\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Инвертировать инвентарь (Вариант 7)\n");
    printf("[7] Поиск предмета в рюкзаке\n");
    printf("[8] Записать состояние в дневник\n");
    printf("[9] Расшифровать старые записи\n");
    printf("[10] Купить масло для фонаря\n");
    printf("[11] Управление временем\n");
    printf("[12] Вход в подземелье (Приключение)\n");
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "ru_RU.UTF-8");
    #ifdef _WIN32
        system("chcp 65001 > nul"); // Для корректного вывода кириллицы в Windows
    #endif
    Player *hero = NULL;
    
    GameTime game_time = { .current_day = 1, .current_hour = 8 };
    int choice;

    printf("Игра 'Веселый фермер' запущена!\n");

    do {
        show_main_menu();
        choice = read_int_safe("Выберите пункт: ", 0, 12);

        switch (choice) {
            case 0:
                printf("Выход из игры...\n");
                break;

            case 1:
                if (hero != NULL) {
                    int confirm = read_int_safe("Персонаж уже существует. Точно удалить его и создать нового? (1-Да, 0-Нет): ", 0, 1);
                    if (confirm == 1) {
                        free_player(hero); 
                        hero = create_player(); 
                    }
                } else {
                    hero = create_player();
                }
                break;

            case 2:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else print_player_card(hero, &game_time);
                break;

            case 3:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else show_inventory(hero);
                break;

            case 4:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else add_item_to_inventory(hero);
                break;

            case 5:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else drop_item_from_inventory(hero);
                break;

            case 6:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else invert_inventory(hero);
                break;

            case 7:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else search_item(hero); 
                break;

            case 8:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else write_diary(hero, &game_time);
                break;

            case 9:
                decode_records();
                break;

            case 10:
                if (hero == NULL) printf("Сначала создайте персонажа!\n");
                else buy_oil(hero);
                break;

            case 11: 
                if (hero == NULL) {
                    printf("Сначала создайте персонажа!\n");
                    break;
                }
                int t_choice;
                do {
                    printf("\n--- УПРАВЛЕНИЕ ВРЕМЕНЕМ ---\n");
                    printf("[1] Посмотреть на часы\n");
                    printf("[2] Пропустить время (Ждать)\n");
                    printf("[3] Лечь спать (Отдых)\n");
                    printf("[0] Назад\n");
                    t_choice = read_int_safe("Выбор: ", 0, 3);

                    if (t_choice == 1) time_show(&game_time);
                    else if (t_choice == 2) time_wait(hero, &game_time);
                    else if (t_choice == 3) time_sleep(hero, &game_time);
                } while (t_choice != 0);
                break;
            case 12:
                if (hero == NULL) {
                    printf("Сначала создайте персонажа!\n");
                } else if (hero->stamina <= 0) {
                    printf("У вас 0 выносливости! Отдохните (Управление временем -> Лечь спать).\n");
                } else if (game_time.current_hour >= 2 && game_time.current_hour < 8) {
                    printf("Сейчас ночь (после 02:00)! Фермер должен спать, а не по подземельям ходить.\n");
                } else {
                    start_adventure(hero, &game_time); // Вызов нашей игры из map.c
                }
                break;
            default:
                printf("Неверный пункт меню!\n");
                break;
        }
    } while (choice != 0);
    if (hero != NULL) {
        free_player(hero);
        hero = NULL;
    }
    return 0;
}