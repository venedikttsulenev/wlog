#ifndef WLOG_TASKS_H
#define WLOG_TASKS_H

#define NO_TASK (-1)
#define BREAK_TASK (-2)

#define is_task(task) task >= 0
#define no_task(task) task < 0

typedef int task_id_t;

void init_tasks();

task_id_t find_or_create_task(char *tag);
char *get_task_tag(task_id_t task_id);

void free_tasks();

#endif //WLOG_TASKS_H
