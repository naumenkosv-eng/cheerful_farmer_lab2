#ifndef ENTITY_H
#define ENTITY_H

typedef enum {
    ENT_TREE,     
    ENT_ROCK,     
    ENT_HEAL,     
    ENT_MONSTER  
} EntityType;

typedef struct {
    EntityType type; 
    char symbol;    
    int x, y;       
    int hp, max_hp;  
    int attack;     
    int defense;     
    int drop_item_id;
    int is_alive;    
} Entity;

#endif 