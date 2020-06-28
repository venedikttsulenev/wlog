#ifndef WLOG_WORKLOG_H
#define WLOG_WORKLOG_H

#include <time.h>

#define WL_MAX_TAG_LENGTH 32

typedef char wl_tag_t[WL_MAX_TAG_LENGTH];

typedef struct wl_summary {
    int size;
    double total_spent;
    double *spent;
    wl_tag_t *tag;
} wl_summary_t;

void wl_init();

double wl_log_time_spent(time_t *since, char *tag);
wl_summary_t wl_get_summary();

void wl_free();

#endif //WLOG_WORKLOG_H
