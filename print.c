#include <stdio.h>
#include <math.h>
#include <string.h>
#include "worklog.h"
#include "task.h"

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

void print_summary(worklog_t *worklog, task_list_t *task_list) {
    double spent[task_list->len];
    memset(spent, 0, sizeof(spent));
    double total_spent = 0;
    for (int i = 0; i < worklog->len; ++i) {
        spent[worklog->entry[i].task] += worklog->entry[i].time_spent;
        total_spent += worklog->entry[i].time_spent;
    }
    puts("Time spent:");
    for (int i = 0; i < task_list->len; ++i) {
        printf("%s: ", task_list->tag[i]);
        print_time_interval(spent[i]);
    }
    printf("---\nTotal: ");
    print_time_interval(total_spent);
}