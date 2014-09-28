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
    pthread_t threadinicializarCamadas;

    int env_no = 3;

    char cmd[128];
    char *ptrFuncao, *ptrParam1, *ptrParam2;

    struct ic ic, ic2;

    int ps, ps2;

    /* Testa Parametros */
    if (argc != 3) {
        printf("Use: %s 'file_name.ini' 'numero_nó'\n\n", argv[0]);
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

    printf("Funções Disponíveis:\n  -'aps()'\n  -'fps()'\n  -'conectar()'\n  -'desconectar()'\n  -'baixar()'\n");
    printf("Digite a função e os respectivos parametros\n");

while (TRUE){

    printf("$ ");
    fgets(cmd , 127 , stdin);
    cmd[strlen(cmd)-1]='\0';

    ptrFuncao = strtok(cmd," ");

    if(strcasecmp(ptrFuncao,"aps") == 0)
    {
        ps = aps();

        if (ps == -1)
            printf("Impossivel criar ps\n");
        else
            printf("ps criado '%d'\n", ps);

    }else if(strcasecmp(ptrFuncao,"fps") == 0)
    {
        int fpsRet = 0;

        ptrParam1 = strtok(NULL," ");

        fpsRet = fps(atoi(ptrParam1));

        if (fpsRet == 1)
            printf("ps fechado!\n");
        else
            printf("ps '%d' não existente\n",atoi(ptrParam1));
        
    }else if(strcasecmp(ptrFuncao,"conectar") == 0)
    {

        ptrParam1 = strtok(NULL," ");
        ptrParam2 = strtok(NULL," ");

        printf("conectar %d %d\n",atoi(ptrParam1), atoi(ptrParam2));

    }else if(strcasecmp(ptrFuncao,"desconectar") == 0)
    {

        ptrParam1 = strtok(NULL," ");

        printf("desconectar %d\n", atoi(ptrParam1));

    }else if(strcasecmp(ptrFuncao,"baixar") == 0)
    {

        ptrParam1 = strtok(NULL," ");
        ptrParam2 = strtok(NULL," ");

        printf("baixar %d %s\n",atoi(ptrParam1), ptrParam2);

    }else
        printf("opção invalida!\n");

/*
    getchar();


    getchar();

    ic = conectar(env_no, ps);

    if(ic.env_no == -1)
        printf("Erro ao conectar, verifique  a existencia do ps\n");
    
    getchar();

    baixar(ic, "O núcleo Linux foi desenvolvido pelo programador finlandês Linus Torvalds, inspirado no sistema Minix. O seu código fonte está disponível sob a licença GPL (versão 2) para que qualquer um o possa utilizar, estudar, modificar e distribuir livremente de acordo com os termos da licença.");

    getchar();

    desconectar(ic);

    getchar();

    ps2 = aps();

    if (ps2 == -1)
        printf("Impossivel criar ps\n");
    else
        printf("ps criado '%d'\n", ps2);

    getchar();

    ic2 = conectar(env_no - 1, ps2);

    if(ic2.env_no == -1)
        printf("Erro ao conectar, verifique  a existencia do ps\n");
    else
        printf("Conectado com sucesso!\n");

    getchar();

    baixar(ic2, "Microsoft Windows é uma popular família de sistemas operacionais criados pela Microsoft, empresa fundada por Bill Gates e Paul Allen. Antes da versão NT, era uma interface gráfica para o sistema operacional MS-DOS.");

    getchar();

    desconectar(ic2);
*/

}
    /* Espera a Thread terminar */
    pthread_join(threadinicializarCamadas, NULL);

    return 0;
}
