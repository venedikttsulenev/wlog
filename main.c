#include <stddef.h>
#include "command.h"

int main(int argc, char **argv) {
    command_t command = command_for_str(argc > 1 ? argv[1] : NULL);
    result_t result = command.execute(argc, argv);
    return handle(result);
}
