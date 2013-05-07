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
    pthread_t threadEnviarPacotes, threadReceberPacotes;

    //Inicia a thread enviarDatagramas
    te = pthread_create(&threadEnviarPacotes, NULL, enviarPacotes, NULL);

    if (te) {
        printf("ERRO: impossivel criar a thread : enviarPacotes\n");
        exit(-1);
    }

    //Inicia a thread enviarDatagramas
    tr = pthread_create(&threadReceberPacotes, NULL, receberPacotes, NULL);

    if (tr) {
        printf("ERRO: impossivel criar a thread : receberPacotes\n");
        exit(-1);
    }

    //Espera as threads terminarem
    pthread_join(threadEnviarPacotes, NULL);
    pthread_join(threadReceberPacotes, NULL);

}

void *enviarPacotes() {

    char dados_aux[128];
    char *pch;

    while (1) {

        struct pacote pacote_env;

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

            strcpy(pacote_env.buffer, pch);

            //Seta tipo de msg, tamanho da msg e nó para enviar
            pacote_env.tam_buffer = strlen(pacote_env.buffer);

            colocarPacotesBufferApliTransEnv(pacote_env);

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

void *receberPacotes() {

    while (TRUE) {

        struct pacote pacote_rcv;

        //Trava mutex de sincronismo
        pthread_mutex_lock(&mutex_apli_trans_rcv2);

        if (buffer_apli_trans_rcv.tam_buffer != -1) {

            retirarPacotesBufferApliTransRcv(&pacote_rcv);

            printf("[APLIC - RECEBER] Tam_buffer: '%d' Bytes, Buffer: '%s'\n", pacote_rcv.tam_buffer,
                    pacote_rcv.buffer);
        }

        //Destrava mutex de sinconismo
        pthread_mutex_unlock(&mutex_apli_trans_rcv1);
    }

}

void colocarPacotesBufferApliTransEnv(struct pacote pacote){

    //Colocar no Buffer
    buffer_apli_trans_env.tam_buffer = pacote.tam_buffer;
    memcpy(&buffer_apli_trans_env.data, &pacote, sizeof (pacote));

}

void retirarPacotesBufferApliTransRcv(struct pacote *pacote) {

    pacote->tam_buffer = buffer_apli_trans_rcv.data.tam_buffer;
    strcpy(pacote->buffer, buffer_apli_trans_rcv.data.buffer);

}

int aps(){
    int ips;

   return ips;
}
int fps(){

    return 1;
}
int conectar(int env_no, int ips){
    int ic;


    return ic;
}
int desconectar(int ic){

    return 1;
}

void baixar(int ic, void *arq){

}