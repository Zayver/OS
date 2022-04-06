#include <sys/types.h>
#pragma once
#define MAXDAT 1000
#define MAXINF 81 
typedef struct noticia {
    char info[MAXINF];
    char tipo;
    int len;
} Noticia;