/**
 * @file sc.c
 * @author Santiago Z 
 * @brief Sistema de comunicación central 
 */
#include <stdio.h>
#include "../util/flag.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include "suscriptor.h"
#include "noticia.h" 
#include <unistd.h>

int main (int argc, char **argv)
{
  int fd, pid, n, bytes=-1;
  Suscriptor s,m;
  
  mode_t fifo_mode = S_IRUSR | S_IWUSR;
  
  unlink(argv[1]);
  if (mkfifo (argv[1], fifo_mode) == -1) {
     perror("mkfifo");
     exit(1);
  }

  
  fd = open (argv[1], O_RDONLY);
  printf ("Abrio el pipe\n");

  bytes=read(fd,&s,sizeof(Suscriptor));
  
  for(int j=0; j<5; j++)
      printf("%c",s.categorias[j]);
  
  
  int i, creado, fd2;
  char suscriptor_pid[10];

   sprintf(suscriptor_pid,"%d",(int)s.pid);
   //printf("%s",suscriptor_pid);

   do { 
      if ((fd2 = open(suscriptor_pid, O_WRONLY)) == -1) {
         perror(" Server Abriendo el segundo pipe ");
         printf(" Se volvera a intentar despues\n");
      } else creado = 1; 
   }  while (creado == 0);


   //enviar noticias de prueba
   Noticia* noticias = malloc(sizeof(Noticia)*5);
   for(int i=0; i<5; i++){
      sprintf(noticias[i].info,"%s","hola");
      noticias[i].len =4;
   }

    for(int i=0; i<5; i++){
       write(fd2,&noticias[i],sizeof(Noticia));
    }

    // Aqui va el codigo de lectura de la informacion por el pipe. 
    
    
}