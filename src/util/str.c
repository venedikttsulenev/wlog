#include <stdarg.h>
#include <stdio.h>

#define BUFFER_SIZE 1024 // 1Kb

static char buffer[BUFFER_SIZE];
static int free_index = 0;

char *new_str(size_t size) {
    char *ret = buffer + free_index;
    if (ret + size >= buffer + BUFFER_SIZE) {
        ret = buffer;
        free_index = 0;
    }
    free_index += size;
    return ret;
}

char *new_s() {
    return new_str(256);
}

char *format_str(const char *format, ...) {
    char *str = new_s();
    va_list ap;
    va_start(ap, format);
    vsprintf(str, format, ap);
    va_end(ap);
    return str;
}