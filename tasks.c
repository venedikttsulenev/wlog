#include "tasks.h"
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 32
#define CAPACITY_GROW_STEP 32
#define MAX_TAG_LENGTH 32

typedef char tag_t[MAX_TAG_LENGTH];

static int ts_init_ran = 0;

static int ts_capacity = DEFAULT_CAPACITY;
static int ts_count = 0;
static tag_t *ts_tag = NULL;

void ts_init() {
    ts_tag = malloc(ts_capacity * sizeof(tag_t));
}

void grow() {
    ts_capacity += CAPACITY_GROW_STEP;
    ts_tag = realloc(ts_tag, ts_capacity * sizeof(tag_t));
}

task_id_t ts_find_or_create(char *tag) {
    if (!ts_init_ran) {
        ts_init();
    }
    for (int i = 0; i < ts_count; ++i) {
        if (0 == strcmp(tag, ts_tag[i]))
            return i;
    }
    if (ts_count == ts_capacity) {
        grow();
    }
    strncpy(ts_tag[ts_count], tag, MAX_TAG_LENGTH);
    return ts_count++;
}

char *ts_get_tag(task_id_t task_id) {
    if (task_id >= 0 && task_id < ts_count)
        return ts_tag[task_id];
    return NULL;
}

void ts_free() {
    if (ts_tag) {
        free(ts_tag);
    }
}