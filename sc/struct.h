#include "../util/list.h"

typedef struct suscriptor_thread_t{
    char * inputPipe;
    list_t list;

}suscriptor_thread_t;

typedef struct publicator_thread_t{
    char * inputPipe;
    list_t list;
}publicator_thread_t;