#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "utils.h"
#include "entity.h"

void free_map(Map *m) {
    if (m == NULL) return;
    for (int i = 0; i < m->height; i++) {
        free(m->grid[i]);
    }
    if (m->entities != NULL) {
        free(m->entities);
    }
    free(m->grid);
    printf("Память структуры карты по адресу %p освобождена\n", (void*)m);
    free(m);
}
void place_walls_and_stairs(Map *m) {
    for (int j = 0; j < m->width; j++) {
        m->grid[0][j] = TILE_WALL;
        m->grid[m->height - 1][j] = TILE_WALL;
    }
    for (int i = 0; i < m->height; i++) {
        m->grid[i][0] = TILE_WALL;
        m->grid[i][m->width - 1] = TILE_WALL;
    }
    int exit_y, exit_x;
    do {
        exit_y = 1 + rand() % (m->height - 2);
        exit_x = 1 + rand() % (m->width - 2);
    } while (exit_y == 1 && exit_x == 1); 
    
    m->grid[exit_y][exit_x] = TILE_EXIT;
}
void generate_ice(Map *m) {
    int target = (m->height - 2) * (m->width - 2) * 25 / 100;  
    int placed = 0;
    int attempts = 0;

    while (placed < target && attempts < 5000) {
        attempts++;
        int len = 2 + rand() % 4;          
        int horizontal = rand() % 2;

        int y, x;
        if (horizontal) {
            if (m->width - 2 < len) continue;
            y = 1 + rand() % (m->height - 2);
            x = 1 + rand() % (m->width - 2 - len + 1);
        } else {
            if (m->height - 2 < len) continue;
            y = 1 + rand() % (m->height - 2 - len + 1);
            x = 1 + rand() % (m->width - 2);
        }

        int ok = 1;
        for (int i = 0; i < len; i++) {
            int cy = horizontal ? y : y + i;
            int cx = horizontal ? x + i : x;
            if (m->grid[cy][cx] != TILE_EMPTY) { ok = 0; break; }
        }
        if (!ok) continue;

        for (int i = 0; i < len; i++) {
            int cy = horizontal ? y : y + i;
            int cx = horizontal ? x + i : x;
            m->grid[cy][cx] = '~'; 
        }
        placed += len;
    }
}
void slide_on_ice(Map *m, Player *p, char direction) {
    printf("\n>>> Вы поскользнулись на льду! <<<\n");
    int dy = 0, dx = 0;
    if (direction == 'W' || direction == 'w') dy = -1;
    else if (direction == 'S' || direction == 's') dy = 1;
    else if (direction == 'A' || direction == 'a') dx = -1;
    else if (direction == 'D' || direction == 'd') dx = 1;
    
    while (1) {
        int new_y = p->y + dy;
        int new_x = p->x + dx;
        
        if (new_y < 0 || new_y >= m->height || new_x < 0 || new_x >= m->width || 
            m->grid[new_y][new_x] == TILE_WALL) {
            break;  
        }
        
        
        p->y = new_y;
        p->x = new_x;
        
        if (m->grid[p->y][p->x] == TILE_EMPTY) {
            break;
        }
        if (m->grid[p->y][p->x] == TILE_EXIT) {
            break;
        }
    }
}
void generate_obstacles(Map *m) {
    int total_cells = (m->height - 2) * (m->width - 2);
    int target_trees = (int)(total_cells * 0.05);
    int target_stones = (int)(total_cells * 0.05);
    int placed_trees = 0;
    while (placed_trees < target_trees) {
        int y = 1 + rand() % (m->height - 2);
        int x = 1 + rand() % (m->width - 2);
        if (y == 1 && x == 1) continue;
        if (m->grid[y][x] == TILE_EMPTY) {
            m->grid[y][x] = TILE_TREE; 
            placed_trees++;
        }
    }
    int placed_stones = 0;
    while (placed_stones < target_stones) {
        int y = 1 + rand() % (m->height - 2);
        int x = 1 + rand() % (m->width - 2);
        if (y == 1 && x == 1) continue;
        if (m->grid[y][x] == TILE_EMPTY) {
            m->grid[y][x] = TILE_STONE; 
            placed_stones++;
        }
    }
}
void add_entity(Map *m, Entity e) {
    if (m->entity_count >= m->max_entities) return; 
    m->entities[m->entity_count] = e;
    m->entity_count++;
    m->grid[e.y][e.x] = e.symbol;
}
void generate_entities(Map *m, int floor) {
    int total_cells = (m->height - 2) * (m->width - 2);
    m->max_entities = (total_cells * 40) / 100 + 10;
    m->entities = (Entity*)malloc(m->max_entities * sizeof(Entity));
    m->entity_count = 0;
    int mult = 1 + (floor / 2);
    int target_trees = total_cells * 0.15;
    for (int i = 0; i < target_trees; i++) {
        int y = 1 + rand() % (m->height - 2);
        int x = 1 + rand() % (m->width - 2);
        if (m->grid[y][x] == '.') {
            Entity e;
            e.type = ENT_TREE;
            e.symbol = 'T';
            e.x = x; e.y = y;
            e.max_hp = 10 * mult; e.hp = e.max_hp;
            e.attack = 0; e.defense = 0;
            e.drop_item_id = 1; 
            e.is_alive = 1;
            add_entity(m, e);
        }
    }
    int target_rocks = total_cells * 0.15;
    for (int i = 0; i < target_rocks; i++) {
        int y = 1 + rand() % (m->height - 2);
        int x = 1 + rand() % (m->width - 2);
        if (m->grid[y][x] == '.') {
            Entity e;
            e.type = ENT_ROCK;
            e.symbol = 'R'; 
            e.x = x; e.y = y;
            e.max_hp = 20 * mult; e.hp = e.max_hp;
            e.attack = 0; e.defense = 0;
            e.drop_item_id = 2; 
            e.is_alive = 1;
            add_entity(m, e);
        }
    }
    int target_heals = total_cells * 0.05;
    for (int i = 0; i < target_heals; i++) {
        int y = 1 + rand() % (m->height - 2);
        int x = 1 + rand() % (m->width - 2);
        if (m->grid[y][x] == '.') {
            Entity e;
            e.type = ENT_HEAL;
            e.symbol = '+';
            e.x = x; e.y = y;
            e.max_hp = 1; e.hp = 1;
            e.attack = 0; e.defense = 0;
            e.drop_item_id = 0;
            e.is_alive = 1;
            add_entity(m, e);
        }
    }
    int target_monsters = total_cells * 0.05;
    for (int i = 0; i < target_monsters; i++) {
        int y = 1 + rand() % (m->height - 2);
        int x = 1 + rand() % (m->width - 2);
        // Не ставим монстра прямо на игрока
        if (m->grid[y][x] == '.' && !(x == 1 && y == 1)) {
            Entity e;
            e.type = ENT_MONSTER;
            e.symbol = 'M';
            e.x = x; e.y = y;
            e.max_hp = 15 * mult; e.hp = e.max_hp;
            e.attack = 3 * mult; 
            e.defense = 1 * mult;
            e.drop_item_id = 0; // Пока без лута
            e.is_alive = 1;
            add_entity(m, e);
        }
    }
}
Map* create_map(int height, int width, int floor) {
    Map *m = (Map*)malloc(sizeof(Map));
    if (m == NULL) return NULL;
    m->height = height;
    m->width = width;
    m->current_floor = floor;
    m->grid = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        m->grid[i] = (char*)malloc(width * sizeof(char));
        for (int j = 0; j < width; j++) {
            m->grid[i][j] = TILE_EMPTY;
        }
    }
    place_walls_and_stairs(m);
    generate_ice(m);   
    generate_entities(m, floor);
    return m;
}
void render_map(const Map *m, const Player *p, const GameTime *t, const char *last_event) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    printf("=== ПОДЗЕМЕЛЬЕ ===\n");
    printf("HP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n", 
           p->hp, p->max_hp, p->stamina, p->max_stamina, p->gold, m->current_floor, t->current_hour);
    printf("Фонарь: [Масло: %d/100] ", p->lantern.oil);
    if (p->lantern.mode == LIGHT_OFF) printf("Режим: ВЫКЛ\n");
    else if (p->lantern.mode == LIGHT_NORMAL) printf("Режим: ОБЫЧНЫЙ\n");
    else printf("Режим: ЯРКИЙ\n");
    printf("-----------------------------------\n");

    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            if (i == p->y && j == p->x) {
                printf("@");
            } else {
                int drawn = 0;
                for (int e = 0; e < m->entity_count; e++) {
                    if (m->entities[e].is_alive && m->entities[e].x == j && m->entities[e].y == i) {
                        printf("%c", m->entities[e].symbol); 
                        drawn = 1;
                        break; 
                    }
                }
                if (!drawn) {
                    printf("%c", m->grid[i][j]);
                }
            }
        }
        printf("\n");
    }
    printf("-----------------------------------\n");
    if (last_event != NULL && last_event[0] != '\0') {
        printf(">>> ПОСЛЕДНЕЕ СОБЫТИЕ: %s <<<\n", last_event);
    } else {
        printf("\n");
    }
}
int can_move(const Map *m, int new_y, int new_x) {
    if (new_y < 0 || new_y >= m->height || new_x < 0 || new_x >= m->width) {
        return 0; 
    }
    if (m->grid[new_y][new_x] == TILE_WALL || 
    m->grid[new_y][new_x] == TILE_TREE || 
    m->grid[new_y][new_x] == TILE_STONE) {
    return 0; 
        } 
    return 1;  
    }
