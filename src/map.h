#ifndef MAP_H
#define MAP_H

#include "player.h"

#define MAP_HEIGHT 30
#define MAP_WIDTH 30

#define TILE_EMPTY '.'
#define TILE_WALL  '#'
#define TILE_EXIT  'Y'  
#define TILE_UP    'U' 
#define TILE_STONE 'K'
#define TILE_TREE  'H'

typedef struct {
    char **grid;       
    int height;        
    int width;         
    int current_floor; 
} Map;


Map* create_map(int height, int width, int floor);
void free_map(Map *m);
void draw_map(const Map *m, int player_y, int player_x);
void start_adventure(Player *p, GameTime *t);

#endif 