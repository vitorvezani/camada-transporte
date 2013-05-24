//
//  transporte.c
//
//  Guilherme Sividal    - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 07/04/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/globals.h"

void *iniciarTransporte() {

    int tes, trs, tea, tra, tt;
    pthread_t threadEnviarSegmentos, threadReceberSegmentos, threadEnviarPacote, threadReceberPacote, threadTimer;

    //Inicia a thread enviarSegmentos
    tes = pthread_create(&threadEnviarSegmentos, NULL, enviarSegmentos, NULL);

    if (tes) {
        printf("ERRO: impossivel criar a thread : enviarSegmentos\n");
        exit(-1);
    }

    //Inicia a thread enviarSegmentos
    trs = pthread_create(&threadReceberSegmentos, NULL, receberSegmentos, NULL);

    if (trs) {
        printf("ERRO: impossivel criar a thread : receberSegmentos\n");
        exit(-1);
    }

    //Inicia a thread enviarPacote
    tea = pthread_create(&threadEnviarPacote, NULL, enviarPacote, NULL);

    if (tea) {
        printf("ERRO: impossivel criar a thread : enviarPacote\n");
        exit(-1);
    }

    //Inicia a thread enviarPacote
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
    pthread_join(threadTimer, NULL);

}

void *enviarPacote() {

    while (TRUE) {

        struct pacote pacote_rcv;

        /* Consumir buffer_trans_trans_rcv */
        pthread_mutex_lock(&mutex_trans_trans_rcv2);

        retirarPacoteBufferTransTransRcv(&pacote_rcv);

        /* Consumir buffer_trans_trans_rcv */
        pthread_mutex_unlock(&mutex_trans_trans_rcv1);

        //TODO

        /* Produzir buffer_apli_trans_rcv */
        pthread_mutex_lock(&mutex_apli_trans_rcv1);

        colocarPacoteBufferApliTransRcv(pacote_rcv);

        /* Produzir buffer_apli_trans_rcv */
        pthread_mutex_unlock(&mutex_apli_trans_rcv2);

    }
}

void *receberPacote() {

    while (TRUE) {

        struct pacote pacote_env;

        /* Consumir buffer_apli_trans_env */
        pthread_mutex_lock(&mutex_apli_trans_env2);

        retirarPacoteBufferApliTransEnv(&pacote_env);

        /* Consumir buffer_apli_trans_env */
        pthread_mutex_unlock(&mutex_apli_trans_env1);

        //TODO

        /* Produzir buffer_trans_trans_env */
        pthread_mutex_lock(&mutex_trans_trans_env1);

        colocarPacoteBufferTransTransEnv(pacote_env);

        /* Produzir buffer_trans_trans_env */
        pthread_mutex_unlock(&mutex_trans_trans_env2);

    }

}

void *timer() {

    usleep(100000);

    nextseqnum = base;

    pthread_mutex_unlock(env_seg_rcv_seg_timer_2);
}

void *enviarSegmentos() {

    int base = 0;
    int nextseqnum = 0;
    int first_pkg;

    while (TRUE) {
        
        /* Consumir buffer_trans_trans_env */
        pthread_mutex_lock(&mutex_trans_trans_env2);

        while(nextseqnum < base + TAM_JANELA){

            struct segmento segmento_env;

            first_pkg = 1;

            retirarSegmentoBufferTransTransEnv(&segmento_env);

            /* Consumir buffer_trans_trans_env */
            pthread_mutex_unlock(&mutex_trans_trans_env1);

            // Envia os segmentos da janela

            //Envia Segmento

            /* Produzir buffer_trans_rede_env */
            pthread_mutex_lock(&mutex_trans_rede_env1);

            colocarSegmentoBufferTransRedeEnv(segmento_env);

            /* Produzir buffer_trans_rede_env */
            pthread_mutex_unlock(&mutex_trans_rede_env2);

            if (first_pkg)
            {
                //Inicia a thread enviarPacote
                tt = pthread_create(&threadTimer, NULL, timer, NULL);

                //Start Time
                if (tt) {
                    printf("ERRO: impossivel criar a thread : timer\n");
                    exit(-1);
                }

                first_pkg = 0;
            }

            nextseqnum += 250;    
        }

        /* Produzir buffer_trans_rede_env */
        pthread_mutex_lock(&env_seg_rcv_seg_timer_2);

        // Se o timer nao estourou, anda com a janela
        if (base != nextseqnum){
            
            pthread_kill(&threadTimer,SIGKILL);

            base = ack;
            base = nextseqnum;
        }
    }
}

