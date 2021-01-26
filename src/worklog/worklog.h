#ifndef WLOG_WORKLOG_H
#define WLOG_WORKLOG_H

#include <time.h>

#define WL_MAX_TASK_STR_LENGTH 32

typedef char wl_task_t[WL_MAX_TASK_STR_LENGTH];

typedef struct wl_summary {
    int size;
    double total_spent;
    double *spent;
    wl_task_t *task;
} wl_summary_t;

void wl_init();

void wl_log(double seconds, const char *task);

double wl_log_since(time_t *since, const char *task);

int wl_unlog(double seconds, const char *task);

wl_summary_t wl_get_summary();

void wl_clear();

void wl_free();

#endif //WLOG_WORKLOG_H
