#include "interactive.h"
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "../util/print.h"
#include "args.h"
#include "../util/str.h"

#define CMD_COUNT 15

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

void err_task_not_found(const char *name) {
    err_set(ERR_LOGIC, format_str("there's no task '%s'", name));
}

#define PROMPT "\033[2m>\033[0m "

void imode_handle_error() {
    err_print();
    err_reset();
}

int is_current_task(const char *task) {
    return i_no_task_yet ? 0 : strcmp(i_current_task, task) == 0;
}

void set_current_task(const char *task) {
    strcpy(i_current_task, task);
}

void imode_run() {
    char *input_str;
    imode_init();
    print_greeting();
    do {
        input_str = readline(PROMPT);
        if (input_str == NULL) {
            err_set(ERR_INPUT, NULL);
        } else {
            add_history(input_str);
            char *tok = strtok(input_str, TOKEN_DELIMETERS);
            if (tok && *tok != '#') {
                command_t command = cmd_for_name(tok, IMODE_COMMANDS, NULL);
                cmd_execute(command);
            }
            imode_handle_error();
            free(input_str);
        }
    } while (!i_stopped);
    imode_free();
}

void imode_break() {
    if (i_break || i_no_task_yet) {
        err_set(ERR_LOGIC, "there's no task being worked on");
        return;
    }

    i_last_spent += wl_log_since(&i_current_task_start_time, i_current_task);
    print_time_spent_message(i_last_spent, wl_get_time_spent(i_current_task), i_current_task);
    i_break = 1;
}

void imode_continue() {
    if (i_no_task_yet) {
        err_set(ERR_LOGIC, "there's no task to continue work on");
        return;
    }
    if (!i_break) {
        err_set(ERR_LOGIC, "there is already task being worked on");
        return;
    }

    i_current_task_start_time = time(NULL);
    i_break = 0;
    i_last_spent = 0;
    print_timer_resumed_message(i_current_task);
}

void imode_log() {
    args_t args = args_get(2, ARG_TASK, ARG_TIME);
    if (!err_occured()) {
        wl_log(args.time_seconds, args.task);
        print_logged_time_message(args.task, args.time_seconds);
    }
}

void imode_timer() {
    args_t args = args_get(1, ARG_TASK);
    if (err_occured()) { return; }
    if (is_current_task(args.task)) {
        return imode_continue();
    } else {
        if (!i_break && !i_no_task_yet) {
            i_last_spent += wl_log_since(&i_current_task_start_time, i_current_task);
            print_time_spent_message(i_last_spent, wl_get_time_spent(i_current_task), i_current_task);
        } else {
            i_no_task_yet = 0;
            i_break = 0;
            i_current_task_start_time = time(NULL);
        }
        set_current_task(args.task);
        i_last_spent = 0;
        print_timer_started_message(i_current_task);
    }
}

void imode_current() {
    if (i_no_task_yet) {
        print_no_task_message();
    } else {
        if (!i_break) {
            i_last_spent += wl_log_since(&i_current_task_start_time, i_current_task);
        }
        print_task_current_message(i_current_task, i_last_spent, wl_get_time_spent(i_current_task), i_break);
    }
}

void imode_unlog() {
    args_t args = args_get(2, ARG_TASK, ARG_TIME);
    if (err_occured()) { return; }

    wl_log_since(&i_current_task_start_time, i_current_task);
    if (wl_unlog(args.time_seconds, args.task)) {
        print_unlogged_time_message(args.task, args.time_seconds);
    } else {
        err_task_not_found(args.task);
    }
}

void imode_rename() {
    args_t args = args_get(2, ARG_TASK, ARG_TASK2);
    if (err_occured()) {return;}

    int cur_task_renamed = 0;
    if (is_current_task(args.task)) {
        set_current_task(args.task2);
        cur_task_renamed = 1;
    }
    if (wl_rename_task(args.task, args.task2) || cur_task_renamed) {
        print_task_renamed_message(args.task, args.task2);
    } else {
        err_set(ERR_LOGIC, format_str("can't rename '%s' to '%s': there's no task '%s' or task '%s' already exists", args.task, args.task2, args.task, args.task2));
    }
}

void imode_merge() {
    args_t args = args_get(2, ARG_TASK, ARG_TASK2);
    if (err_occured()) { return; }

    if (!(wl_exists(args.task) || is_current_task(args.task))) {
        err_task_not_found(args.task);
        return;
    }
    if (!(wl_exists(args.task2) || is_current_task(args.task2))) {
        err_task_not_found(args.task2);
        return;
    }
    if (is_current_task(args.task)) {
        i_no_task_yet = 1;
    }
    double task_time = wl_get_time_spent(args.task);
    wl_log(task_time, args.task2);
    wl_delete_task(args.task);
    print_task_merged_message(args.task, args.task2, task_time);
}

void imode_delete() {
    args_t args = args_get(1, ARG_TASK);
    if (err_occured()) {return;}

    int was_current = 0;
    if (is_current_task(args.task)) {
        i_no_task_yet = 1;
        was_current = 1;
    }
    if (!wl_delete_task(args.task) && !was_current) {
        err_task_not_found(args.task);
        return;
    }
    print_task_deleted_message(args.task);
}

void imode_report() {
    if (!i_break && !i_no_task_yet) {
        i_last_spent += wl_log_since(&i_current_task_start_time, i_current_task);
    }
    print_summary(wl_get_summary());
}

void imode_clear() {
    imode_report();
    i_current_task_start_time = 0;
    i_break = 0;
    i_no_task_yet = 1;
    wl_clear();
    print_worklog_cleared_message();
}

void imode_stop() {
    imode_report();
    i_stopped = 1;
}

void imode_quit() {
    i_stopped = 1;
}

void imode_help() {
    print_imode_help(IMODE_COMMANDS);
}

void imode_version() {
    print_version();
}

static command_t IMODE_COMMANDS[] = {
        {imode_timer,    "timer",    "t",   "Start timer for 'task'. There can only be one active timer.", "task"},
        {imode_break,    "break",    "br",  "Pause timer"},
        {imode_continue, "continue", "co",  "Resume timer"},
        {imode_current,  "current",  "cur", "Print active timer info"},
        {imode_log,      "log",      "l",   "Log time", "task time"},
        {imode_unlog,    "unlog",    "u",   "Unlog time", "task time"},
        {imode_rename,   "rename",   "rn",  "Rename 'task' to 'task2'", "task task2"},
        {imode_merge,    "merge",    "mr",  "Merge 'task' into 'task2': delete 'task' and add time logged in it to 'task2'.", "task task2"},
        {imode_delete,   "delete",   "del", "Delete 'task' log. If there was a timer for 'task', it gets discarded.", "task"},
        {imode_report,   "report",   "rep", "Print logged time summary"},
        {imode_clear,    "clear",    "cl",  "Print summary and clear worklog"},
        {imode_stop,     "stop",     "st",  "Print summary and quit"},
        {imode_quit,     "quit",     "q",   "Quit"},
        {imode_help,     "help",     "h",   "Print help"},
        {imode_version,  "version",  "v",   "Print version"},
        {}
};