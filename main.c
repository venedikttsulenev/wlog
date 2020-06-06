#include <stddef.h>
#include "usage.h"

int main(int argc, char **argv) {
    char *cmd_name = argc > 1 ? argv[1] : NULL;
    command_t command = get_command(cmd_name);
    result_t result = execute_command(command);
    return handle(result);
}
