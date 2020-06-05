#ifndef WLOG_INTERACTIVE_MODE_H
#define WLOG_INTERACTIVE_MODE_H

#include "task.h"
#include "worklog.h"
#include "time.h"
#include "error.h"
#include "command.h"

result_t run_interactive_mode();

command_t *get_imode_commands();
command_t get_imode_command(char *name);

#endif //WLOG_INTERACTIVE_MODE_H
