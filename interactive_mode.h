#ifndef WLOG_INTERACTIVE_MODE_H
#define WLOG_INTERACTIVE_MODE_H

#include "task.h"
#include "worklog.h"
#include "time.h"
#include "error.h"

#define IMODE_TOKEN_DELIMETERS " \t\n"

typedef struct interactive_mode_state {
    task_list_t tasks;
    worklog_t worklog;
    task_t current_task;
    task_t prev_task;
    time_t time_started;
    int stopped;
} imode_state_t;

typedef struct imode_command {
    error_t (*execute)(imode_state_t *);
    const char *name;
    const char *description;
    const char *arg_description;
} imode_command_t;

void init_interactive_mode(imode_state_t *state);
imode_command_t *get_imode_command_for_str(char *str);

imode_command_t *get_imode_commands();

#endif //WLOG_INTERACTIVE_MODE_H
