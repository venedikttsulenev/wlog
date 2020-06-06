#include "worklog.h"
#include <stdlib.h>

#define WL_CAPACITY_GROW_STEP 128
#define SUMMARY_CAPACITY_GROW_STEP 32

typedef struct worklog_entry {
    task_id_t task_id;
    double time_spent;
} worklog_entry_t;

static int wl_length = 0;
static int wl_capacity = 128;
static worklog_entry_t *wl_entry = NULL;
static worklog_summary_t wl_summary;
static int wl_summary_capacity = 32;
static int wl_last_summary_worklog_length = 0;

void grow_worklog() {
    wl_capacity += WL_CAPACITY_GROW_STEP;
    wl_entry = realloc(wl_entry, wl_capacity);
}

void grow_summary() {
    wl_summary_capacity += SUMMARY_CAPACITY_GROW_STEP;
    wl_summary.spent = realloc(wl_summary.spent, wl_summary_capacity * sizeof(double));
}

void init_worklog() {
    wl_entry = malloc(wl_capacity * sizeof(worklog_entry_t));
    wl_summary.size = 0;
    wl_summary.total_spent = 0;
    wl_summary.spent = calloc(wl_summary_capacity, sizeof(double));
}

double log_time_spent(time_t *since, int task_id) {
    if (wl_length == wl_capacity) {
        grow_worklog();
    }
    time_t current = time(NULL);
    double delta = difftime(current, *since);
    worklog_entry_t *log_entry = &wl_entry[wl_length++];
    *since = current;
    log_entry->time_spent = delta;
    log_entry->task_id = task_id;
    return delta;
}

worklog_summary_t get_worklog_summary() {
    for (int i = wl_last_summary_worklog_length; i < wl_length; ++i) {
        task_id_t task_id = wl_entry[i].task_id;
        while (task_id >= wl_capacity) {
            grow_summary();
        }
        double spent = wl_entry[i].time_spent;
        wl_summary.spent[task_id] += spent;
        wl_summary.total_spent += spent;
        if (task_id >= wl_summary.size) {
            wl_summary.size = task_id + 1;
        }
    }
    wl_last_summary_worklog_length = wl_length;
    return wl_summary;
}

void free_worklog() {
    free(wl_entry);
    free(wl_summary.spent);
}