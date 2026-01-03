#include <stdio.h>
#include <time.h>

char* getTodayDate() {
    static char buffer[40];
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}