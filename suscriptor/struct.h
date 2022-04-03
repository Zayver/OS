#pragma once

#include <sys/types.h>

typedef struct message_t{
    pid_t pid;
    char* topics;
}message_t;