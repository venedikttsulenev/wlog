#ifndef WLOG_TASK_H
#define WLOG_TASK_H

#define NO_TASK (-1)
#define BREAK_TASK (-2)

typedef int task_t;

#define is_task(task) task >= 0
#define no_task(task) task < 0

#define MAX_TAGS 256
#define MAX_TAG_LENGTH 32

typedef char tag_t[MAX_TAG_LENGTH];

typedef struct task_list {
    tag_t tag[MAX_TAGS];
    int len;
} task_list_t;

void init_task_list(task_list_t *task_list);

task_t find_or_create_task(char *tag_str, task_list_t *tasks);

#endif //WLOG_TASK_H
