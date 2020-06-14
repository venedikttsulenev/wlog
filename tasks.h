#ifndef WLOG_TASKS_H
#define WLOG_TASKS_H

#define NO_TASK (-1)
#define BREAK_TASK (-2)

#define is_task(task) task >= 0
#define no_task(task) task < 0

typedef int task_id_t;

task_id_t ts_find_or_create(char *tag);
char *ts_get_tag(task_id_t task_id);

void ts_free();

#endif //WLOG_TASKS_H
