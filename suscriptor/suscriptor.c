#include "suscriptor.h"
#include "Flags.h"
#include "noticia.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


void clear(){
    system("clear");
}

void enviarSuscripcion(Suscriptor suscriptor, char nombre_pipe[]) {

    int fd;
    for(int tries=0;;tries++){
        if(tries==5){
            printf("5 failed attemps to connect pipe, exiting...\n");
            exit(1);
        }

        fd = open(nombre_pipe, O_WRONLY);

        if (fd == -1) {
            perror("Pipe writer");
            sleep(3);
        }
        else{
            break;
        }
    }

    write(fd, &suscriptor, sizeof(Suscriptor));
}

void recibirNoticias() {
    // Se abre el segundo pipe
    int fd;
    char suscriptor_pid[10];

    sprintf(suscriptor_pid, "%d", getpid());
    
    sleep(2);
    for(int tries=0; ; tries++){
        if(tries==5){
            printf("5 failed attemps to connect pipe, exiting...\n");
            exit(1);
        }

        fd = open(suscriptor_pid, O_RDONLY);

        if (fd == -1) {
            perror("Pipe reader");
            sleep(3);
        }
        else{
            break;
        }
    }

    Noticia noticia;
    // Se lee un mensaje por el segundo pipe.
    while (true) {
        size_t bytes = read(fd, &noticia, sizeof(Noticia));
        printf("Se lee la noticia: %c %s \n", noticia.tipo, noticia.info);
    }
}

bool continuar() {

    char temp_lector;

    do {

        printf("\n¿Desea continuar? [S/n] ");
        scanf(" %c", &temp_lector);
        if (temp_lector == 'n' || temp_lector == 'N')
            return false;
        else if (temp_lector == 's' || temp_lector == 'S')
            return true;
        else
            printf("\nNo ha digitado [S/n] para continuar\n");

    } while (temp_lector != 'n' || temp_lector != 'N' || temp_lector != 's' ||
             temp_lector != 'S');
    return true;
}

char *solicitarCategorias() {
    char *categorias = malloc(sizeof(char) * 5);
    char lector = '\0';
    bool error = false;
    int i = 0;

    printf("\nSuscriptor\n");

    while (lector != 'O' || lector != 'o') {
        error = false;

        printf("\nEscriba la letra de una categoria a la que quiere "
               "suscribirse:\n\n A: Noticia de Arte\n E: Noticia de farándula "
               "y Espectáculos\n C: Noticia de Ciencia\n P: Noticia de "
               "Política\n S: Noticia de Sucesos\n O: Confirmar");
        printf("\n\nDigite O para confirmar las categorias digitadas\n");

        if (i <= 4) {
            printf("\nLetra:");
            scanf(" %c", &lector);
        } else
            lector = 'o';

        if (lector == 'A' || lector == 'a')
            categorias[i] = 'A';

        else if (lector == 'E' || lector == 'e')
            categorias[i] = 'E';

        else if (lector == 'C' || lector == 'c')
            categorias[i] = 'C';

        else if (lector == 'P' || lector == 'p')
            categorias[i] = 'P';

        else if (lector == 'S' || lector == 's')
            categorias[i] = 'S';

        else if (lector == 'O' || lector == 'o') {
            if (i == 0) {
                error = true;
                printf("\nNo ha digitado ninguna categoria valida, debe "
                       "digitar minimo una\n");
                if (!continuar())
                    break;
            } else {
                printf("\n¿Desea confirmar las categorias digitadas?\n");

                bool cont = true;

                if (i <= 4)
                    cont = continuar();

                if (cont) {
                    while (i < 5) {
                        categorias[i] = ' ';
                        i++;
                    }
                    printf("\nCategorias confirmadas\n");
                    return categorias;
                } else
                    error = true;
            }
        }

        else {
            error = true;
            printf("\n\nEl caracter digitado no corresponde a ninguna de las "
                   "categorias presentadas anteriormente o la confirmacion de "
                   "las categorias\n");
            if (!continuar())
                break;
        }

        if (error == false)
            i++;
        clear();
    }
    return categorias;
}

void ayuda() {
    printf("El numero de argumentos ingresado es incorrecto\n");
    printf("-s pipe nominal por donde el suscriptor envía información al "
           "sistema central\n");
}

Flags determinarFlags(int len, char *args[]) {
    Flags f;
    char *p;
    for (int i = 1; i < len; i += 2) {
        p = args[i];
        switch (p[1]) {
        case 's':
            f.pipe = args[i + 1];
            break;
        default:
            printf("Flag desconocido: %c\n", p[1]);
            exit(-1);
            break;
        }
    }
    return f;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        ayuda();
        exit(-1);
    }

    Flags flags = determinarFlags(argc, argv);

    Suscriptor s;
    strcpy(s.categorias, solicitarCategorias());
    s.pid = getpid();


    // envia categorias y abre segundo pipe
    enviarSuscripcion(s, flags.pipe);
    recibirNoticias();
    return 0;
}