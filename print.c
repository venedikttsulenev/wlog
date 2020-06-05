#include <stdio.h>
#include <math.h>
#include <string.h>
#include "worklog.h"
#include "task.h"
#include "usage.h"
#include "interactive_mode.h"
#include "version.h"

#define CURRENT_TIME_OFFSET "10"

void print_current_time() {
    time_t t = time(NULL);
    const struct tm *tm = localtime(&t);
    char str[6];
    strftime(str, 5, "%H:%M", tm);
    printf("\033[A\033["CURRENT_TIME_OFFSET"C\033[2m - %s\033[0m\n", str);
}

void print_task_started_message(char *task_tag) {
    printf("Started working on %s\n", task_tag);
}

void print_time_interval(double seconds) {
    long sec = lrint(round(seconds));
    long min = sec / 60L;
    long hour = min / 60L;
    if (hour > 0L) {
        min -= 60L * hour;
        printf("%ldh %ldm\n", hour, min);
    } else {
        printf("%ldm\n", min);
    }
}

void print_time_spent_message(double seconds, char *tag_str) {
    printf("Time spent working on %s: ", tag_str);
    print_time_interval(seconds);
}

void print_summary(worklog_t *worklog, task_list_t *task_list, task_t current_task, double spent_on_current_task) {
    if (!task_list->len)
        return;
    double spent[task_list->len];
    memset(spent, 0, sizeof(spent));
    double total_spent = 0;
    if (is_task(current_task)) {
        total_spent = spent[current_task] = spent_on_current_task;
    }
    for (int i = 0; i < worklog->len; ++i) {
        spent[worklog->entry[i].task] += worklog->entry[i].time_spent;
        total_spent += worklog->entry[i].time_spent;
    }
    puts("Time spent:");
    for (int i = 0; i < task_list->len; ++i) {
        printf("%s: ", task_list->tag[i]);
        print_time_interval(spent[i]);
    }
    fputs("--------------\nTotal: ", stdout);
    print_time_interval(total_spent);
}

void print_interactive_mode_help() {
    command_t *imode_commands = get_imode_commands();

    puts("\033[1mINTERACTIVE MODE\033[0m\n");
    for (command_t *command = imode_commands; command_exists(command); ++command) {
        if (command->arg_description) {
            printf("  \033[1m%-8s\033[0m \033[4m%-4s\033[0m - %s\n", command->name, command->arg_description, command->description);
        } else {
            printf("  \033[1m%-13s\033[0m - %s\n", command->name, command->description);
        }
    }
}

void print_help() {
    command_t *commands = get_supported_commands();

    puts("\033[1mSYNOPSIS\n"
         "  wlog\033[0m [\033[4mcommand\033[0m]\n\n"
         "\033[1mCOMMANDS\033[0m\n");
    for (command_t *command = commands; command_exists(command); ++command) {
        char *name = command->name ? command->name : "";
        if (command->shortname) {
            printf("  \033[1mwlog %-7s (%1s)\033[0m - %s\n", name, command->shortname, command->description);
        } else {
            printf("  \033[1mwlog %-11s\033[0m - %s\n", name, command->description);
        }
    }
    putchar('\n');
    print_interactive_mode_help();
}

void print_version() {
    puts("wlog "VERSION"\033[2m, "RELEASE_DATE"\033[0m");
}