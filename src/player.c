#include "utils.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 32
#define MAX_ITEMS 10
#define MAX_OIL 100
#define OIL_PRICE 30
#define NIGHT_HOUR 2 


typedef enum {
    LIGHT_OFF = 0,    
    LIGHT_NORMAL = 1, 
    LIGHT_BRIGHT = 2  
} LightMode;

typedef struct {
    int oil;          
    LightMode mode;   
} Lantern;

typedef struct {
    int current_day;   
    int current_hour;  
} GameTime;

typedef struct {
    char name[MAX_NAME_LEN];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x, y;
    int inventory[MAX_ITEMS];
    Lantern lantern; 
} Player;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

const char* get_item_name(int id) {
    switch (id) {
        case 0: return "Пусто";
        case 1: return "Дерево";
        case 2: return "Камень";
        case 3: return "Семена";
        case 4: return "Морковь";
        case 5: return "Вода";
        case 6: return "Яблоко";
        case 7: return "Сено";
        case 8: return "Топор";
        case 9: return "Грабли";
        case 10: return "Топливо для фонаря";
        default: return "Неизвестный предмет";
    }
}

Player* create_player(void) {
    Player *p = (Player*)malloc(sizeof(Player));
    if (p == NULL) {
        printf("Ошибка: не удалось выделить память под игрока.\n");
        return NULL;
    }
    printf("Введите имя персонажа: ");
    scanf("%31s", p->name); 

    p->max_hp = 100;
    p->hp = p->max_hp;
    p->max_stamina = 50;
    p->stamina = p->max_stamina;
    p->gold = 50; 
    p->x = 0;
    p->y = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        p->inventory[i] = 0; 
    }
    p->lantern.oil = 50;             
    p->lantern.mode = LIGHT_OFF;     
    printf("\nПерсонаж %s успешно создан!\n", p->name);
    return p;
}
void free_player(Player *p) {
    if (p != NULL) {
        printf("Память структуры по адресу %p освобождена\n", (void*)p);
        free(p);
    }
}
void print_player_card(const Player *p, const GameTime *t) {
    if (p == NULL) {
        printf("Ошибка: персонаж не создан!\n");
        return;
    }

    printf("\n===================================\n");
    printf("       КАРТОЧКА ПЕРСОНАЖА\n");
    printf("===================================\n");
    
    // Имя и статы
    printf("Имя: %s\n", p->name);
    printf("HP (Здоровье):    %d / %d\n", p->hp, p->max_hp);
    printf("Выносливость:     %d / %d\n", p->stamina, p->max_stamina);
    printf("Золото:           %d монет\n", p->gold);

    // Состояние фонаря (наш вариант 7)
    printf("Фонарь:           масло %d/%d, режим: ", p->lantern.oil, MAX_OIL);
    switch (p->lantern.mode) {
        case LIGHT_OFF:    printf("Погашен\n"); break;
        case LIGHT_NORMAL: printf("Обычный\n"); break;
        case LIGHT_BRIGHT: printf("Яркий\n");   break;
    }

    
    if (t != NULL) {
        printf("Игровое время:    День %d, %02d:00\n", t->current_day, t->current_hour);
    }

    printf("-----------------------------------\n");
    printf("ИНВЕНТАРЬ:\n");
    int has_items = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (p->inventory[i] != 0) { // 0 - значит слот пустой
            printf("  Слот %d: %s (ID: %d)\n", i + 1, get_item_name(p->inventory[i]), p->inventory[i]);
            has_items = 1;
        }
    }
    if (!has_items) {
        printf("  (Инвентарь пуст)\n");
    }
    printf("===================================\n\n");
}
void buy_oil(Player *p) {
    if (p == NULL) return;

    int amount;
    printf("\n--- ЛАВКА ТОРГОВЦА ---\n");
    printf("У вас %d золота. В фонаре %d/100 масла.\n", p->gold, p->lantern.oil);
    printf("Цена: %d золота за 1 ед. масла.\n", OIL_PRICE);
    printf("Сколько единиц масла купить? (0 - отмена): ");
    if (scanf("%d", &amount) != 1) {
        printf("Некорректный ввод!\n");
        while (getchar() != '\n'); 
        return;
    }
    if (amount <= 0) return;
    if (p->lantern.oil + amount > MAX_OIL) {
        printf("Нельзя купить больше, чем влезает в бак (макс 100)!\n");
        return;
    }
    int total_cost = amount * OIL_PRICE;
    if (p->gold < total_cost) {
        printf("Недостаточно золота! Нужно %d, а у вас %d.\n", total_cost, p->gold);
        return;
    }
    p->gold -= total_cost;
    p->lantern.oil += amount;
    printf("Успешно куплено %d ед. масла! Остаток золота: %d\n", amount, p->gold);
}
void time_show(const GameTime *t) {
    if (t == NULL) return;
    printf("Сейчас: День %d, %02d:00\n", t->current_day, t->current_hour);
}
void time_wait(Player *p, GameTime *t) {
    if (p == NULL || t == NULL) return;

   
    int max_hours;
    if (t->current_hour < NIGHT_HOUR) {
        max_hours = NIGHT_HOUR - t->current_hour;
    } else {
        max_hours = (24 - t->current_hour) + NIGHT_HOUR;
    }

    if (max_hours <= 0) {
        printf("Уже ночь (02:00)! Фермер должен спать.\n");
        return;
    }

    printf("Сейчас день %d, %02d:00. До ночи (02:00) можно ждать не больше %d ч.\n",
           t->current_day, t->current_hour, max_hours);

    int hours = read_int_safe("Сколько часов ждать? (1 - отмена): ", 1, max_hours);
    if (hours <= 0) {
        printf("Отмена ожидания.\n");
        return;
    }

    t->current_hour += hours;
    while (t->current_hour >= 24) {
        t->current_hour -= 24;
        t->current_day++;
    }


    p->stamina += hours * 2;
    if (p->stamina > p->max_stamina) p->stamina = p->max_stamina;

    printf("Вы прождали %d ч. Теперь: день %d, %02d:00. Стамина: %d/%d\n",
           hours, t->current_day, t->current_hour, p->stamina, p->max_stamina);
}
void time_sleep(Player *p, GameTime *t) {
    if (p == NULL || t == NULL) return;

    t->current_day++;
    t->current_hour = 8;

    
    p->hp = p->max_hp;
    p->stamina = p->max_stamina;

    printf("Фермер сладко поспал. Наступил день %d, 08:00.\n", t->current_day);
    printf("HP: %d/%d, Стамина: %d/%d\n",
           p->hp, p->max_hp, p->stamina, p->max_stamina);
}
void show_inventory(const Player *p) {
    if (p == NULL) return;
    printf("\n--- ИНВЕНТАРЬ ---\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
        // Выводим ID предмета и его название
        printf("Слот %d: [%d] - %s\n", i, p->inventory[i], get_item_name(p->inventory[i]));
    }
}
void add_item_to_inventory(Player *p) {
    if (p == NULL) return;
    
    int item_id = read_int_safe("Введите ID предмета (0-9): ", 0, 9);
    int slot = read_int_safe("Введите номер слота (0-9): ", 0, MAX_ITEMS - 1);
    
    p->inventory[slot] = item_id;
    printf("Предмет %s добавлен в слот %d.\n", get_item_name(item_id), slot);
}

void drop_item_from_inventory(Player *p) {
    if (p == NULL) return;
    
    int slot = read_int_safe("Введите номер слота для удаления (0-9): ", 0, MAX_ITEMS - 1);
    p->inventory[slot] = ITEM_EMPTY; 
    printf("Слот %d очищен.\n", slot);
}
void invert_inventory(Player *p) {
    if (p == NULL) return;
    
    printf("\n--- Инвентарь ДО инверсии ---\n");
    show_inventory(p);
    int temp;
    for (int i = 0, j = MAX_ITEMS - 1; i < j; i++, j--) {
        temp = p->inventory[i];
        p->inventory[i] = p->inventory[j];
        p->inventory[j] = temp;
    }
    
    printf("\n--- Инвентарь ПОСЛЕ инверсии ---\n");
    show_inventory(p);
}
endif; 