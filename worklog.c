#include "worklog.h"
#include <stdlib.h>
#include <string.h>

#define WL_CAPACITY_GROW_STEP 32

static int wl_init_ran = 0;

static int wl_capacity = 128;
static wl_summary_t wl_summary = {0, 0, NULL};

void grow_worklog() {
    wl_capacity += WL_CAPACITY_GROW_STEP;
    wl_summary.spent = realloc(wl_summary.spent, wl_capacity);
    memset(wl_summary.spent + wl_capacity - WL_CAPACITY_GROW_STEP, 0, WL_CAPACITY_GROW_STEP * sizeof(double));
}

void wl_init() {
    wl_summary.spent = calloc(wl_capacity, sizeof(double));
    wl_init_ran = 1;
}

double wl_log_time_spent(time_t *since, task_id_t task_id) {
    if (!wl_init_ran) {
        wl_init();
    }
    if (task_id >= wl_summary.size) {
        if (task_id >= wl_capacity) {
            grow_worklog();
        }
        wl_summary.size = task_id + 1;
    }
    time_t current = time(NULL);
    double delta = difftime(current, *since);
    *since = current;
    wl_summary.spent[task_id] += delta;
    wl_summary.total_spent += delta;
    return delta;
}

wl_summary_t wl_get_summary() {
    return wl_summary;
}

double wl_get_time_spent(task_id_t task_id) {
    return wl_summary.spent[task_id];
}

void wl_free() {
    if (wl_summary.spent) {
        free(wl_summary.spent);
    }
}