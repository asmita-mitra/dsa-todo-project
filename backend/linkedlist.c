#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

char *getTodayDate();  
void saveTasksToFile();
void loadTasksFromFile();

Task* head = NULL;
Task* tail = NULL;
int nextId = 1;


void addTask(char title[], int deadline) {
    Task* newTask = malloc(sizeof(Task));

    newTask->id = nextId++;
    strcpy(newTask->title, title);
    strcpy(newTask->created, getTodayDate());  
    newTask->deadline = deadline;
    newTask->mark = 0;

    newTask->prev = tail;
    newTask->next = NULL;

    if (tail)
        tail->next = newTask;
    else
        head = newTask;

    tail = newTask;
    
    saveTasksToFile();
    
}


void displayTasks() {
    Task* curr = head;

    while (curr) {
        printf("ID:%d | %s | Created:%s | Deadline:%d | %s\n",
               curr->id,
               curr->title,
               curr->created,
               curr->deadline,
               curr->mark ? "Done" : "Pending");
        curr = curr->next;
    }
}


void markTaskComplete(int id) {
    Task* curr = head;

    while (curr) {
        if (curr->id == id) {
            curr->mark = 1;

            saveTasksToFile();
            return;
        }
        curr = curr->next;
    }
}


void deleteTask(int id) {
    Task* curr = head;

    while (curr) {
        if (curr->id == id) {
            if (curr->prev)
                curr->prev->next = curr->next;
            else
                head = curr->next;

            if (curr->next)
                curr->next->prev = curr->prev;
            else
                tail = curr->prev;

            free(curr);

            // ✅ SAVE AFTER DELETING
            saveTasksToFile();
            return;
        }
        curr = curr->next;
    }
}





Task* split(Task* head) {
    Task* fast = head;
    Task* slow = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    Task* second = slow->next;
    slow->next = NULL;
    if (second) second->prev = NULL;

    return second;
}

Task* merge(Task* first, Task* second) {
    if (!first) return second;
    if (!second) return first;

    if (first->deadline <= second->deadline) {
        first->next = merge(first->next, second);
        if (first->next) first->next->prev = first;
        first->prev = NULL;
        return first;
    } else {
        second->next = merge(first, second->next);
        if (second->next) second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}

Task* mergeSort(Task* head) {
    if (!head || !head->next)
        return head;

    Task* second = split(head);

    head = mergeSort(head);
    second = mergeSort(second);

    return merge(head, second);
}

void sortByDeadline() {
    head = mergeSort(head);

    Task* temp = head;
    while (temp && temp->next)
        temp = temp->next;
    tail = temp;


    saveTasksToFile();
}

void saveTasksToFile() {
    FILE* f = fopen("tasks.txt", "w");
    Task* curr = head;

    while (curr) {
        fprintf(f, "%d|%s|%s|%d|%d\n",
                curr->id,
                curr->title,
                curr->created,
                curr->deadline,
                curr->mark);
        curr = curr->next;
    }

    fclose(f);
}

void loadTasksFromFile() {
    FILE* f = fopen("tasks.txt", "r");
    if (!f) return;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        Task* t = malloc(sizeof(Task));

        sscanf(
            line,
            "%d|%99[^|]|%39[^|]|%d|%d",
            &t->id,
            t->title,
            t->created,
            &t->deadline,
            &t->mark
        );

        t->prev = tail;
        t->next = NULL;

        if (tail)
            tail->next = t;
        else
            head = t;

        tail = t;

        if (t->id >= nextId)
            nextId = t->id + 1;
    }

    fclose(f);
}
