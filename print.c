#include "print.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "version.h"

#define STYLE_NORMAL "\033[0m"
#define STYLE_BOLD "\033[1m"
#define STYLE_FAINT "\033[2m"
#define STYLE_UNDERLINED "\033[4m"

#define COLOR_RED "\033[38;5;52m"
#define COLOR_GREY "\033[38;5;241m"
#define COLOR_YELLOW "\033[38;5;94m"
#define COLOR_GREEN "\033[38;5;22m"

#define BUFFER_SIZE 16

char *get_buffer() {
    static char buffer[BUFFER_SIZE][256]; // BUFFER_SIZE strings of length 256
    static int free_buffer_index = 0;

    char *ret = buffer[free_buffer_index++];
    if (free_buffer_index >= BUFFER_SIZE) {
        free_buffer_index = 0;
    }
    return ret;
}

char *current_time_str() {
    char *str = get_buffer();
    time_t t = time(NULL);
    const struct tm *tm = localtime(&t);
    strftime(str, 6, "%H:%M", tm);
    char *colored_str = get_buffer();
    sprintf(colored_str, COLOR_GREY"%s"STYLE_NORMAL, str);
    return colored_str;
}

char *format_task(const char *task) {
    char *str = get_buffer();
    sprintf(str, COLOR_GREEN"%s"STYLE_NORMAL, task);
    return str;
}

char *format_time_interval(double seconds) {
    char *str = get_buffer();
    long sec = lrint(round(seconds));
    long min = sec / 60L;
    long hour = min / 60L;
    if (hour > 0L) {
        min -= 60L * hour;
        sprintf(str, COLOR_YELLOW"%ldh %ldm"STYLE_NORMAL, hour, min);
    } else {
        sprintf(str, COLOR_YELLOW"%ldm"STYLE_NORMAL, min);
    }
    return str;
}

void print_timer_started_message(char *task) {
    printf("%s Started working on %s\n", current_time_str(), format_task(task));
}

void print_timer_resumed_message(char *task) {
    printf("%s Resumed working on %s\n", current_time_str(), format_task(task));
}

void print_time_spent_message(double seconds, char *task) {
    printf("%s %s spent working on %s\n", current_time_str(), format_time_interval(seconds), format_task(task));
}

void print_summary(wl_summary_t summary) {
    printf("%s ", current_time_str());
    if (summary.size) {
        printf("Time spent: %s\n", format_time_interval(summary.total_spent));
        if (summary.size > 1) {
            int max_taskname_len = 0;
            for (int t = 0; t < summary.size; ++t) {
                int len = (int) strlen(format_task(summary.task[t]));
                if (len > max_taskname_len) {
                    max_taskname_len = len;
                }
            }
            for (int t = 0; t < summary.size; ++t) {
                printf("%s %*s: %s\n", current_time_str(), max_taskname_len, format_task(summary.task[t]), format_time_interval(summary.spent[t]));
            }
        }
    } else {
        puts("Nothing yet");
    }
}

void print_worklog_cleared_message() {
    printf("%s Worklog cleared\n", current_time_str());
}

void print_logged_time_message(const char *task, double seconds) {
    printf("%s Logged %s to %s", current_time_str(), format_time_interval(seconds), format_task(task));
}

void print_unlogged_time_message(const char *task, double seconds) {
    printf("%s Unlogged %s from %s", current_time_str(), format_time_interval(seconds), format_task(task));
}

void print_error(const char *message, const char *info) {
    if (info) {
        printf(COLOR_RED"%s: %s\n"STYLE_NORMAL, message, info);
    } else {
        printf(COLOR_RED"%s\n"STYLE_NORMAL, message);
    }
}

void print_version() {
    puts("wlog "VERSION STYLE_FAINT", "RELEASE_DATE STYLE_NORMAL);
}

#define DESCRIPTION_OFFSET "20"

void print_commands(command_t *commands) {
    puts(STYLE_BOLD"COMMANDS"STYLE_NORMAL);
    for (command_t *command = commands; cmd_exists(command); ++command) {
        if (command->name) {
            if (command->shortname) {
                printf(STYLE_FAINT"%3s"STYLE_NORMAL" "STYLE_BOLD"%s "STYLE_NORMAL, command->shortname, command->name);
            } else {
                printf("    "STYLE_BOLD"%s "STYLE_NORMAL, command->name);
            }
            if (command->arg_list) {
                fputs(STYLE_UNDERLINED, stdout);
                for (char *c = command->arg_list; *c != '\0'; ++c) {
                    if (*c == ' ') {
                        fputs(STYLE_NORMAL, stdout);
                        putchar(*c);
                        fputs(STYLE_UNDERLINED, stdout);
                    } else {
                        putchar(*c);
                    }
                }
                fputs(STYLE_NORMAL, stdout);
            }
            printf("\n\033[A\033["DESCRIPTION_OFFSET"C - %s\033[0m\n", command->description);
        }
    }
}

void print_help(command_t *commands) {
    puts(STYLE_BOLD"SYNOPSIS\n"
         "  wlog"STYLE_NORMAL"         - Run interactive mode\n"
         "  "STYLE_BOLD"wlog"STYLE_NORMAL" "STYLE_UNDERLINED"command"STYLE_NORMAL" - Run command\n");
    print_commands(commands);
}

void print_imode_help(command_t *commands) {
    print_commands(commands);
    puts("\nTo leave a "STYLE_BOLD"comment"STYLE_NORMAL" use '#' symbol:\n"
         "  > # Comment\n");
}

void print_greeting() {
    puts("Welcome to wlog "VERSION"\nType 'help' or 'h' to see list of available commands");
}