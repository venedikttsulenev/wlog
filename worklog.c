#include "worklog.h"

void init_worklog(worklog_t *worklog) {
    worklog->len = 0;
}

double log_time_spent(time_t *since, worklog_t *worklog, int task) {
    time_t current = time(NULL);
    double delta = difftime(current, *since);
    worklog_entry_t *log_entry = &worklog->entry[worklog->len++];
    *since = current;
    log_entry->time_spent = delta;
    log_entry->task = task;
    return delta;
}