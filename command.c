#include <stdio.h>
#include <string.h>
#include <time.h>
#include "command.h"
#include "worklog.h"
#include "task.h"
#include "print.h"

#define error(code, info) (result_t) {code, info}
#define OK (result_t) {0, NULL}

#define ERR_UNKNOWN_COMMAND 1
#define ERR_INPUT 2
#define ERR_LOGIC 3
#define ERR_ARGUMENTS 4

#define MAX_INPUT_STR_LENGTH 256

#define INTERACTIVE_MODE_HELP \
"INTERACTIVE MODE COMMANDS\n\n" \
"  task name\n" \
"    Start work on task\n\n" \
"  break\n" \
"    Pause logging\n\n" \
"  report\n" \
"    Print logged work summary\n\n" \
"  stop\n" \
"    Print summary and quit\n\n" \
"  quit\n" \
"    Quit\n"

result_t interactive_mode_command() {
    static const char *TOKEN_DELIMETERS = " \t\n";
    task_list_t tasks;
    init_task_list(&tasks);
    worklog_t worklog;
    init_worklog(&worklog);
    char input_str[MAX_INPUT_STR_LENGTH];
    task_t current_task = NO_TASK;
    time_t time_started = 0;
    int stop = 0;
    do {
        putchar('>');
        putchar(' ');
        result_t result = OK;
        if (!fgets(input_str, MAX_INPUT_STR_LENGTH, stdin)) {
            result = error(ERR_INPUT, NULL);
        } else {
            char *tok = strtok(input_str, TOKEN_DELIMETERS);
            if (tok) {
                if (strcmp(tok, "task") == 0) {
                    char *arg = strtok(NULL, TOKEN_DELIMETERS);
                    if (!arg || !*arg) {
                        result = error(ERR_ARGUMENTS, "expected 1");
                    }
                    task_t new_task = find_or_create_task(arg, &tasks);
                    if (new_task != current_task) {
                        if (is_task(current_task)) {
                            double spent = log_time_spent_since(&time_started, &worklog, current_task);
                            print_time_spent_message(spent, tasks.tag[current_task]);
                        } else {
                            time_started = time(NULL);
                        }
                        current_task = new_task;
                        printf("Started work on %s\n", tasks.tag[current_task]);
                    }
                } else if (strcmp(tok, "break") == 0) {
                    if (no_task(current_task)) {
                        result = error(ERR_LOGIC, "there's no task in progress");
                    } else {
                        double spent = log_time_spent_since(&time_started, &worklog, current_task);
                        print_time_spent_message(spent, tasks.tag[current_task]);
                        current_task = BREAK_TASK;
                    }
                } else if (strcmp(tok, "report") == 0) {
                    print_summary(&worklog, &tasks);
                } else if (strcmp(tok, "stop") == 0) {
                    if (is_task(current_task)) {
                        log_time_spent_since(&time_started, &worklog, current_task);
                    }
                    stop = 1;
                } else if (strcmp(tok, "quit") == 0) {
                    return OK;
                } else if (strcmp(tok, "help") == 0) {
                    puts(INTERACTIVE_MODE_HELP);
                } else {
                    result = error(ERR_UNKNOWN_COMMAND, tok);
                }
            }
        }
        handle(result);
    } while (!stop);
    print_summary(&worklog, &tasks);
    return OK;
}

result_t print_help_command() {
    puts("SYNOPSIS\n\n"
         "  wlog [command]\n\n"
         "COMMANDS\n\n"
         "  wlog help (h)\n"
         "    Print help and quit\n\n"
         "  wlog\n"
         "    Enter interactive mode\n\n"
         "  wlog version (v)\n"
         "    Print version and quit\n\n"
         INTERACTIVE_MODE_HELP);
    return OK;
}

result_t print_version_command() {
    puts("wlog 0.1\033[2m, 17 May 2020\033[0m");
    return OK;
}

result_t unknown_command(int argc, char **argv) {
    return error(ERR_UNKNOWN_COMMAND, argv[1]);
}

static const int CMD_COUNT = 3;

static const command_t COMMANDS[CMD_COUNT] = {
        {interactive_mode_command, NULL, NULL},
        {print_help_command,    "help",    "h"},
        {print_version_command, "version", "v"}
};
static const command_t UNKNOWN_COMMAND = {unknown_command};

#define DEFAULT_COMMAND COMMANDS[0]

command_t command_for_str(char *str) {
    if (NULL == str)
        return DEFAULT_COMMAND;
    for (int i = 1; i < CMD_COUNT; ++i) {
        if (0 == strcmp(str, COMMANDS[i].shortname) || 0 == strcmp(str, COMMANDS[i].name))
            return COMMANDS[i];
    }
    return UNKNOWN_COMMAND;
}

static const char *ERR_MSG[] = {
        NULL,
        "Unknown command",
        "Input error",
        "Error",
        "Argument error"
};

int handle(result_t result) {
    if (result.code) {
        if (result.info) {
            printf("%s: %s\n", ERR_MSG[result.code], result.info);
        } else {
            printf("%s\n", ERR_MSG[result.code]);
        }
    }
    return result.code;
}