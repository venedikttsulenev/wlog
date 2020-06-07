#include <stdio.h>
#include <math.h>
#include <string.h>
#include "worklog.h"
#include "tasks.h"
#include "usage.h"
#include "version.h"

#define NORMAL "\033[0m"
#define BOLD "\033[1m"
#define FAINT "\033[2m"
#define UNDERLINED "\033[4m"

#define RED "\033[38;5;52m"
#define GREY "\033[38;5;241m"
#define MAGENTA "\033[38;5;94m"
#define GREEN "\033[38;5;22m"

void print_current_time() {
    time_t t = time(NULL);
    const struct tm *tm = localtime(&t);
    char str[6];
    strftime(str, 6, "%H:%M", tm);
    printf(GREY"%s "NORMAL, str);
}

void print_task_started_message(char *task_tag) {
    print_current_time();
    printf("Started working on "GREEN"%s\n"NORMAL, task_tag);
}

void print_time_interval(double seconds) {
    long sec = lrint(round(seconds));
    long min = sec / 60L;
    long hour = min / 60L;
    if (hour > 0L) {
        min -= 60L * hour;
        printf(MAGENTA"%ldh %ldm"NORMAL, hour, min);
    } else {
        printf(MAGENTA"%ldm"NORMAL, min);
    }
}

void print_time_spent_message(double seconds, char *task_tag) {
    print_current_time();
    print_time_interval(seconds);
    printf(" spent working on "GREEN"%s\n"NORMAL, task_tag);
}

void print_summary(worklog_summary_t summary) {
    print_current_time();
    if (summary.size) {
        fputs("Time spent: ", stdout);
        print_time_interval(summary.total_spent);
        putchar('\n');
        if (summary.size > 1) {
            int max_tag_len = 0;
            for (task_id_t t = 0; t < summary.size; ++t) {
                int len = (int) strlen(get_task_tag(t));
                if (len > max_tag_len) {
                    max_tag_len = len;
                }
            }
            for (task_id_t t = 0; t < summary.size; ++t) {
                print_current_time();
                printf(GREEN"%*s"NORMAL": ", max_tag_len, get_task_tag(t));
                print_time_interval(summary.spent[t]);
                putchar('\n');
            }
        }
    } else {
        puts("Nothing yet");
    }
}

void set_print_mode(char c) {
    putchar('\033');
    putchar('[');
    putchar(c);
    putchar('m');
}

void set_underlined() {
    set_print_mode('4');
}

void set_normal() {
    set_print_mode('0');
}

void print_error(const char *message, const char *info) {
    if (info) {
        printf(RED"%s: %s\n"NORMAL, message, info);
    } else {
        printf(RED"%s\n"NORMAL, message);
    }
}

void print_version() {
    puts("wlog "VERSION FAINT", "RELEASE_DATE NORMAL);
}

void print_commands(command_t *commands) {
    for (command_t *command = commands; command_exists(command); ++command) {
        if (command->name) {
            if (command->shortname) {
                printf(FAINT"%3s"NORMAL" "BOLD"%s "NORMAL, command->shortname, command->name);
            } else {
                printf("    "BOLD"%s "NORMAL, command->name);
            }
            if (command->arg_description) {
                set_underlined();
                for (char *c = command->arg_description; *c != '\0'; ++c) {
                    if (*c == ' ') {
                        set_normal();
                        putchar(*c);
                        set_underlined();
                    } else {
                        putchar(*c);
                    }
                }
                set_normal();
                printf("%*s - %s \n", 9 - (int) (strlen(command->name) + strlen(command->arg_description)), "", command->description);
            } else {
                printf("%*s - %s \n", 9 - (int) strlen(command->name), "", command->description);
            }
        }
    }
}

void print_help(command_t *commands) {
    puts(BOLD"SYNOPSIS\n"
         "  wlog"NORMAL"         - Run interactive mode\n"
         "  "BOLD"wlog"NORMAL" "UNDERLINED"command"NORMAL" - Run command\n\n"
         BOLD"COMMANDS"NORMAL);
    print_commands(commands);
}

void print_imode_help(command_t *commands) {
    puts(BOLD"INTERACTIVE MODE\n"NORMAL);
    print_commands(commands);
    puts("\nTo leave a "BOLD"comment"NORMAL" use '#' symbol:\n"
         "  > # Comment\n");
}

void print_greeting() {
    puts("Welcome to wlog "VERSION"!\nType 'help' or 'h' to see list of available commands");
}