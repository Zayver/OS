#include "../util/list.h"
#pragma once

typedef struct suscriptor_thread_t{
    char * inputPipe;
    list_t *list;
    list_t * news;

}suscriptor_thread_t;

typedef struct publicator_thread_t{
    char * inputPipe;
    list_t *list;
    unsigned int wait_time;
    list_t* news;
}publicator_thread_t;

static bool volatile run = true; // * Para liberar recursos con signal