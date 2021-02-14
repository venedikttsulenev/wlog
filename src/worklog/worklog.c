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

void wl_set_task(int index, const char* task) {
    strcpy(wl_task[index], task);
}

void wl_log(double seconds, const char *task) {
    int index = wl_task_index(task);
    if (WL_NOT_FOUND == index) {
        if (wl_size == wl_capacity) {
            wl_grow();
        }
        index = wl_size;
        wl_spent[index] = 0;
        wl_set_task(index, task);
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

void wl_delete(int index) {
    wl_total_spent -= wl_spent[index];
    wl_size -= 1;
    if (index != wl_size) {
        wl_spent[index] = wl_spent[wl_size];
        wl_set_task(index, wl_task[wl_size]);
    }
}

int wl_unlog(double seconds, const char *task) {
    int index = wl_task_index(task);
    if (index == WL_NOT_FOUND) {
        return 0;
    }
    if (seconds >= wl_spent[index]) {
        wl_delete(index);
    } else {
        wl_spent[index] -= seconds;
        wl_total_spent -= seconds;
    }
    return 1;
}

wl_summary_t wl_get_summary() {
    return (wl_summary_t) {wl_size, wl_total_spent, wl_spent, wl_task};
}

double wl_get_time_spent(const char *task) {
    int index = wl_task_index(task);
    return index != WL_NOT_FOUND ? wl_spent[index] : 0;
}

int wl_exists(const char *task) {
    return wl_task_index(task) != WL_NOT_FOUND;
}

int wl_rename_task(const char *task, const char *new_name) {
    int index = wl_task_index(task);
    if (index == WL_NOT_FOUND || wl_task_index(new_name) != WL_NOT_FOUND) {
        return 0;
    }
    strcpy(wl_task[index], new_name);
    return 1;
}

int wl_delete_task(const char *task) {
    int index = wl_task_index(task);
    if (index == WL_NOT_FOUND) {
        return 0;
    }
    wl_delete(index);
    return 1;
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