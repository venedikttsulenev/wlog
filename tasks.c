#include "tasks.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 32
#define CAPACITY_GROW_STEP 32
#define MAX_TAG_LENGTH 32

typedef char tag_t[MAX_TAG_LENGTH];

static int t_capacity = DEFAULT_CAPACITY;
static int t_count = 0;
static tag_t *t_tag = NULL;

void init_tasks() {
    t_tag = malloc(t_capacity * sizeof(tag_t));
}

void grow() {
    t_capacity += CAPACITY_GROW_STEP;
    t_tag = realloc(t_tag, t_capacity * sizeof(tag_t));
}

task_id_t find_or_create_task(char *tag) {
    for (int i = 0; i < t_count; ++i) {
        if (0 == strcmp(tag, t_tag[i]))
            return i;
    }
    if (t_count == t_capacity) {
        grow();
    }
    strncpy(t_tag[t_count], tag, MAX_TAG_LENGTH);
    return t_count++;
}

char *get_task_tag(task_id_t task_id) {
    if (task_id >= 0 && task_id < t_count)
        return t_tag[task_id];
    return NULL;
}

void free_tasks() {
    free(t_tag);
}