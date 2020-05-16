#include "task.h"
#include <string.h>

void init_task_list(task_list_t *task_list) {
    task_list->len = 0;
}

task_t find_or_create_task(char *tag_str, task_list_t *tasks) {
    for (int i = 0; i < tasks->len; ++i) {
        if (0 == strcmp(tag_str, tasks->tag[i]))
            return i;
    }
    strncpy(tasks->tag[tasks->len], tag_str, MAX_TAG_LENGTH);
    return tasks->len++;
}