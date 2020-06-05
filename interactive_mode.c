#include "interactive_mode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "print.h"

#define TOKEN_DELIMETERS " \t\n"

static task_list_t tasks;
static worklog_t worklog;
static task_t current_task;
static task_t prev_task;
static time_t current_task_start_time;
static int stopped;

char *get_completion(const char *text, int state) {
    if (state == 0) {
        for (command_t *command = get_imode_commands(); command_exists(command); ++command) {
            if (strstr(command->name, text) == command->name) {
                rl_completion_append_character = '\0';
                char *compl = (char *) malloc(1 + strlen(command->name));
                strcpy(compl, command->name);
                return compl;
            }
        }
    }
    return NULL;
}

void init_interactive_mode() {
    init_task_list(&tasks);
    init_worklog(&worklog);
    current_task = NO_TASK;
    prev_task = NO_TASK;
    current_task_start_time = 0;
    stopped = 0;
    rl_completion_entry_function = get_completion;
    using_history();
}

result_t run_interactive_mode() {
    static const char *PROMPT = "> ";
    char *input_str;
    init_interactive_mode();
    do {
        result_t result = OK;
        input_str = readline(PROMPT);
        if (!input_str) {
            result = error(ERR_INPUT, NULL);
        } else {
            add_history(input_str);
            char *tok = strtok(input_str, TOKEN_DELIMETERS);
            command_t command = get_imode_command(tok);
            print_current_time();
            result = command.execute(0, NULL);
            free(input_str);
        }
        handle(result);
    } while (!stopped);
    return OK;
}

result_t task_command() {
    char *arg = strtok(NULL, TOKEN_DELIMETERS);
    if (!arg || !*arg)
        return error(ERR_ARGUMENTS, "expected 1");

    task_t new_task = find_or_create_task(arg, &tasks);
    if (new_task != current_task) {
        if (is_task(current_task)) {
            double spent = log_time_spent(&current_task_start_time, &worklog, current_task);
            print_time_spent_message(spent, tasks.tag[current_task]);
        } else {
            current_task_start_time = time(NULL);
        }
        current_task = new_task;
        print_task_started_message(tasks.tag[current_task]);
    }
    return OK;
}

result_t break_command() {
    if (no_task(current_task))
        return error(ERR_LOGIC, "there's no task in progress");

    double spent = log_time_spent(&current_task_start_time, &worklog, current_task);
    print_time_spent_message(spent, tasks.tag[current_task]);
    prev_task = current_task;
    current_task = BREAK_TASK;
    return OK;
}

result_t continue_command() {
    if (current_task != BREAK_TASK)
        return error(ERR_LOGIC, "there is no break to continue after");

    current_task_start_time = time(NULL);
    current_task = prev_task;
    print_task_started_message(tasks.tag[current_task]);
    return OK;
}

result_t report_command() {
    double spent = difftime(time(NULL), current_task_start_time);
    print_summary(&worklog, &tasks, current_task, spent);
    return OK;
}

result_t stop_command() {
    if (is_task(current_task)) {
        log_time_spent(&current_task_start_time, &worklog, current_task);
    }
    print_summary(&worklog, &tasks, NO_TASK, 0);
    stopped = 1;
    return OK;
}

result_t quit_command() {
    stopped = 1;
    return OK;
}

result_t help_command() {
    print_interactive_mode_help();
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
        {}
};

command_t get_imode_command(char *name) {
    return command_for_name(name, IMODE_COMMANDS, NULL);
}

command_t *get_imode_commands() {
    return IMODE_COMMANDS;
}