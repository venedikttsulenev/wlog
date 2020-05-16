#ifndef WLOG_COMMAND_H
#define WLOG_COMMAND_H

typedef struct {
    int code;
    const char *info;
} result_t;

typedef struct {
    result_t (*execute)(int argc, char **argv);
    char *name;
    char *shortname;
} command_t;

command_t command_for_str(char *str);
int handle(result_t result);

#endif //WLOG_COMMAND_H