void update_player_action(Player *p, Map *m, char action, char *last_event) {
    int nx = p->x;
    int ny = p->y;

    if (action == 'W' || action == 'w') ny--;
    else if (action == 'S' || action == 's') ny++;
    else if (action == 'A' || action == 'a') nx--;
    else if (action == 'D' || action == 'd') nx++;
    else return; 

    Entity *target = get_entity_at(m, nx, ny);

    if (target != NULL) {
        if (target->type == ENT_HEAL) {
            p->hp += 20; 
            if (p->hp > p->max_hp) p->hp = p->max_hp;
            p->stamina += 10; 
            if (p->stamina > p->max_stamina) p->stamina = p->max_stamina;
            
            target->is_alive = 0; 
            sprintf(last_event, "Вы собрали аптечку! (+20 HP, +10 стамины)");
            
        } else if (target->type == ENT_TREE || target->type == ENT_ROCK) {
            int dmg = p->attack - (target->defense / 2);
            if (dmg < 1) dmg = 1;
            
            target->hp -= dmg;
            
            if (target->hp <= 0) {
                target->is_alive = 0;
                int slot = -1;
                for (int i = 0; i < MAX_ITEMS; i++) {
                    if (p->inventory[i] == ITEM_EMPTY) { 
                        slot = i; break;
                    }
                }
                
                if (slot != -1) {
                    p->inventory[slot] = target->drop_item_id;
                    sprintf(last_event, "Вы разрушили объект и получили: %s!", get_item_name(target->drop_item_id));
                } else {
                    sprintf(last_event, "Рюкзак полон! Ресурс пропал.");
                }
            } else {
                sprintf(last_event, "Вы ударили объект на %d урона. (Осталось: %d)", dmg, target->hp);
            }
            
        } else if (target->type == ENT_MONSTER) {
            p->stamina -= 2; 
            int dmg = p->attack - (target->defense / 2);
            if (dmg < 1) dmg = 1;
            target->hp -= dmg;
            
            if (target->hp <= 0) {
                target->is_alive = 0;
                int reward_gold = 10 * m->current_floor;
                p->gold += reward_gold;
                sprintf(last_event, "Вы победили монстра! Получено %d золота.", reward_gold);
            } else {
                sprintf(last_event, "Вы ударили монстра на %d урона. (Осталось HP: %d)", dmg, target->hp);
            }
        }
        
    } else {
        if (can_move(m, nx, ny)) {
            p->x = nx;
            p->y = ny;
            sprintf(last_event, "Вы сделали шаг.");
            
            if (m->grid[p->y][p->x] == '~') {
                slide_on_ice(m, p, action);
                strcat(last_event, " Вы поскользнулись на льду!");
            }
        } else {
            sprintf(last_event, "Туда нельзя идти! (Стена)");
        }
    }
}
void make_step(Player *p, Map *m, char action) {
    p->stamina -= 1;
    int oil_cost = 0;
    if (p->lantern.mode == LIGHT_NORMAL) oil_cost = 1;
    else if (p->lantern.mode == LIGHT_BRIGHT) oil_cost = 2;
    
    if (p->lantern.oil >= oil_cost) {
        p->lantern.oil -= oil_cost;
    } else {
        p->lantern.oil = 0;
        if (p->lantern.mode != LIGHT_OFF) {
            printf("\n>>> Масло кончилось, фонарь погас! <<<\n");
        }
        p->lantern.mode = LIGHT_OFF;
    }
    if (p->lantern.mode == LIGHT_OFF) {
        if (rand() % 100 < 20) { 
            printf("\n>>> В темноте вы ударились о камни! (-5 HP, -2 стамины) <<<\n");
            p->hp -= 5;
            p->stamina -= 2;
        }
    }
    int new_y = p->y;
    int new_x = p->x;
    
    if (action == 'W' || action == 'w') new_y--;
    else if (action == 'S' || action == 's') new_y++;
    else if (action == 'A' || action == 'a') new_x--;
    else if (action == 'D' || action == 'd') new_x++;

    if (can_move(m, new_y, new_x)) {
        p->y = new_y;
        p->x = new_x;
        if (m->grid[p->y][p->x] == '~') {
            slide_on_ice(m, p, action); 
        }
        
    } else {
        printf("Туда нельзя идти! (Стена)\n");
    }
}
void update_monsters(Map *m, Player *p, char *last_event) {
    for (int i = 0; i < m->entity_count; i++) {
        if (m->entities[i].is_alive && m->entities[i].type == ENT_MONSTER) {
            Entity *e = &m->entities[i];
            int dist = abs(e->x - p->x) + abs(e->y - p->y);
            
            if (dist == 1) {
                int dmg = e->attack - (p->defense / 2);
                if (dmg < 1) dmg = 1;
                p->hp -= dmg;
                sprintf(last_event, "Монстр нанес вам %d урона!", dmg);
            } else {
                int dir = rand() % 4;
                int nx = e->x, ny = e->y;
                
                if (dir == 0) ny--;
                else if (dir == 1) ny++;
                else if (dir == 2) nx--;
                else if (dir == 3) nx++;
                
                if (m->grid[ny][nx] == '.' || (nx == p->x && ny == p->y)) {
                    if (nx == p->x && ny == p->y) {
                        int dmg = e->attack - (p->defense / 2);
                        if (dmg < 1) dmg = 1;
                        p->hp -= dmg;
                        sprintf(last_event, "Монстр набросился на вас и нанес %d урона!", dmg);
                    } else {
                        e->x = nx;
                        e->y = ny;
                    }
                }
            }
        }
    }
}
void start_adventure(Player *p, GameTime *t) {
    Map *m = create_map(MAP_HEIGHT, MAP_WIDTH, 1);
    p->x = 1; p->y = 1;
    
    char action;
    char last_event[128] = "Вы вошли в подземелье!"; 

    do {
        render_map(m, p, t, last_event); 
        
    
        printf("\n(W/A/S/D - действие, F - фонарь, Q - выход): ");
        action = get_action(); 
        
        if (action == 'Q' || action == 'q') {
            break;
        }
        
        if (action == 'F' || action == 'f') {
            printf("\nВыберите режим фонаря (0-Выкл, 1-Обычный, 2-Яркий): ");
            int l_choice = read_int_safe("", 0, 2);
            if (p->lantern.oil > 0 || l_choice == 0) {
                p->lantern.mode = (LightMode)l_choice;
                strcpy(last_event, "Вы переключили фонарь.");
            } else {
                strcpy(last_event, "Нет масла, чтобы зажечь!");
            }
            continue;
        }
        update_player_action(p, m, action, last_event);
        update_monsters(m, p, last_event);
        
        if (p->hp <= 0 || p->stamina <= 0) {
            printf("\n>>> Вы упали без сил! <<<\n");
            break;
        }

    } while (1);
    
    free_map(m);
    printf("\nНажмите Enter для возврата в меню...");
    while (getchar() != '\n');
}