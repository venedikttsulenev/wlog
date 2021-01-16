#include <stdarg.h>
#include <stdio.h>

#define BUFFER_SIZE 16

static char buffer[BUFFER_SIZE][256]; // BUFFER_SIZE strings of length 256
static int free_buffer_index = 0;

char *new_str() {
    char *ret = buffer[free_buffer_index++];
    if (free_buffer_index >= BUFFER_SIZE) {
        free_buffer_index = 0;
    }
    return ret;
}

char *format_str(const char *format, ...) {
    char *str = new_str();
    va_list ap;
    va_start(ap, format);
    vsprintf(str, format, ap);
    va_end(ap);
    return str;
}