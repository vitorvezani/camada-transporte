//
//  meuprog.c
//
//  Guilherme Sividal    - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 07/05/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#define TRUE    1

#include "headers/globals.h"

int main(int argc, char const *argv[]) {

    int tic;
    char dados_aux[128];
    char *pch;
    pthread_t threadinicializarCamadas;

    int oper; 

    int env_no = 2;

    char buffer[50];

    struct ic ic;

    int ps, ret_fps;

    char *arq;

	pch = (char*) malloc (128);

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
    }

    usleep(20000);

    /* Se desapega da Thread */
    //pthread_detach(threadinicializarCamadas);

if (file_info.num_no == 1)
{

    getchar();

    ps = aps();

    if (ps == -1)
        printf("Impossivel criar ps\n");
    else
        printf("ps criado '%d'\n", ps);

    getchar();

    ic = conectar(env_no, ps);

    if(ic.env_no == -1)
        printf("Erro ao conectar, verifique  a existencia do ps\n");
    else
        printf("Conectado com sucesso!\n");

    getchar();

    //desconectar(ic);

    baixar(ic, "Linux é um termo utilizado para se referir a sistemas operativos (português europeu) ou sistemas operacionais (português brasileiro) que utilizem o núcleo Linux. O núcleo Linux foi desenvolvido pelo programador finlandês Linus Torvalds, inspirado no sistema Minix. O seu código fonte está disponível sob a licença GPL (versão 2) para que qualquer pessoa o possa utilizar, estudar, modificar e distribuir livremente de acordo com os termos da licença.");
}

/*
    //Pega os Dados digitado pelo usuario

    printf("[MEUPROG] INFO: 1.aps(), 2.fps(ips), 3.conectar(no, ips), 4.desconectar(ic), 5.baixar(ic, nome_arq), 6.sair\n");
    while(oper != 6){

        printf("[MEUPROG] [oper] [nó] [data]: ");
        fflush(stdout);
        fpurge(stdin);
        fgets(dados_aux, 127, stdin);
        dados_aux[strlen(dados_aux) - 1] = '\0';

        if (isdigit(dados_aux[0])){

            pch = strtok(dados_aux, " ");

            oper = atoi(pch);

//      if(dados_aux[5] != NULL){ 
//   
//	        pch = strtok(NULL, "");
//
//	        strcpy(buffer, pch);
//		}

            
            switch (oper){
                case 1:

                    ps = aps();

                    if (ps == -1)
                        printf("Impossivel criar ps\n");
                    else
                        printf("ps criado '%d'\n", ps);

                break;

                case 2:

                    ret_fps = fps(ps);

                    if (ret_fps != -1)
                        printf("ps: '%d' desconectado!\n", ps);

                break;

                case 3:

                    pch = strtok(NULL, " ");

                    env_no = atoi(pch);

                    printf("env_no: '%d', ps: '%d'\n", env_no, ps);

                    ic = conectar(env_no, ps);

                    if(ic.env_no == -1)
                        printf("Erro ao conectar, verifique  a existencia do ps\n");
                    else
                        printf("Conectado com sucesso!\n");

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
            printf("[MEUPROG] data[0] ou data[2] não é um int\n");
*/

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
//    }


    printf("Esperando thread threadinicializarCamadas terminar\n");

    /* Espera a Thread terminar */
    pthread_join(threadinicializarCamadas, NULL);

    return 0;
}
