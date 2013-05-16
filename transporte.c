//
//  transporte.c
//
//  Guilherme Sividal - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 07/04/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/globals.h"

void *iniciarTransporte() {

    int tes, trs, tea, tra;
    pthread_t threadEnviarSegmentos, threadReceberSegmentos, threadEnviarPacote, threadReceberPacote;

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
    tea = pthread_create(&threadEnviarPacote, NULL, enviarPacote, NULL);

    if (tea) {
        printf("ERRO: impossivel criar a thread : enviarPacote\n");
        exit(-1);
    }

    //Inicia a thread enviarDatagramas
    tra = pthread_create(&threadReceberPacote, NULL, receberPacote, NULL);

    if (tra) {
        printf("ERRO: impossivel criar a thread : receberPacote\n");
        exit(-1);
    }

    //Espera as threads terminarem
    pthread_join(threadEnviarSegmentos, NULL);
    pthread_join(threadReceberSegmentos, NULL);
    pthread_join(threadEnviarPacote, NULL);
    pthread_join(threadReceberPacote, NULL);

}

void *enviarPacote() {

    while (TRUE) {

        struct pacote pacote_env;

    }
}

void *receberPacote() {

    while (TRUE) {

        struct pacote pacote_rcv;

    }

}

void *enviarSegmentos() {

    while (TRUE) {

        struct segmento segmento_env;

    }
}

void *receberSegmentos() {

    while (TRUE) {

        struct segmento segmento_rcv;

    }

}

void colocarPacoteBufferApliTransRcv(struct pacote pacote){

    //Colocar no Buffer
    buffer_trans_rede_env.tam_buffer = pacote.tam_buffer;
    memcpy(&buffer_trans_rede_env.data, &pacote, sizeof (pacote));

}

void retirarPacoteBufferApliTransEnv(struct pacote *pacote) {

    //Retirar do Buffer
    pacote->tam_buffer = buffer_trans_rede_rcv.data.tam_buffer;
    strcpy(pacote->buffer, buffer_trans_rede_rcv.data.buffer);

}

void colocarSegmentoBufferTransRedeEnv(struct segmento segment){

    //Colocar no Buffer
    buffer_trans_rede_env.tam_buffer = segment.tam_buffer;
    memcpy(&buffer_trans_rede_env.data, &segment, sizeof (segment));

}

void retirarSegmentoBufferTransRedeRcv(struct segmento *segment) {

    //Retirar do Buffer
    segment->tam_buffer = buffer_trans_rede_rcv.data.tam_buffer;
    strcpy(segment->buffer, buffer_trans_rede_rcv.data.buffer);

}