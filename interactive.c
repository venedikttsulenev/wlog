#include "interactive.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "print.h"
#include "worklog.h"

#define CMD_COUNT 8

static wl_tag_t i_current_task;
static time_t i_current_task_start_time;
static int i_break;
static int i_no_task_yet;
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
    i_current_task_start_time = 0;
    i_break = 0;
    i_no_task_yet = 1;
    i_stopped = 0;
    rl_attempted_completion_function = get_completions;
    using_history();
}

#define PROMPT "\033[2m>\033[0m "
#define TOKEN_DELIMETERS " \t\n"

result_t run_interactive_mode() {
    char *input_str;
    init_interactive_mode();
    print_greeting();
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
                    command_t command = cmd_for_name(tok, IMODE_COMMANDS, NULL);
                    result = cmd_execute(command);
                }
            }
            free(input_str);
        }
        handle(result);
    } while (!i_stopped);
    wl_free();
    return OK;
}

result_t task_command() {
    char *arg = strtok(NULL, TOKEN_DELIMETERS);
    if (!arg) {
        return error(ERR_ARGUMENTS, "expected 1");
    }

    if (strcmp(arg, i_current_task) != 0) {
        if (!i_break && !i_no_task_yet) {
            wl_log_time_spent(&i_current_task_start_time, i_current_task);
            print_time_spent_message(wl_get_time_spent(i_current_task), i_current_task);
        } else {
            i_no_task_yet = 0;
            i_break = 0;
            i_current_task_start_time = time(NULL);
        }
        strncpy(i_current_task, arg, WL_MAX_TAG_LENGTH);
        print_task_started_message(i_current_task);
    }
    return OK;
}

result_t break_command() {
    if (i_break || i_no_task_yet) {
        return error(ERR_LOGIC, "there's no task being worked on");
    }

    wl_log_time_spent(&i_current_task_start_time, i_current_task);
    print_time_spent_message(wl_get_time_spent(i_current_task), i_current_task);
    i_break = 1;
    return OK;
}

result_t continue_command() {
    if (i_no_task_yet) {
        return error(ERR_LOGIC, "there's no task to continue work on");
    }
    if (!i_break) {
        return error(ERR_LOGIC, "there is already task being worked on");
    }

    i_current_task_start_time = time(NULL);
    i_break = 0;
    print_task_started_message(i_current_task);
    return OK;
}

result_t report_command() {
    if (!i_break && !i_no_task_yet) {
        wl_log_time_spent(&i_current_task_start_time, i_current_task);
    }
    print_summary(wl_get_summary());
    return OK;
}

result_t stop_command() {
    report_command();
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