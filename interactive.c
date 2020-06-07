#include "interactive.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "print.h"
#include "worklog.h"
#include "tasks.h"

#define CMD_COUNT 8

static task_id_t i_current_task;
static task_id_t i_prev_task;
static time_t i_current_task_start_time;
static int i_stopped;
static command_t IMODE_COMMANDS[CMD_COUNT + 1];

char *generate_completions(const char *text, int state) {
    static int index = 0;
    if (state == 0) {
        index = 0;
    }
    while (index < CMD_COUNT) {
        command_t *command = &IMODE_COMMANDS[index++];
        if (strstr(command->name, text) == command->name) {
            char *compl = (char *) malloc(1 + strlen(command->name));
            strcpy(compl, command->name);
            return compl;
        }
    }
    return NULL;
}

char **get_completions(const char *text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, generate_completions);
}

void init_interactive_mode() {
    i_current_task = NO_TASK;
    i_prev_task = NO_TASK;
    i_current_task_start_time = 0;
    i_stopped = 0;
    rl_attempted_completion_function = get_completions;
    using_history();
    init_tasks();
    init_worklog();
}

#define PROMPT "\033[2m>\033[0m "
#define TOKEN_DELIMETERS " \t\n"

result_t run_interactive_mode() {
    char *input_str;
    init_interactive_mode();
    do {
        result_t result = OK;
        input_str = readline(PROMPT);
        if (input_str == NULL) {
            result = error(ERR_INPUT, NULL);
        } else {
            char *tok = strtok(input_str, TOKEN_DELIMETERS);
            if (tok) {
                add_history(input_str);
                if (*tok != '#') {
                    command_t command = command_for_name(tok, IMODE_COMMANDS, NULL);
                    result = execute_command(command);
                }
            }
            free(input_str);
        }
        handle(result);
    } while (!i_stopped);
    free_worklog();
    free_tasks();
    return OK;
}

result_t task_command() {
    char *arg = strtok(NULL, TOKEN_DELIMETERS);
    if (!arg || !*arg) {
        return error(ERR_ARGUMENTS, "expected 1");
    }

    task_id_t new_task = find_or_create_task(arg);
    if (new_task != i_current_task) {
        if (is_task(i_current_task)) {
            double spent = log_time_spent(&i_current_task_start_time, i_current_task);
            print_time_spent_message(spent, get_task_tag(i_current_task));
        } else {
            i_current_task_start_time = time(NULL);
        }
        i_current_task = new_task;
        print_task_started_message(get_task_tag(i_current_task));
    }
    return OK;
}

result_t break_command() {
    if (no_task(i_current_task)) {
        return error(ERR_LOGIC, "there's no task being worked on");
    }

    double spent = log_time_spent(&i_current_task_start_time, i_current_task);
    print_time_spent_message(spent, get_task_tag(i_current_task));
    i_prev_task = i_current_task;
    i_current_task = BREAK_TASK;
    return OK;
}

result_t continue_command() {
    if (i_current_task != BREAK_TASK) {
        return error(ERR_LOGIC, "there is already task being worked on");
    }

    i_current_task_start_time = time(NULL);
    i_current_task = i_prev_task;
    print_task_started_message(get_task_tag(i_current_task));
    return OK;
}

result_t report_command() {
    if (is_task(i_current_task)) {
        log_time_spent(&i_current_task_start_time, i_current_task);
    }
    print_summary(get_worklog_summary());
    return OK;
}

result_t stop_command() {
    if (is_task(i_current_task)) {
        log_time_spent(&i_current_task_start_time, i_current_task);
    }
    print_summary(get_worklog_summary());
    i_stopped = 1;
    return OK;
}

result_t quit_command() {
    i_stopped = 1;
    return OK;
}

result_t help_command() {
    print_imode_help(IMODE_COMMANDS);
    return OK;
}

result_t version_command() {
    print_version();
    return OK;
}

static command_t IMODE_COMMANDS[] = {
        {task_command,     "task",     "t",   "Start work on task", "name"},
        {break_command,    "break",    "br",  "Pause logging"},
        {continue_command, "continue", "co",  "Resume logging"},
        {report_command,   "report",   "rep", "Print logged work summary"},
        {stop_command,     "stop",     "st",  "Print summary and quit"},
        {quit_command,     "quit",     "q",   "Quit"},
        {help_command,     "help",     "h",   "Print help"},
        {version_command,  "version",  "v",   "Print version"},
        {}
};