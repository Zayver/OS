#pragma once

#include <sys/types.h>

typedef struct suscriptor_message_t{
    pid_t pid;
    char* topics;
}message_t;