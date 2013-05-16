//
//  meuprog.c
//
//  Guilherme Sividal - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 07/05/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#define TRUE    1

#include "headers/globals.h"
#include "headers/arquivo.h"

int main(int argc, char const *argv[]) {

    int tic;
    char dados_aux[128];
    char *pch;
    pthread_t threadinicializarCamadas;

    int oper, no_dst;
    char buffer[50];

    int ic,ips;
    char *arq;

    int socket;
    int ret;

    /* Testa Parametros */
    if (argc != 3) {
        printf("Use: %s 'file_name.ini' 'numero_nó'\n", argv[0]);
        exit(1);
    }

    /* Copia para as Variaveis */
    strcpy(file_info.file_name, argv[1]);
    file_info.num_no = atoi(argv[2]);

    printf("Nome do arquivo: '%s'\n Numero nó: '%d'\n", file_info.file_name, file_info.num_no);

    tic = pthread_create(&threadinicializarCamadas, NULL, inicializarCamadas, NULL);

    if (tic) {
        printf("ERRO: impossivel criar a thread : inicializarCamadas\n");
        exit(-1);
    }else
        printf("inicializarCamadas iniciada com sucesso!\n");

    usleep(3000);

    /* Se desapega da Thread */
    //pthread_detach(threadinicializarCamadas);

    //Pega os Dados digitado pelo usuario
    while(oper != 6){

        printf("[MEUPROG]1- aps, 2-fps, 3-conectar, 4-desconectar, 5-baixar, 6-sair\n");
        printf("[MEUPROG] oper, nó, data: ");
        fflush(stdout);
        fpurge(stdin);
        fgets(dados_aux, 127, stdin);
        dados_aux[strlen(dados_aux) - 1] = '\0';

        if (isdigit(dados_aux[0]) && isdigit(dados_aux[2])) {

            pch = strtok(dados_aux, " ");

            oper = atoi(pch);

            pch = strtok(NULL, " ");

            no_dst = atoi(pch);

            pch = strtok(NULL, "");

            strncpy(buffer, pch, 50);

            switch (oper){
                case 1:
                    ips = aps();
                break;

                case 2:
                    fps();
                break;

                case 3:
                    ic = conectar(no_dst, ips);
                break;

                case 4:
                    desconectar(ic);
                break;

                case 5:
                    baixar(ic, arq);
                break;

                case 6:
                    printf("Sair da aplicacao\n");
                break;

                default:
                    printf("Opção inválida!\n");
            }

        } else
            printf("[MEUPROG] data[0] ou data[2]:'%c'/'%c' não é um int\n", dados_aux[0],dados_aux[2]);

        // TESTE DE RETORNO DA CAMADA DE REDE
        /*
        pthread_mutex_lock(&mutex_apli_trans_env1);

        //Testa o retorno da camada de enlace
        if (buffer_apli_trans_env.retorno == 0) {
            printf("[MEPROG] ok\n\n");
        } else if (buffer_apli_trans_env.retorno == -1) {
            printf("[MEPROG] não é nó vizinho: '%d'!\n\n", buffer_apli_trans_env.env_no);
        }else {
            printf("[MEPROG] erro fatal(2)\n\n");
        }

        pthread_mutex_unlock(&mutex_apli_trans_env1);
         */
    }
/*
    printf("Vou criar um socket!\n");

    socket = aps();

    printf("Criei um socket ID: '%d'\n", socket);

    printf("Agora, vou deleta-lo!\n");

    ret = fps(socket);

    if (ret)
        printf("Deletado com sucesso!\n");
    else
        printf("Socket não foi deletado\n");
    
*/

    /* Espera a Thread terminar */
    pthread_join(threadinicializarCamadas, NULL);

    return 0;
}