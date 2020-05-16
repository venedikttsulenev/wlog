#ifndef WLOG_WORKLOG_H
#define WLOG_WORKLOG_H

#include <time.h>

#define MAX_WORKLOG_LEN 128

typedef struct worklog_entry {
    int task;
    double time_spent;
} worklog_entry_t;

typedef struct worklog {
    int len;
    worklog_entry_t entry[MAX_WORKLOG_LEN];
} worklog_t;

void init_worklog(worklog_t *worklog);

double log_time_spent_since(time_t *timestamp, worklog_t *worklog, int task);

#endif //WLOG_WORKLOG_H
