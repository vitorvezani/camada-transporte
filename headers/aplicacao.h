//
//  aplicacao.h
//
//  Guilherme Sividal - 09054512 
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>		    /* for printf() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */

// Defines
#define TRUE 	1
#define FALSE	0

#define TAM_MAX_BUFFER 1400

//DEFINIR pacote

/* Estrutura do pacote */
struct pacote {
    //DEFINIR
    int tam_buffer;
    char buffer[TAM_MAX_BUFFER];
};

/* Estrutura do buffer entre a camada de aplicacao e transporte*/
struct buffer_apli_trans {
    //DEFINIR
    int tam_buffer;
    int env_no;
    int retorno;
    struct pacote data;
};

/* Estrutura do Segmento */
struct segmento {
    //DEFINIR
    int tam_buffer;
    struct pacote data;
};

/* Estrutura do arquivo */
struct file {
    char file_name[20];
    int num_no;
};

// Variaveis Globais à Camada de Rede e Main

extern struct buffer_apli_trans buffer_apli_trans_env, buffer_apli_trans_rcv;
extern struct file file_info;

extern pthread_mutex_t mutex_apli_trans_env1, mutex_apli_trans_env2;
extern pthread_mutex_t mutex_apli_trans_rcv1, mutex_apli_trans_rcv2;

// Threads
void *enviarPacotes();
void *receberPacotes();

//Funcoes

void colocarPacotesBufferApliTransEnv(struct pacote pacote);
void retirarPacotesBufferApliTransRcv(struct pacote *pacote);