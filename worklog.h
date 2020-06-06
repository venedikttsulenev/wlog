#ifndef WLOG_WORKLOG_H
#define WLOG_WORKLOG_H

#include <time.h>
#include "tasks.h"

typedef struct worklog_summary {
    int size;
    double *spent;
    double total_spent;
} worklog_summary_t;

void init_worklog();

double log_time_spent(time_t *since, task_id_t task_id);
worklog_summary_t get_worklog_summary();

void free_worklog();

#endif //WLOG_WORKLOG_H
