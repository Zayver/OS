/**
 * @file flag.h
 * @author Santiago Z
 * @brief Cabecera para poder identificar flags
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Flags{
    char* publicator_pipe;  //nombre del pipe del publicador
    char* suscriptor_pipe; //nombre del pipe del suscriptor
    unsigned int time; //tiempo a esperar 
}Flags;

/**
 * @brief Determina los flags pasados como arreglo
 * @param len  longitud de argumentos
 * @param args arreglo de argumentos
 * @return Flags separados en su estructura
 */
Flags determineFlags(int len ,char* args[]){
    Flags f; char *p;
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

/**
 * @brief Imprime ayuda del SC
 * 
 */
void help(){
    printf("Error en los argumentos, uso ./sc [opción]... [argumento]...\n");
    printf("-s      Pipe nominal para la comunicación de suscriptores\n");
    printf("-p      Pipe nominal para la comunicación de publicador\n");
    printf("-t      Tiempo de espera entre refresco\n");
}