#include <stddef.h>
#include "command/singlerun.h"

int main(int argc, char **argv) {
    char *cmd_name = argc > 1 ? argv[1] : NULL;
    command_t command = singlerun_command(cmd_name);
    cmd_execute(command);
    return err_handle();
}