void *receberSegmentos() {

    int expectedseqnum = 0;

    while (TRUE) {

        struct segmento segmento_rcv;

        /* Consumir buffer_trans_rede_rcv */
        pthread_mutex_lock(&mutex_trans_rede_rcv2);

        /* Retira segmento do buffer */
        retirarSegmentoBufferTransRedeRcv(&segmento_rcv);

        /* Consumir buffer_trans_rede_rcv */
        pthread_mutex_unlock(&mutex_trans_rede_rcv1);

/*
        if (Segmento de ack)
            sigkill(ThreadTimer)
        else{
*/
            // Produzir buffer_trans_trans_rcv
            pthread_mutex_lock(&mutex_trans_trans_rcv1);

            colocarSegmentoBufferTransTransRcv(segmento_rcv);

            // Produzir buffer_trans_trans_rcv
            pthread_mutex_unlock(&mutex_trans_trans_rcv1);

/*      //Enviar Sinal de ack para o remetente
        enviarack();
        }
*/
    }
}

void colocarPacoteBufferApliTransRcv(struct pacote pacote){

    //Colocar no Buffer
    buffer_apli_trans_rcv.tam_buffer = pacote.tam_buffer;
    //DEMAIS VARIAVEIS
    memcpy(&buffer_apli_trans_rcv.data, &pacote, sizeof (pacote));

}

void retirarPacoteBufferApliTransEnv(struct pacote *pacote) {

    //Retirar do Buffer
    pacote->tam_buffer = buffer_apli_trans_env.data.tam_buffer;
    strcpy(pacote->buffer, buffer_apli_trans_env.data.buffer);

}

void colocarPacoteBufferTransTransEnv(struct pacote pacote){

    //Colocar no Buffer
    buffer_trans_trans_env.tam_buffer = pacote.tam_buffer;
    memcpy(&buffer_trans_trans_env.data, &pacote, sizeof (pacote));

}

void retirarPacoteBufferTransTransRcv(struct pacote *pacote){

    //Retirar do Buffer
    pacote->tam_buffer = buffer_trans_trans_rcv.data.tam_buffer;
    memcpy(pacote, &buffer_trans_trans_rcv.data, sizeof (pacote));

}

void colocarSegmentoBufferTransTransRcv(struct segmento segment){

    //Colocar no Buffer
    buffer_trans_trans_rcv.tam_buffer = segment.tam_buffer;
    memcpy(&buffer_trans_trans_rcv.data, &segment, sizeof (segment));

}

void retirarSegmentoBufferTransTransEnv(struct segmento *segment){

    //Retirar do Buffer
    segment->tam_buffer = buffer_trans_trans_env.data.tam_buffer;
    memcpy(segment, &buffer_trans_trans_env.data, sizeof (segment));

}


void colocarSegmentoBufferTransRedeEnv(struct segmento segment){

    //Colocar no Buffer
    buffer_trans_rede_env.tam_buffer = segment.tam_buffer;
    memcpy(&buffer_trans_rede_env.data, &segment, sizeof (segment));

}

void retirarSegmentoBufferTransRedeRcv(struct segmento *segment) {

    //Retirar do Buffer
    segment->tam_buffer = buffer_trans_rede_rcv.data.tam_buffer;
    memcpy(segment, &buffer_trans_rede_rcv.data, sizeof (segment));

}

