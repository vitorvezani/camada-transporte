//
//  teste.c
//
//  Guilherme Sividal - 09054512
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 07/05/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#define TRUE    1

#include <stdio.h>
#include "headers/transporte.h"

int main(int argc, char const *argv[]) {
    int socket;
    int ret;

    printf("Vou criar um socket!\n");

    socket = aps();

    printf("Criei um socket ID: '%d'\n", socket);

    printf("Agora, vou deleta-lo!\n");

    ret = fps(socket);

    if (ret)
        printf("Deletado com sucesso!\n");
    else
        printf("Socket não foi deletado\n");
    
    return 0;
}