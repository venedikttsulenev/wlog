#include "interactive_mode.h"
#include <stdio.h>
#include <string.h>
#include "print.h"

void init_interactive_mode(imode_state_t *state) {
    init_task_list(&state->tasks);
    init_worklog(&state->worklog);
    state->current_task = NO_TASK;
    state->prev_task = NO_TASK;
    state->time_started = 0;
    state->stopped = 0;
}

error_t task_command(imode_state_t *state) {
    char *arg = strtok(NULL, IMODE_TOKEN_DELIMETERS);
    if (!arg || !*arg)
        return error(ERR_ARGUMENTS, "expected 1");

    task_t new_task = find_or_create_task(arg, &state->tasks);
    if (new_task != state->current_task) {
        if (is_task(state->current_task)) {
            double spent = log_time_spent(&state->time_started, &state->worklog, state->current_task);
            print_time_spent_message(spent, state->tasks.tag[state->current_task]);
        } else {
            state->time_started = time(NULL);
        }
        state->current_task = new_task;
        print_task_started_message(state->tasks.tag[state->current_task]);
    }
    return OK;
}

error_t break_command(imode_state_t *state) {
    if (no_task(state->current_task))
        return error(ERR_LOGIC, "there's no task in progress");

    double spent = log_time_spent(&state->time_started, &state->worklog, state->current_task);
    print_time_spent_message(spent, state->tasks.tag[state->current_task]);
    state->prev_task = state->current_task;
    state->current_task = BREAK_TASK;
    return OK;
}

error_t continue_command(imode_state_t *state) {
    if (state->current_task != BREAK_TASK)
        return error(ERR_LOGIC, "there is no break to continue after");

    state->time_started = time(NULL);
    state->current_task = state->prev_task;
    print_task_started_message(state->tasks.tag[state->current_task]);
    return OK;
}

error_t report_command(imode_state_t *state) {
    print_summary(&state->worklog, &state->tasks);
    return OK;
}

error_t stop_command(imode_state_t *state) {
    if (is_task(state->current_task)) {
        log_time_spent(&state->time_started, &state->worklog, state->current_task);
    }
    print_summary(&state->worklog, &state->tasks);
    state->stopped = 1;
    return OK;
}

error_t quit_command(imode_state_t *state) {
    state->stopped = 1;
    return OK;
}

error_t help_command() {
    print_interactive_mode_help();
    return OK;
}

static imode_command_t IMODE_COMMANDS[] = {
        {task_command,     "task",     "Start work on task", "name"},
        {break_command,    "break",    "Pause logging"},
        {continue_command, "continue", "Resume logging"},
        {report_command,   "report",   "Print logged work summary"},
        {stop_command,     "stop",     "Print summary and quit"},
        {quit_command,     "quit",     "Quit"},
        {help_command,     "help",     "Print help"},
        {}
};

imode_command_t *get_imode_command_for_str(char *str) {
    if (str) {
        for (imode_command_t *command = IMODE_COMMANDS; command->name; ++command) {
            if (strcmp(str, command->name) == 0) {
                return command;
            }
        }
    }
    return NULL;
}

imode_command_t *get_imode_commands() {
    return IMODE_COMMANDS;
}