all: directory publicator suscriptor sc clear

DIRECTORY=output
COMPILER=gcc


directory:
	mkdir -p $(DIRECTORY)
	cp publicator/Noticias* $(DIRECTORY)/

publicator.o: publicator/publicator.c
	gcc -c publicator/publicator.c -o publicator.o

publicator: publicator.o
	gcc publicator.o -o $(DIRECTORY)/publicator

suscriptor.o: suscriptor/suscriptor.c
	gcc -c suscriptor/suscriptor.c -o suscriptor.o

suscriptor: suscriptor.o
	gcc suscriptor.o -o $(DIRECTORY)/suscriptor

sc.o: sc/sc.c
	gcc -c sc/sc.c -o sc.o -pthread
sc: sc.o
	gcc sc.o -o $(DIRECTORY)/sc -pthread

clear:
	rm *.o

remove:
	rm -r output
