//
//  aplicacao.c
//
//  Guilherme Sividal - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 06/05/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/aplicacao.h"

void *iniciarAplicacao() {

    int te, tr;
    pthread_t threadEnviarX, threadReceberX;

    //Inicia a thread enviarDatagramas
    te = pthread_create(&threadEnviarX, NULL, enviarX, NULL);

    if (te) {
        printf("ERRO: impossivel criar a thread : enviarX\n");
        exit(-1);
    }

    //Inicia a thread enviarDatagramas
    tr = pthread_create(&threadReceberX, NULL, receberX, NULL);

    if (tr) {
        printf("ERRO: impossivel criar a thread : receberX\n");
        exit(-1);
    }

    //Espera as threads terminarem
    pthread_join(threadEnviarX, NULL);
    pthread_join(threadReceberX, NULL);

}

void *enviarX() {

    char dados_aux[128];
    char *pch;

    while (1) {

        struct X X_env;

        //Pega os Dados digitado pelo usuario
        printf("[APLIC - ENVIAR] Digite nó e data: ");
        fflush(stdout);
        fpurge(stdin);
        fgets(dados_aux, 127, stdin);
        dados_aux[strlen(dados_aux) - 1] = '\0';

        if (isdigit(dados_aux[0])) {

            //Trava o Mutex de sincronismo
            pthread_mutex_lock(&mutex_apli_trans_env1);

            pch = strtok(dados_aux, " ");

            buffer_apli_trans_env.env_no = atoi(pch);

            pch = strtok(NULL, "");

            strcpy(X_env.buffer, pch);

            //Seta tipo de msg, tamanho da msg e nó para enviar
            X_env.tam_buffer = strlen(X_env.buffer);

            //Colocar no Buffer
            buffer_apli_trans_env.tam_buffer = X_env.tam_buffer;
            memcpy(&buffer_apli_trans_env.data, &X_env, sizeof (X_env));

            //Destrava mutex de sincronismo
            pthread_mutex_unlock(&mutex_apli_trans_env2);

        } else
            printf("[APLIC - ENVIAR] data[0] :'%c' não é um int\n", dados_aux[0]);

        // TESTE DE RETORNO DA CAMADA DE REDE
        /*
        pthread_mutex_lock(&mutex_apli_trans_env1);

        //Testa o retorno da camada de enlace
        if (buffer_apli_trans_env.retorno == 0) {
            printf("[APLICACAO - Retorno] ok\n\n");
        } else if (buffer_apli_trans_env.retorno == -1) {
            printf("[APLICACAO - Retorno] não é nó vizinho: '%d'!\n\n", buffer_apli_trans_env.env_no);
        }else {
            printf("[APLICACAO - Retorno] erro fatal(2)\n\n");
        }

        pthread_mutex_unlock(&mutex_apli_trans_env1);
         */
    }
}

void *receberX() {

    while (TRUE) {

        struct X X_rcv;

        //Trava mutex de sincronismo
        pthread_mutex_lock(&mutex_apli_trans_rcv2);

        if (buffer_apli_trans_rcv.tam_buffer != -1) {

            montarSegmento(&X_rcv);

            printf("[APLIC - RECEBER] Tam_buffer: '%d' Bytes, Buffer: '%s'\n", X_rcv.tam_buffer,
                    X_rcv.buffer);

        }

        //Destrava mutex de sinconismo
        pthread_mutex_unlock(&mutex_apli_trans_rcv1);
    }

}

void montarSegmento(struct X *X) {

    X->tam_buffer = buffer_apli_trans_rcv.data.tam_buffer;
    strcpy(X->buffer, buffer_apli_trans_rcv.data.buffer);

}