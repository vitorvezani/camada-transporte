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

    ack = -1;
    syn = -1;

    //Inicia a thread enviarSegmentos
    tes = pthread_create(&threadEnviarSegmentos, NULL, enviarSegmentos, NULL);

    if (tes) {
        printf("ERRO: impossivel criar a thread : enviarSegmentos\n");
        exit(-1);
    }

    //Inicia a thread receberSegmentos
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

    //Inicia a thread receberPacote
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
        struct ic ic;

        /* Consumir buffer_apli_trans_env */
        pthread_mutex_lock(&mutex_apli_trans_env2);

        retirarPacoteBufferApliTransEnv(&pacote_env, &ic);

        if (buffer_apli_trans_env.tipo == CONECTAR) {

            // Alocar Buffer
            buffer_apli_trans_env.retorno = (char *) malloc(sizeof (TAM_JANELA * 2));

        } else if (buffer_apli_trans_env.tipo == DESCONECTAR) {

            // LIBERAR BUFFER
            free(buffer_apli_trans_env.retorno);

        } else if (buffer_apli_trans_env.tipo == DADOS) {

            /* Produzir buffer_trans_trans_env */
            pthread_mutex_lock(&mutex_trans_trans_env1);

                colocarPacoteBufferTransTransEnv(pacote_env, ic);

            /* Produzir buffer_trans_trans_env */
            pthread_mutex_unlock(&mutex_trans_trans_env2);

        }

    /* Consumir buffer_apli_trans_env */
    pthread_mutex_unlock(&mutex_apli_trans_env1);

    }

}

void *timer() {

#ifdef DEBBUG_TRANSPORTE
    printf("Iniciei a Thread Timer!!\n");
#endif

    sleep(2);

    // Acorda a thread de enviar segmento
    pthread_mutex_unlock(&env_seg_rcv_seg_timer_2);

}

void *enviarSegmentos() {

    int first_pkg;
    int tt;
    pthread_t threadTimer;

    int base = 0;
    int nextseqnum = 0;

    /* Consumir buffer_trans_trans_env */
    pthread_mutex_lock(&mutex_trans_trans_env2);

    while (TRUE) {

        if (base >= sizeof(buffer_trans_trans_env) && base == nextseqnum)
        {

        printf("sizeof(buffer): '%zd', base final: '%d', nextseqnum final: '%d'\n", sizeof(buffer_trans_trans_env), base, nextseqnum);

        /* Consumir buffer_trans_trans_env */
        pthread_mutex_lock(&mutex_trans_trans_env2);

        }

        #ifdef DEBBUG_TRANSPORTE
            printf("Vou enviar a Janela\n");
        #endif

        first_pkg = 1;

        while (nextseqnum < base + TAM_JANELA) {

            printf("base: '%d', nextseqnum: '%d'\n", base, nextseqnum);

            struct segmento segmento_env;

            retirarSegmentoBufferTransTransEnv(&segmento_env, nextseqnum);

            /* Consumir buffer_trans_trans_env */
            pthread_mutex_unlock(&mutex_trans_trans_env1);

            /* Produzir buffer_trans_rede_env */
            pthread_mutex_lock(&mutex_trans_rede_env1);

            // Envia os segmentos da janela

            segmento_env.flag_ack = -1;
            segmento_env.flag_connect = -1;
            segmento_env.flag_syn = -1;

            segmento_env.flag_push = 1;
            segmento_env.num_no = file_info.num_no;
            segmento_env.env_no = buffer_apli_trans_env.env_no; //LOGICA A SER MUDADA

            colocarSegmentoBufferTransRedeEnv(segmento_env);

#ifdef DEBBUG_TRANSPORTE

            printf("Mandei um Segmento\n");
            printf("flag_push = '%d'\n", segmento_env.flag_push);
            printf("env_no = '%d'\n", segmento_env.env_no);
            printf("num_no = '%d'\n", segmento_env.num_no);
            printf("tam_buffer = '%d'\n", buffer_trans_rede_env.tam_buffer);

#endif

            /* Produzir buffer_trans_rede_env */
            pthread_mutex_unlock(&mutex_trans_rede_env2);

            if (first_pkg) {

#ifdef DEBBUG_TRANSPORTE

                printf("Primeiro Segmento da Janela, iniciando thread timer\n");

#endif    

                //Inicia a thread timer
                tt = pthread_create(&threadTimer, NULL, timer, NULL);

                //Dispara Thread timer
                if (tt) {
                    printf("ERRO: impossivel criar a thread : timer\n");
                    exit(-1);
                }

                first_pkg = 0;
            }

            nextseqnum += TAM_SEGMENT;

        }

        /* Produzir buffer_trans_rede_env */
        pthread_mutex_lock(&env_seg_rcv_seg_timer_2);

        // Se o timer nao estourou, anda com a janela
        if (ack != -1) {

            pthread_cancel(threadTimer);

            base = ack;

#ifdef DEBBUG_TRANSPORTE

            printf("Recebi um pacote de ack: '%d'\n", ack);
            printf("Andei com a janela: '%d'\n", base);
#endif

            ack = -1;

        } else if (syn == 1) {

#ifdef DEBBUG_TRANSPORTE
            printf("Recebi um pacote de syn\n");
#endif
           syn = -1;

        } else {

#ifdef DEBBUG_TRANSPORTE
            printf("Thread de timer estourou. Reenviando Janela\n");
#endif

            nextseqnum = base;
        }
    }
}

