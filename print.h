#ifndef WLOG_PRINT_H
#define WLOG_PRINT_H

void print_current_time(unsigned int offset);
void print_task_started_message(char *task_tag);
void print_time_spent_message(double seconds, char *tag_str);
void print_summary(worklog_t *worklog, task_list_t *task_list);
void print_interactive_mode_help();
void print_help();
void print_version();

#endif //WLOG_PRINT_H
