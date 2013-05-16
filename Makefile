#
#	makefile.GNUmakefile
#
#  Guilherme Sividal - 09054512 
#  Vitor Rodrigo Vezani - 10159861
#
#  Created by Vitor Vezani on 19/03/13.
#  Copyright (c) 2013 Vitor Vezani. All rights reserved.
#

all: meuprog.o enlace.o rede.o arquivo.o transporte.o garbler.o inicializador.o
	gcc -o meuprog.exe meuprog.o enlace.o rede.o arquivo.o transporte.o garbler.o inicializador.o

meuprog: meuprog.c
	gcc -c meuprog.c

enlace: enlace.c
	gcc -c enlace.c

rede: rede.c
	gcc -c rede.c

arquivo: arquivo.c
	gcc -c arquivo.c

transporte: transporte.c
	gcc -c transporte.c
	
garbler: garbler.c
	gcc -c garbler.c

inicializador: inicializador.c
	gcc -c inicializador.c

clean:
	rm -f *.o *.exe