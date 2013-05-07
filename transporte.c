//
//  transporte.c
//
//  Guilherme Sividal - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 07/04/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/transporte.h"

void *iniciarTransporte() {

    int tes, trs, tea, tra;
    pthread_t threadEnviarSegmentos, threadReceberSegmentos, threadEnviarPacotes, threadReceberPacotes;

    //Inicia a thread enviarDatagramas
    tes = pthread_create(&threadEnviarSegmentos, NULL, enviarSegmentos, NULL);

    if (tes) {
        printf("ERRO: impossivel criar a thread : enviarSegmentos\n");
        exit(-1);
    }

    //Inicia a thread enviarDatagramas
    trs = pthread_create(&threadReceberSegmentos, NULL, receberSegmentos, NULL);

    if (trs) {
        printf("ERRO: impossivel criar a thread : receberSegmentos\n");
        exit(-1);
    }

    //Inicia a thread enviarDatagramas
    tea = pthread_create(&threadEnviarPacotes, NULL, enviarPacotes, NULL);

    if (tea) {
        printf("ERRO: impossivel criar a thread : enviarPacotes\n");
        exit(-1);
    }

    //Inicia a thread enviarDatagramas
    tra = pthread_create(&threadReceberPacotes, NULL, receberPacotes, NULL);

    if (tra) {
        printf("ERRO: impossivel criar a thread : receberPacotes\n");
        exit(-1);
    }

    //Espera as threads terminarem
    pthread_join(threadEnviarSegmentos, NULL);
    pthread_join(threadReceberSegmentos, NULL);
    pthread_join(threadEnviarPacotes, NULL);
    pthread_join(threadReceberPacotes, NULL);

}

void *enviarPacotes() {

    while (1) {

        struct pacote pacote_env;

    }
}

void *receberPacotes() {

    while (TRUE) {

        struct pacote pacote_rcv;

    }

}

void *enviarSegmentos() {

    while (1) {

        struct segmento segmento_env;

    }
}

void *receberSegmentos() {

    while (TRUE) {

        struct segmento segmento_rcv;

    }

}

void colocarPacotesBufferApliTransEnv(struct pacote pacote){

    //Colocar no Buffer
    buffer_trans_rede_env.tam_buffer = pacote.tam_buffer;
    memcpy(&buffer_trans_rede_env.data, &pacote, sizeof (pacote));

}

void retirarPacotesBufferApliTransRcv(struct pacote *pacote) {

    pacote->tam_buffer = buffer_trans_rede_rcv.data.tam_buffer;
    strcpy(pacote->buffer, buffer_trans_rede_rcv.data.buffer);

}

void colocarSegmentoBufferTransRedeEnv(struct segmento segment){

    //Colocar no Buffer
    buffer_trans_rede_env.tam_buffer = segment.tam_buffer;
    memcpy(&buffer_trans_rede_env.data, &segment, sizeof (segment));

}

void retirarSegmentoBufferTransRedeRcv(struct segmento *segment) {

    segment->tam_buffer = buffer_trans_rede_rcv.data.tam_buffer;
    strcpy(segment->buffer, buffer_trans_rede_rcv.data.buffer);

}