#include "interactive.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "print.h"
#include "worklog.h"
#include "args.h"

#define CMD_COUNT 11

static wl_tag_t i_current_task;
static time_t i_current_task_start_time;
static int i_break;
static int i_no_task_yet;
static int i_stopped;
static double i_last_spent = 0;
static command_t IMODE_COMMANDS[CMD_COUNT + 1];

char *imode_generate_completions(const char *text, int state) {
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

char **imode_get_completions(const char *text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, imode_generate_completions);
}

void imode_init() {
    i_current_task_start_time = 0;
    i_break = 0;
    i_no_task_yet = 1;
    i_stopped = 0;
    rl_attempted_completion_function = imode_get_completions;
    using_history();
    wl_init();
}

void imode_free() {
    wl_free();
}

#define PROMPT "\033[2m>\033[0m "
#define TOKEN_DELIMETERS " \t\n"

result_t imode_run() {
    char *input_str;
    imode_init();
    print_greeting();
    do {
        result_t result = OK;
        input_str = readline(PROMPT);
        if (input_str == NULL) {
            result = error(ERR_INPUT, NULL);
        } else {
            add_history(input_str);
            char *tok = strtok(input_str, TOKEN_DELIMETERS);
            if (tok) {
                if (*tok != '#') {
                    command_t command = cmd_for_name(tok, IMODE_COMMANDS, NULL);
                    result = cmd_execute(command);
                }
            }
            free(input_str);
        }
        err_handle(result);
    } while (!i_stopped);
    imode_free();
    return OK;
}

result_t imode_task() {
    char *arg = strtok(NULL, TOKEN_DELIMETERS);
    if (!arg) {
        return error(ERR_ARGUMENTS, "expected 1");
    }

    if (strcmp(arg, i_current_task) != 0) {
        if (!i_break && !i_no_task_yet) {
            i_last_spent += wl_log_time_spent(&i_current_task_start_time, i_current_task);
            print_time_spent_message(i_last_spent, i_current_task);
        } else {
            i_no_task_yet = 0;
            i_break = 0;
            i_current_task_start_time = time(NULL);
        }
        strncpy(i_current_task, arg, WL_MAX_TAG_LENGTH);
        i_last_spent = 0;
        print_task_started_message(i_current_task);
    }
    return OK;
}

result_t imode_break() {
    if (i_break || i_no_task_yet) {
        return error(ERR_LOGIC, "there's no task being worked on");
    }

    i_last_spent += wl_log_time_spent(&i_current_task_start_time, i_current_task);
    print_time_spent_message(i_last_spent, i_current_task);
    i_break = 1;
    return OK;
}

result_t imode_continue() {
    if (i_no_task_yet) {
        return error(ERR_LOGIC, "there's no task to continue work on");
    }
    if (!i_break) {
        return error(ERR_LOGIC, "there is already task being worked on");
    }

    i_current_task_start_time = time(NULL);
    i_break = 0;
    i_last_spent = 0;
    print_task_started_message(i_current_task);
    return OK;
}

result_t imode_log() {
    char *spent_time = strtok(NULL, TOKEN_DELIMETERS);
    char *tag = strtok(NULL, TOKEN_DELIMETERS);
    if (!tag) {
        return error(ERR_ARGUMENTS, "expected 2");
    }

    double spent_seconds;
    error_t parse_error = args_time(spent_time, &spent_seconds);
    if (!err_occured(parse_error)) {
        wl_log(spent_seconds, tag);
    }

    return parse_error;
}

result_t imode_unlog() {
    char *time_str = strtok(NULL, TOKEN_DELIMETERS);
    char *tag = strtok(NULL, TOKEN_DELIMETERS);
    if (!tag) {
        return error(ERR_ARGUMENTS, "expected 2");
    }

    double seconds;
    error_t parse_error = args_time(time_str, &seconds);
    if (!err_occured(parse_error)) {
        wl_unlog(seconds, tag);
    }

    return parse_error;
}

result_t imode_report() {
    if (!i_break && !i_no_task_yet) {
        i_last_spent += wl_log_time_spent(&i_current_task_start_time, i_current_task);
    }
    print_summary(wl_get_summary());
    return OK;
}

result_t imode_clear() {
    imode_report();
    i_current_task_start_time = 0;
    i_break = 0;
    i_no_task_yet = 1;
    wl_clear();
    print_worklog_cleared_message();
    return OK;
}

result_t imode_stop() {
    imode_report();
    i_stopped = 1;
    return OK;
}

result_t imode_quit() {
    i_stopped = 1;
    return OK;
}

result_t imode_help() {
    print_imode_help(IMODE_COMMANDS);
    return OK;
}

result_t imode_version() {
    print_version();
    return OK;
}

static command_t IMODE_COMMANDS[] = {
        {imode_task,     "task",     "t",   "Start work on task", "task"},
        {imode_break,    "break",    "br",  "Pause logging"},
        {imode_continue, "continue", "co",  "Resume logging"},
        {imode_log,      "log",      "l",   "Log time instantly", "time task"},
        {imode_unlog,    "unlog",    "u",   "Unlog time", "time task"},
        {imode_report,   "report",   "rep", "Print logged time summary"},
        {imode_clear,    "clear",    "cl",  "Print summary and reset worklog (clear all previous data)"},
        {imode_stop,     "stop",     "st",  "Print summary and quit"},
        {imode_quit,     "quit",     "q",   "Quit"},
        {imode_help,     "help",     "h",   "Print help"},
        {imode_version,  "version",  "v",   "Print version"},
        {}
};