#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Datos.h"
#include "Flags.h"
#include "StructPid.h"

/*Enviar Pipe
abre el pipe y envia los datos
*/
void EnviarPipe(datos *midat, int cuantos,char pipe[],ElPid pidEnviar) {

  int i, creado, fd;
  do{
  fd=open(pipe, O_WRONLY);
  if(fd==-1){
  perror("pipe");
  printf("Se volvera a intentar\n");
  }else creado=1;
  }while(creado==0);

  write(fd,&pidEnviar,sizeof(ElPid));
 for(i=0;i<cuantos;i++){
 write(fd,&midat[i],sizeof(datos));
 }
}

/* asignar
   inserta una nueva noticia en la lista en la posicion pos
   e inicializa el tipo de noticia, el contenido y el tamaño del mismo
*/
void asignar(datos *dat,char prueba,char *notifica, int pos)
{ 
  dat[pos].len=(strlen(notifica));
  dat[pos].tipo=prueba;
  strcpy(dat[pos].notifica, notifica);
 
}
/* DeterminarFlags
   Valida que tipo de argumento se tiene segun los flags correspondientes
   en este caso se ignora el nombre
*/
Flags determinarFlags(int len,char* args[])
{
  Flags f; 
  char *p;
  int value;
  for(int i=1; i<len;i+=2){
    p=args[i];
    switch(p[1]){
      case 'f':
        f.NombreArchivo=args[i+1];
        break;
      case'p':
        f.pipe=args[i+1];
        break;
      case't':
        f.timeN=atoi(args[i+1]);
        break;
      default:
        printf("Flag desconocido: %c\n",p[1]);
        exit(-1);
        break;
    }
  }
  return f;
}

/* ayuda
   cuando no se ingresan la cantidad de argumentos que se eseran
   explica el tipo de flags que se pueden utlizar y sus funciones
*/
void ayuda(){
  printf("El numero de argumentos ingresado es incorrecto\n");
  printf("-p pipe nominal por donde los publicadores envían información\n");
  printf("-f nombre de un archivo donde están diferentes noticias\n");
  printf("-t tiempo en segundos entre el envío de una noticia y el envío de la siguiente\n");
}

int main(int argc, char *argv[]) 
{
  //Se espera que se ingerese 7 arguemntos, en caso contrario se muestra la funcion ayuda
  if(argc !=7){
    ayuda();
    exit(-1);
  }
  //Se guardan los argumentos enviados en en una variable tipo Flags
  Flags flags=determinarFlags(argc,argv);
  //Se guardara el pid en una variable de tipo Elpid
  ElPid mipid;
  //Se guarda el PID
  mipid.pid = fork ();
  if (!mipid.pid) {
    execlp ("sleep", "sleep", "5",   NULL);
  }

  //Se definen las variables que guardaran la informacion de los publicadores
  datos midat[MAXDAT];
  char not[MAXINF];
  char tipo;
  FILE *fp;
  int i;
  
  fp = fopen(flags.NombreArchivo, "r");
  i=0;
  while (!feof(fp))  {
    //Se lee el tipo de noticia
   fscanf(fp, "%c\n",&tipo);
    //Se lee la noticia
   fgets(not,60,fp);

    if(tipo !='#')
     {
    //Entre cada noticia se espera el tiempo solicitado
    sleep (flags.timeN);
    asignar(midat,tipo,not,i++);
    EnviarPipe(midat,i,flags.pipe,mipid);
    i=0;
      }
  }
  fclose(fp);
}
