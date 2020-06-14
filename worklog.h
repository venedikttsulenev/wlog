#ifndef WLOG_WORKLOG_H
#define WLOG_WORKLOG_H

#include <time.h>
#include "tasks.h"

typedef struct wl_summary {
    int size;
    double total_spent;
    double *spent;
} wl_summary_t;

double wl_log_time_spent(time_t *since, task_id_t task_id);
double wl_get_time_spent(task_id_t task_id);
wl_summary_t wl_get_summary();

void wl_free();

#endif //WLOG_WORKLOG_H
