#include "interactive.h"
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "print.h"
#include "worklog.h"
#include "args.h"
#include "str.h"

#define CMD_COUNT 11

static wl_task_t i_current_task;
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
            if (tok && *tok != '#') {
                command_t command = cmd_for_name(tok, IMODE_COMMANDS, NULL);
                result = cmd_execute(command);
            }
            free(input_str);
        }
        err_handle(result);
    } while (!i_stopped);
    imode_free();
    return OK;
}

result_t imode_break() {
    if (i_break || i_no_task_yet) {
        return error(ERR_LOGIC, "there's no task being worked on");
    }

    i_last_spent += wl_log_since(&i_current_task_start_time, i_current_task);
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
    print_timer_resumed_message(i_current_task);
    return OK;
}

result_t imode_log() {
    args_t args;
    result_t result = args_time_and_task(&args);
    if (!err_occured(result)) {
        wl_log(args.time_seconds, args.task);
        print_logged_time_message(args.task, args.time_seconds);
    }
    return result;
}

result_t imode_timer() {
    args_t args;
    result_t result = args_task(&args);
    if (!err_occured(result)) {
        if (strcmp(args.task, i_current_task) == 0) {
            return imode_continue();
        }
        else {
            if (!i_break && !i_no_task_yet) {
                i_last_spent += wl_log_since(&i_current_task_start_time, i_current_task);
                print_time_spent_message(i_last_spent, i_current_task);
            } else {
                i_no_task_yet = 0;
                i_break = 0;
                i_current_task_start_time = time(NULL);
            }
            strncpy(i_current_task, args.task, WL_MAX_TASK_STR_LENGTH);
            i_last_spent = 0;
            print_timer_started_message(i_current_task);
        }
    }
    return result;
}

result_t imode_unlog() {
    args_t args;
    result_t result = args_time_and_task(&args);
    if (!err_occured(result)) {
        if (wl_unlog(args.time_seconds, args.task)) {
            print_unlogged_time_message(args.task, args.time_seconds);
        } else {
            result = error(ERR_LOGIC, format_str("There's no task '%s'", args.task));
        }
    }
    return result;
}

result_t imode_report() {
    if (!i_break && !i_no_task_yet) {
        i_last_spent += wl_log_since(&i_current_task_start_time, i_current_task);
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
        {imode_timer,    "timer",    "t",   "Start timer for task", "task"},
        {imode_break,    "break",    "br",  "Pause timer"},
        {imode_continue, "continue", "co",  "Resume timer"},
        {imode_log,      "log",      "l",   "Log time",             "task time"},
        {imode_unlog,    "unlog",    "u",   "Unlog time",           "task time"},
        {imode_report,   "report",   "rep", "Print logged time summary"},
        {imode_clear,    "clear",    "cl",  "Print summary and reset worklog (clear all previous data)"},
        {imode_stop,     "stop",     "st",  "Print summary and quit"},
        {imode_quit,     "quit",     "q",   "Quit"},
        {imode_help,     "help",     "h",   "Print help"},
        {imode_version,  "version",  "v",   "Print version"},
        {}
};