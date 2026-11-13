#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NAME_LEN 32
#define MAX_ITEMS 10
#define MAX_OIL 100
#define OIL_PRICE 30
#define NIGHT_HOUR 2

#define ITEM_EMPTY  0
#define ITEM_WOOD   1
#define ITEM_STONE  2
#define ITEM_SEEDS  3
#define ITEM_CARROT 4
#define ITEM_WATER  5
#define ITEM_APPLE  6
#define ITEM_HAY    7
#define ITEM_AXE    8
#define ITEM_RAKE   9


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

Player* create_player(void);
void free_player(Player *p);
void print_player_card(const Player *p, const GameTime *t);
void buy_oil(Player *p);

void time_show(const GameTime *t);
void time_wait(Player *p, GameTime *t);
void time_sleep(Player *p, GameTime *t);

void show_inventory(const Player *p);
void add_item_to_inventory(Player *p);
void drop_item_from_inventory(Player *p);
void invert_inventory(Player *p);
const char* get_item_name(int id);

void search_item(const Player *p);
void write_diary(const Player *p, const GameTime *t);
void decode_records(void);

#endif // PLAYER_H