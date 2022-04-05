#include "../util/list.h"

typedef struct suscriptor_thread_t{
    char * inputPipe;
    list_t list;

}suscriptor_thread_t;

typedef struct publicator_thread_t{
    char * inputPipe;
    list_t list;
}publicator_thread_t;

static bool volatile run = true; // * Para liberar recursos con signal
static bool volatile run_publicator = true;
static bool volatile run_suscriptor = true;