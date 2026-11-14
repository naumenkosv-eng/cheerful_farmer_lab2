#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "utils.h"

void free_map(Map *m) {
    if (m == NULL) return;
    for (int i = 0; i < m->height; i++) {
        free(m->grid[i]);
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
    generate_obstacles(m);
    generate_ice(m);
    return m;
}
void draw_map(const Map *m, int player_y, int player_x) {
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            if (i == player_y && j == player_x) {
                printf("@"); // Игрок
            } else {
                printf("%c", m->grid[i][j]);
            }
        }
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
void start_adventure(Player *p, GameTime *t) {
    Map *m = create_map(MAP_HEIGHT, MAP_WIDTH, 1);
    p->x = 1;
    p->y = 1;

    char action;

    do {
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
        draw_map(m, p->y, p->x);
        
        printf("\n(W/A/S/D - идти, F - фонарь, Q - выход): ");
        action = get_action();

        if (action == 'Q' || action == 'q') {
            printf("Вы благополучно выбрались из подземелья! Золото сохранено.\n");
            break; 
        }
        if (action == 'F' || action == 'f') {
            printf("\nВыберите режим фонаря (0-Выкл, 1-Обычный, 2-Яркий): ");
            int l_choice = read_int_safe("", 0, 2);
            if (p->lantern.oil > 0 || l_choice == 0) {
                p->lantern.mode = (LightMode)l_choice;
            } else {
                printf("Нет масла, чтобы зажечь!\n");
                printf("Нажмите Enter...");
                while (getchar() != '\n');
            }
            continue;
        }
        make_step(p, m, action);

        if (p->stamina <= 0 || p->hp <= 0 || (t->current_hour >= 2 && t->current_hour < 8)) {
            printf("\n>>> Вы упали без сил! Спасатели доставили вас домой и забрали половину золота. <<<\n");
            p->gold = p->gold / 2;
            p->stamina = p->max_stamina;
            p->hp = p->max_hp;
            t->current_hour = 8; // Просыпаемся в 8 утра
            t->current_day++;
            break; 
        }
        if (m->grid[p->y][p->x] == TILE_EXIT) {
            m->current_floor++;
            t->current_hour += 1; 
            p->stamina -= 5;      
            int reward = 10 * m->current_floor; 
            
            if (p->lantern.mode == LIGHT_BRIGHT) {
                reward = (int)(reward * 1.5);
                printf("\n>>> Яркий свет фонаря помог найти тайник! Золото увеличено на 50%%! <<<\n");
            }
            
            p->gold += reward;
            printf("\n>>> Вы спустились на этаж %d! Получено %d золота. <<<\n", m->current_floor, reward);
            
            free_map(m);
            m = create_map(MAP_HEIGHT, MAP_WIDTH, m->current_floor); // Тут маленькая хитрость: current_floor передается при создании
            p->x = 1;
            p->y = 1;
            
            printf("Нажмите Enter для продолжения...");
            while (getchar() != '\n');
        }

    } while (1);

    free_map(m);
    printf("\nНажмите Enter для возврата в меню...");
    while (getchar() != '\n');
}