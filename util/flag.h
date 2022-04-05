#include <stdio.h>
#include <stdlib.h>

typedef struct Flags{
    char* publicator_pipe; 
    char* suscriptor_pipe;
    unsigned int time;
}Flags;


Flags determineFlags(int len ,char* args[]){
    Flags f; char *p;int value;
    for (int i=1; i<len;i+=2){ //1 porque se ignora el nombre
        p=args[i];
        // apuntando p al '-' el siguiente byte sería el flag entonces:
        switch (p[1]){
            case 'p':
                f.publicator_pipe=args[i+1];
                break;
            case 's':
                f.suscriptor_pipe=args[i+1];
                break;
            case 't':
                f.time=atoi(args[i+1]);
                break;
            default:
                printf("Flag desconocido: '%c' \n", p[1]);
                exit(22);
                break;
        }
    }
    return f;

}


void help(){
    printf("Error en los argumentos, uso ./sc [opción]... [argumento]...\n");
    printf("-s      Pipe nominal para la comunicación de suscriptores\n");
    printf("-p      Pipe nominal para la comunicación de publicador\n");
    printf("-t      Tiempo de espera entre refresco\n");
}