void *receberSegmentos() {

    int expectedseqnum = 0;

    while (TRUE) {

        struct segmento segmento_rcv;

        /* Consumir buffer_trans_rede_rcv */
        pthread_mutex_lock(&mutex_trans_rede_rcv2);

        #ifdef DEBBUG_TRANSPORTE

        printf("RECEBI UM SEGMENTOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n");

        #endif

        /* Retira segmento do buffer */
        retirarSegmentoBufferTransRedeRcv(&segmento_rcv);


        #ifdef DEBBUG_TRANSPORTE

        printf("segmento_rcv.flag_ack: '%d'\n", segmento_rcv.flag_ack);
        printf("segmento_rcv.flag_syn: '%d'\n", segmento_rcv.flag_syn);
        printf("segmento_rcv.flag_connect: '%d'\n", segmento_rcv.flag_connect);
        printf("segmento_rcv.flag_push: '%d'\n\n", segmento_rcv.flag_push);
        printf("segmento_rcv.ack: '%d'\n\n", segmento_rcv.ack);
        printf("segmento_rcv.num_no: '%d'\n", segmento_rcv.num_no);
        printf("segmento_rcv.env_no: '%d'\n", segmento_rcv.env_no);

        #endif

        /* Consumir buffer_trans_rede_rcv */
        pthread_mutex_unlock(&mutex_trans_rede_rcv1);

        if (segmento_rcv.flag_ack == 1) {

            ack = segmento_rcv.ack;

            // Produzir buffer_trans_trans_rcv
            pthread_mutex_unlock(&env_seg_rcv_seg_timer_2);

        } else if (segmento_rcv.flag_syn == 1) {

            syn = 1;

            // Produzir buffer_trans_trans_rcv
            pthread_mutex_unlock(&env_seg_rcv_seg_timer_2);

        } else if (segmento_rcv.flag_connect == 1) {

            struct segmento segmento_env_ack;

            segmento_env_ack.flag_ack = -1;
            segmento_env_ack.flag_push = -1;
            segmento_env_ack.flag_connect = -1;

            segmento_env_ack.flag_syn = 1;
            segmento_env_ack.num_no = segmento_rcv.env_no;
            segmento_env_ack.env_no = segmento_rcv.num_no;

            /* Produzir buffer_trans_rede_env */
            pthread_mutex_lock(&mutex_trans_rede_env1);

            colocarSegmentoBufferTransRedeEnv(segmento_env_ack);

            /* Produzir buffer_trans_rede_env */
            pthread_mutex_unlock(&mutex_trans_rede_env2);

        } else if (segmento_rcv.flag_push == 1) {

            if (segmento_rcv.seqnum == expectedseqnum) {

                struct segmento segmento_env_ack;

        #ifdef DEBBUG_TRANSPORTE

                printf("SeqNum('%d') recebido == ExpectedSeqNum('%d')\n", segmento_rcv.seqnum, expectedseqnum);

        #endif

                // Produzir buffer_trans_trans_rcv
                pthread_mutex_lock(&mutex_trans_trans_rcv1);

                colocarSegmentoBufferTransTransRcv(segmento_rcv, segmento_rcv.seqnum);

                // Produzir buffer_trans_trans_rcv
                pthread_mutex_unlock(&mutex_trans_trans_rcv2);

                // Enviar ACK para nó destino

                segmento_env_ack.flag_syn = -1;
                segmento_env_ack.flag_push = -1;
                segmento_env_ack.flag_connect = -1;

                segmento_env_ack.flag_ack = 1;
                segmento_env_ack.ack = expectedseqnum + TAM_SEGMENT;

                expectedseqnum += TAM_SEGMENT;

                segmento_env_ack.num_no = file_info.num_no;
                segmento_env_ack.env_no = segmento_rcv.num_no;

                /* Produzir buffer_trans_rede_env */
                pthread_mutex_lock(&mutex_trans_rede_env1);

                colocarSegmentoBufferTransRedeEnv(segmento_env_ack);

                /* Produzir buffer_trans_rede_env */
                pthread_mutex_unlock(&mutex_trans_rede_env2);


            } else {

                #ifdef DEBBUG_TRANSPORTE

                printf("SeqNum('%d') recebido != ExpectedSeqNum('%d')\n", segmento_rcv.seqnum, expectedseqnum);
                printf("Pacote Descartado !\n");

                #endif

            }
        }
    }
}

