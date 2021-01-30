#ifndef WLOG_PRINT_H
#define WLOG_PRINT_H

#include "../worklog/worklog.h"
#include "../command/command.h"

void print_timer_started_message(char *task);

void print_timer_resumed_message(char *task);

void print_time_spent_message(double seconds, double total_seconds, char *task);

void print_summary(wl_summary_t summary);

void print_error(const char *message, const char *info);

void print_version();

void print_help(command_t *commands);

void print_imode_help(command_t *commands);

void print_greeting();

void print_worklog_cleared_message();

void print_logged_time_message(const char *task, double seconds);

void print_unlogged_time_message(const char *task, double seconds);

void print_task_deleted_message(const char *task);

#endif //WLOG_PRINT_H
