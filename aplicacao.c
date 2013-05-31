//
//  aplicacao.c
//
//  Guilherme Sividal    - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 06/05/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/globals.h"

void *iniciarAplicacao() {

    int i;

    for (i = 0; i < MAX_PS; i++)
        ps[i] = -1;

    int te, tr;
    pthread_t threadEnviarPacotes, threadReceberPacotes;

    //Inicia a thread enviarPacotes
    te = pthread_create(&threadEnviarPacotes, NULL, enviarPacotes, NULL);

    if (te) {
        printf("ERRO: impossivel criar a thread : enviarPacotes\n");
        exit(-1);
    }

    //Inicia a thread receberPacotes
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

int aps() {

    int i;

    for (i = 0; i < MAX_PS; ++i) {
        if (ps[i] == -1) {
            ps[i] = 1;
            return i;
        }
    }
    return -1;
}

int fps(int num_ps) {

    ps[num_ps] = -1;

    printf("Recebi pedido de fechamento do ps '%d'\n", num_ps);

    return 1;
}

struct ic conectar(int env_no, int num_ps) {

    struct ic ic;

    int i;
    int flag_existe = 0;

    struct pacote pacote_env;

    printf("Recebi pedido para conectar no no : '%d', ps '%d'\n", env_no, num_ps);

    for (i = 0; i < MAX_PS; i++)
        if (ps[num_ps] == 1)
            flag_existe = 1;

    if (flag_existe == 1) {
        /* Produzir buffer_rede_enlace_env */
        pthread_mutex_lock(&mutex_apli_trans_env1);

        pacote_env.tipo = CONECTAR;

        colocarPacotesBufferApliTransEnv(pacote_env, ic);

        /* Produzir buffer_rede_enlace_env */
        pthread_mutex_unlock(&mutex_apli_trans_env2);

        /* Consome resposta da camada de enlace */
        pthread_mutex_lock(&mutex_apli_trans_env1);

        retornoTransporte(pacote_env);

        /* Consome resposta da camada de enlace */
        pthread_mutex_unlock(&mutex_apli_trans_env1);

        ic.end_buffer = pacote_env.retorno;
        ic.env_no     = env_no;
        ic.ps         = num_ps;
        ic.num_no     = file_info.num_no;

        return ic;

    } else {

        ic.env_no = -1;
        return ic;

    }
}

int desconectar(struct ic ic) {

    struct pacote pacote_env;

    printf("Recebi pedido para desconectar do env_no: '%d' e ps: '%d'\n", ic.env_no, ic.ps);

    /* Produzir buffer_rede_enlace_env */
    pthread_mutex_lock(&mutex_apli_trans_env1);

    colocarPacotesBufferApliTransEnv(pacote_env, ic);

    pacote_env.tipo = DESCONECTAR;

    /* Produzir buffer_rede_enlace_env */
    pthread_mutex_unlock(&mutex_apli_trans_env2);

    /* Consome resposta da camada de enlace */
    pthread_mutex_lock(&mutex_apli_trans_env1);

    retornoTransporte(pacote_env);

    /* Consome resposta da camada de enlace */
    pthread_mutex_unlock(&mutex_apli_trans_env1);

    return 1;
}

void enviar(struct ic ic, char *arq) {

    struct pacote pacote_env;

    printf("'%s'\n", arq);
    printf("'%zd'\n", strlen(arq));

    //Produz no buffer apli_trans
    pthread_mutex_lock(&mutex_apli_trans_env1);

    pacote_env.tipo = DADOS;
    pacote_env.tam_buffer = strlen(arq);
    strncpy(pacote_env.buffer, arq, strlen(arq) + 1);

    pacote_env.buffer[strlen(pacote_env.buffer) - 1] = '\0';

    colocarPacotesBufferApliTransEnv(pacote_env, ic);

    //Produz no buffer apli_trans
    pthread_mutex_unlock(&mutex_apli_trans_env2);

}

void colocarPacotesBufferApliTransEnv(struct pacote pacote, struct ic ic) {

    // Colocar no Buffer
    buffer_apli_trans_env.tam_buffer = pacote.tam_buffer;
    buffer_apli_trans_env.tipo = pacote.tipo;
    buffer_apli_trans_env.env_no = ic.env_no;

    // Copiando Dados do Pacote para o Buffer
    memcpy(&buffer_apli_trans_env.ic, &ic, sizeof (ic));
    memcpy(&buffer_apli_trans_env.data, &pacote, sizeof (pacote));

}

void retirarPacotesBufferApliTransRcv(struct pacote *pacote) {

    pacote->tam_buffer = buffer_apli_trans_rcv.data.tam_buffer;
    strcpy(pacote->buffer, buffer_apli_trans_rcv.data.buffer);

}

void retornoTransporte() {

    // Não houve erro de malloc ou free
    if (buffer_apli_trans_env.retorno != NULL)

        if (buffer_apli_trans_env.tipo == CONECTAR) {
            printf("Conexão estabelecida com sucesso! end_buffer: '%p' \n", buffer_apli_trans_env.retorno);
        } else if (buffer_apli_trans_env.tipo == DESCONECTAR) {
            printf("Conexão encerrada com sucesso! end_buffer: '%p' \n", buffer_apli_trans_env.retorno);
        }
}