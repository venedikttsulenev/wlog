#ifndef WLOG_PRINT_H
#define WLOG_PRINT_H

void print_time_interval(double seconds);
void print_time_spent_message(double seconds, char *tag_str);
void print_summary(worklog_t *worklog, task_list_t *task_list);

#endif //WLOG_PRINT_H
