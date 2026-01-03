#ifndef TASK_H
#define TASK_H

typedef struct Task {
    int id;
    char title[100];
    char created[40];
    int deadline;
    int mark;
    struct Task* prev;
    struct Task* next;
} Task;

/* ---- function declarations ---- */
char *getTodayDate();
void loadTasksFromFile();
void saveTasksToFile();
void addTask(char title[], int deadline);
void deleteTask(int id);
void markTaskComplete(int id);
void sortByDeadline();

#endif
