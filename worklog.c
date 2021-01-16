#include "worklog.h"
#include <stdlib.h>
#include <string.h>

#define WL_DEFAULT_CAPACITY 128
#define WL_CAPACITY_GROW_STEP 32
#define WL_NOT_FOUND (-1)

static int wl_capacity = WL_DEFAULT_CAPACITY;
static int wl_size = 0;
static double wl_total_spent = 0;
static double *wl_spent = NULL;
static wl_task_t *wl_task = NULL;

void wl_init() {
    wl_spent = malloc(wl_capacity * sizeof(double));
    wl_task = malloc(wl_capacity * sizeof(wl_task_t));
}

void wl_grow() {
    wl_capacity += WL_CAPACITY_GROW_STEP;
    wl_spent = realloc(wl_spent, wl_capacity);
    wl_task = realloc(wl_task, wl_capacity);
}

int wl_task_index(const char *task) {
    for (int i = 0; i < wl_size; ++i) {
        if (strcmp(task, wl_task[i]) == 0) {
            return i;
        }
    }
    return WL_NOT_FOUND;
}

void wl_log(double seconds, const char *task) {
    int index = wl_task_index(task);
    if (WL_NOT_FOUND == index) {
        if (wl_size == wl_capacity) {
            wl_grow();
        }
        index = wl_size;
        wl_spent[index] = 0;
        strncpy(wl_task[index], task, WL_MAX_TASK_STR_LENGTH);
        wl_size += 1;
    }
    wl_spent[index] += seconds;
    wl_total_spent += seconds;
}

double wl_log_since(time_t *since, const char *task) {
    time_t current = time(NULL);
    double delta = difftime(current, *since);
    *since = current;
    wl_log(delta, task);
    return delta;
}

int wl_unlog(double seconds, const char *task) {
    int index = wl_task_index(task);
    if (index == WL_NOT_FOUND) {
        return 0;
    }
    double delta = seconds < wl_spent[index] ? seconds : wl_spent[index];
    wl_spent[index] -= delta;
    wl_total_spent -= delta;
    return 1;
}

wl_summary_t wl_get_summary() {
    return (wl_summary_t) {wl_size, wl_total_spent, wl_spent, wl_task};
}

void wl_clear() {
    wl_size = 0;
    wl_total_spent = 0;
}

void wl_free() {
    if (wl_spent) {
        free(wl_spent);
    }
    if (wl_task) {
        free(wl_task);
    }
}