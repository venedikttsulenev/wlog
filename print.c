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

void print_current_time() {
    time_t t = time(NULL);
    const struct tm *tm = localtime(&t);
    char str[6];
    strftime(str, 6, "%H:%M", tm);
    printf(COLOR_GREY"%s "STYLE_NORMAL, str);
}

void print_task_started_message(char *task_tag) {
    print_current_time();
    printf("Started working on "COLOR_GREEN"%s\n"STYLE_NORMAL, task_tag);
}

void print_time_interval(double seconds) {
    long sec = lrint(round(seconds));
    long min = sec / 60L;
    long hour = min / 60L;
    if (hour > 0L) {
        min -= 60L * hour;
        printf(COLOR_YELLOW"%ldh %ldm"STYLE_NORMAL, hour, min);
    } else {
        printf(COLOR_YELLOW"%ldm"STYLE_NORMAL, min);
    }
}

void print_time_spent_message(double seconds, char *task_tag) {
    print_current_time();
    print_time_interval(seconds);
    printf(" spent working on "COLOR_GREEN"%s\n"STYLE_NORMAL, task_tag);
}

void print_summary(wl_summary_t summary) {
    print_current_time();
    if (summary.size) {
        fputs("Time spent: ", stdout);
        print_time_interval(summary.total_spent);
        putchar('\n');
        if (summary.size > 1) {
            int max_tag_len = 0;
            for (int t = 0; t < summary.size; ++t) {
                int len = (int) strlen(summary.tag[t]);
                if (len > max_tag_len) {
                    max_tag_len = len;
                }
            }
            for (int t = 0; t < summary.size; ++t) {
                print_current_time();
                printf(COLOR_GREEN"%*s"STYLE_NORMAL": ", max_tag_len, summary.tag[t]);
                print_time_interval(summary.spent[t]);
                putchar('\n');
            }
        }
    } else {
        puts("Nothing yet");
    }
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

void print_commands(command_t *commands) {
    puts(STYLE_BOLD"COMMANDS"STYLE_NORMAL);
    for (command_t *command = commands; cmd_exists(command); ++command) {
        if (command->name) {
            if (command->shortname) {
                printf(STYLE_FAINT"%3s"STYLE_NORMAL" "STYLE_BOLD"%s "STYLE_NORMAL, command->shortname, command->name);
            } else {
                printf("    "STYLE_BOLD"%s "STYLE_NORMAL, command->name);
            }
            if (command->arg_description) {
                fputs(STYLE_UNDERLINED, stdout);
                for (char *c = command->arg_description; *c != '\0'; ++c) {
                    if (*c == ' ') {
                        fputs(STYLE_NORMAL, stdout);
                        putchar(*c);
                        fputs(STYLE_UNDERLINED, stdout);
                    } else {
                        putchar(*c);
                    }
                }
                fputs(STYLE_NORMAL, stdout);
                printf("%*s - %s \n", 9 - (int) (strlen(command->name) + strlen(command->arg_description)), "", command->description);
            } else {
                printf("%*s - %s \n", 9 - (int) strlen(command->name), "", command->description);
            }
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