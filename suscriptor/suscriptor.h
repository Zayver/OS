#include <sys/types.h>
#define MAXCAT 5

typedef struct Suscriptor_ {
  pid_t pid;
  char categorias[MAXCAT];
} Suscriptor;