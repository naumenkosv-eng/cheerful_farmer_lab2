#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils.h"

int read_int_safe(const char* prompt, int min, int max) {
      int num;
      while (1) {
        printf("%s", prompt);
        if (scanf("%d", &num) != 1) {
            printf("Ошибка! Нужно ввести число.\n");
            while (getchar() != '\n'); 
            continue;
        }
        while (getchar() != '\n'); 
        if (num >= min && num <= max) {
            return num; 
        }
        printf("Ошибка! Число должно быть от %d до %d.\n", min, max);
    }
}
void remove_newline(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n' ||  str[i] == '\r')  {
            str[i] = '\0';
            break;
        }
    }
}
char get_action(void) {
    char action;
    scanf(" %c", &action); 
    while (getchar() != '\n');
    return action;
}
void print_current_time(void) {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", timeinfo);
    printf("Текущее время: %s\n", buffer);
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