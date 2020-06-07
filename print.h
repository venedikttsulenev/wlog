#ifndef WLOG_PRINT_H
#define WLOG_PRINT_H

#include "worklog.h"
#include "command.h"

void print_current_time();
void print_task_started_message(char *task_tag);
void print_time_spent_message(double seconds, char *task_tag);
void print_summary(worklog_summary_t summary);
void print_error(const char *message, const char *info);
void print_version();
void print_help(command_t *commands);
void print_imode_help(command_t *commands);
void print_greeting();

#endif //WLOG_PRINT_H
