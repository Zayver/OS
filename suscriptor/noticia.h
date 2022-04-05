#include <sys/types.h>

#define MAXDAT 1000
#define MAXINF 81 
typedef struct datos_p {
    char info[MAXINF];
    char tipo;
    int len;
} Noticia;