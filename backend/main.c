#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>     
#include "task.h"

/* backend function declarations */
void addTask(char[], int);
void markTaskComplete(int);
void deleteTask(int);
void sortByDeadline();
void loadTasksFromFile();

int main(int argc, char *argv[]) {

    loadTasksFromFile();

    if (argc < 2) {
        return 0;
    }

    if (strcmp(argv[1], "add") == 0 && argc == 4) {
        addTask(argv[2], atoi(argv[3]));
    }
    else if (strcmp(argv[1], "complete") == 0 && argc == 3) {
        markTaskComplete(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "delete") == 0 && argc == 3) {
        deleteTask(atoi(argv[2]));
    }
    else if (strcmp(argv[1], "sort") == 0) {
        sortByDeadline();
    }

    return 0;
}