void colocarPacoteBufferApliTransRcv(struct pacote package) {

    //Colocar no Buffer
    memcpy(&buffer_apli_trans_rcv.data, &package, sizeof (package));

}

void retirarPacoteBufferApliTransEnv(struct pacote *package, struct ic *ic) {

    //Retirar do Buffer
    memcpy(ic, &buffer_apli_trans_env.ic , sizeof (buffer_apli_trans_env.ic));
    memcpy(package, &buffer_apli_trans_env.data , sizeof (buffer_apli_trans_env.data));

}

void colocarPacoteBufferTransTransEnv(struct pacote package, struct ic ic) {

    //Colocar no Buffer
    memcpy(&buffer_trans_trans_env.tam_buffer, &buffer_apli_trans_env.tam_buffer, sizeof (int));
    memcpy(&buffer_trans_trans_env.ic, &ic, sizeof (ic));
    memcpy(&buffer_trans_trans_env.data, &package, sizeof (package));

}

void retirarPacoteBufferTransTransRcv(struct pacote *package) {

    //Retirar do Buffer
    memcpy(package, &buffer_trans_trans_rcv.data, sizeof (buffer_trans_trans_rcv.data));

}

void colocarSegmentoBufferTransTransRcv(struct segmento segment, int seqnum) {

    void *ptr;
    ptr = &(buffer_trans_trans_rcv.data);
    ptr += seqnum;

    //Colocar no Buffer
    memcpy(ptr, &segment, TAM_SEGMENT);

}

void retirarSegmentoBufferTransTransEnv(struct segmento *segment, int nextseqnum) {

    void *ptr;
    ptr = &(buffer_trans_trans_env.data);
    ptr += nextseqnum;

    segment->seqnum = nextseqnum;
    segment->num_no = file_info.num_no;
    segment->tam_buffer = buffer_trans_trans_env.data.tam_buffer;
    memcpy(&(segment->data), ptr, TAM_SEGMENT);

}


void colocarSegmentoBufferTransRedeEnv(struct segmento segment){

    //Colocar no Buffer
    buffer_trans_rede_env.env_no = segment.env_no;
    buffer_trans_rede_env.tam_buffer = segment.tam_buffer;
    memcpy(&buffer_trans_rede_env.data, &segment, sizeof (segment));

}

void retirarSegmentoBufferTransRedeRcv(struct segmento *segment) {

    //Retirar do Buffer
    memcpy(segment, &buffer_trans_rede_rcv.data, sizeof (buffer_trans_rede_rcv.data));

}