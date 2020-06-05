#ifndef WLOG_PRINT_H
#define WLOG_PRINT_H

void print_current_time();
void print_task_started_message(char *task_tag);
void print_time_spent_message(double seconds, char *tag_str);
void print_summary(worklog_t *worklog, task_list_t *task_list, task_t current_task, double spent_on_current_task);
void print_interactive_mode_help();
void print_help();
void print_version();

#endif //WLOG_PRINT_H
