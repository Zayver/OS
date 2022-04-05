
#include <sys/types.h>
#pragma once
#define MAXDAT 1000
#define MAXINF 81 //Segun enunciado
typedef struct datos_p {
    char notifica[MAXINF];
    char tipo;
    int len;
    pid_t pid;

} datos_p;
