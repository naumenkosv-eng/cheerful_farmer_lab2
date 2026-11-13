#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NAME_LEN 32
#define MAX_ITEMS 10
#define MAX_OIL 100
#define OIL_PRICE 10
#define NIGHT_HOUR 2 

typedef enum {
    ITEM_EMPTY = 0, 
    ITEM_WOOD, 
    ITEM_STONE, 
    ITEM_SEEDS,
    ITEM_CARROT, 
    ITEM_WATER, 
    ITEM_APPLE, 
    ITEM_HAY, 
    ITEM_AXE, 
    ITEM_RAKE
} ItemID;

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

#endif 