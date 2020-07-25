#include "worklog.h"
#include <stdlib.h>
#include <string.h>

#define WL_DEFAULT_CAPACITY 128
#define WL_CAPACITY_GROW_STEP 32
#define WL_NOT_FOUND -1

static int wl_capacity = WL_DEFAULT_CAPACITY;
static int wl_size = 0;
static double wl_total_spent = 0;
static double *wl_spent = NULL;
static wl_tag_t *wl_tag = NULL;

void wl_init() {
    wl_spent = malloc(wl_capacity * sizeof(double));
    wl_tag = malloc(wl_capacity * sizeof(wl_tag_t));
}

void wl_grow() {
    wl_capacity += WL_CAPACITY_GROW_STEP;
    wl_spent = realloc(wl_spent, wl_capacity);
    wl_tag = realloc(wl_tag, wl_capacity);
}

int wl_index(char *tag) {
    for (int i = 0; i < wl_size; ++i) {
        if (strcmp(tag, wl_tag[i]) == 0) {
            return i;
        }
    }
    return WL_NOT_FOUND;
}

void wl_log(double seconds, char *tag) {
    int index = wl_index(tag);
    if (WL_NOT_FOUND == index) {
        if (wl_size == wl_capacity) {
            wl_grow();
        }
        index = wl_size;
        wl_spent[index] = 0;
        strncpy(wl_tag[index], tag, WL_MAX_TAG_LENGTH);
        wl_size += 1;
    }
    wl_spent[index] += seconds;
    wl_total_spent += seconds;
}

double wl_log_time_spent(time_t *since, char *tag) {
    time_t current = time(NULL);
    double delta = difftime(current, *since);
    *since = current;
    wl_log(delta, tag);
    return delta;
}

void wl_unlog(double seconds, char *tag) {
    int index = wl_index(tag);
    if (index != WL_NOT_FOUND) {
        double delta = seconds < wl_spent[index] ? seconds : wl_spent[index];
        wl_spent[index] -= delta;
        wl_total_spent -= delta;
    }
}

wl_summary_t wl_get_summary() {
    return (wl_summary_t) {wl_size, wl_total_spent, wl_spent, wl_tag};
}

void wl_clear() {
    wl_size = 0;
    wl_total_spent = 0;
}

void wl_free() {
    if (wl_spent) {
        free(wl_spent);
    }
    if (wl_tag) {
        free(wl_tag);
    }
}