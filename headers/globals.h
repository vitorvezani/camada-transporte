//
//  globals.h
//
//  Guilherme Sividal    - 09054512 
//  Vitor Rodrigo Vezani - 10159861
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>		    /* for printf() */
#include <stdlib.h>			/* for exit() */
#include <string.h>			/* for string manipulation() */
#include <pthread.h>     	/* para poder manipular threads */
#include <stdlib.h>			/* for exit() */
#include <sys/socket.h>
#include <netinet/in.h>

// Defines
#define MAXNOS          6

#define TRUE 	1
#define FALSE	0

#define TAM_MAX_BUFFER 1400

#define NOS             1
#define ENLACES         2

#define MAX_BUFFERS_DESFRAG 5

#define INFINITO 999999

//#define DEBBUG_ENLACE
//#define DEBBUG_REDE
//#define DEBBUG_REDE_FRAGMENTAR
//#define DEBBUG_REDE_DESFRAGMENTAR
//#define DEBBUG_MONTAR_TABELA
//#define DEBBUG_ROTEAMENTO
//#define DEBBUG_ARQUIVO

// Variaveis Globais

int nos_vizinhos[6]; 						//Nós vizinhos
int flag_id; 								// Inicializa ID em 1
int flag_iniciei; 							// Enviar tabela de rotas à vizinhos
int flag_saida; 							//Qual nó enviar

pthread_mutex_t mutex_rede_enlace_env1, mutex_rede_enlace_env2;
pthread_mutex_t mutex_rede_enlace_rcv1, mutex_rede_enlace_rcv2;
pthread_mutex_t mutex_rede_rede_atualizei1, mutex_rede_rede_atualizei2;
pthread_mutex_t mutex_rede_rede_receberotas2;
pthread_mutex_t mutex_rede_rede_env1, mutex_rede_rede_env2;
pthread_mutex_t mutex_rede_rede_rcv1, mutex_rede_rede_rcv2;
pthread_mutex_t mutex_trans_rede_env1, mutex_trans_rede_env2;
pthread_mutex_t mutex_trans_rede_rcv1, mutex_trans_rede_rcv2;
pthread_mutex_t mutex_trans_trans_env1, mutex_trans_trans_env2;
pthread_mutex_t mutex_trans_trans_rcv1, mutex_trans_trans_rcv2;
pthread_mutex_t mutex_apli_trans_env1, mutex_apli_trans_env2;
pthread_mutex_t mutex_apli_trans_rcv1, mutex_apli_trans_rcv2;

/* Estrutura do pacote */
struct pacote {
    //DEFINIR
    int tam_buffer;
    char buffer[TAM_MAX_BUFFER];
};

struct buffer_apli_trans {
    int tipo;
    int tam_buffer;
    int env_no;
    int retorno;
    struct pacote data;
};

/* Estrutura da tabela de rotas */
struct tabela_rotas {
	int quem_enviou;
    int destino;
    int custo;
    int saida;
};

/* Estrutura do segmento */
struct segmento {
    int tam_buffer;
    char buffer[TAM_MAX_BUFFER];
};

/* uniao do segmento ou tabela de rotas */
union segmento_tabela { 
    struct segmento segmento;
    struct tabela_rotas tabela_rotas[6];
};

/* Estrutura do datagrama */
struct datagrama {
    int tam_buffer;
    int offset;
    int id;
    int mf;
    int type;
    int env_no;
    int num_no;
    int retorno;
    union segmento_tabela data;
};

/* Estrutura do buffer entre transporte e rede */
struct buffer_trans_rede {
    int tam_buffer;
    int env_no;
    int retorno;
    struct segmento data;
};

/* Estrutura do buffer entre rede e enlace */
struct buffer_rede_enlace {
    int tam_buffer;
    int env_no;
    int retorno;
    struct datagrama data;
};

/* Estrutura do Frame */
struct frame {
    int tam_buffer;
    int ecc;
    struct datagrama data;
};

struct ligacoes {
    char nos[6][3][35];
    int enlaces[18][3];
};

struct file {
    char file_name[20];
    int num_no;
};

struct tabela_rotas tabela_rotas[6]; 		// Tabela de Rotas Interna do nó 

struct datagrama buffers_fragmentacao[MAX_BUFFERS_DESFRAG]; // Buffer interno de fragmentos
struct datagrama buffer_rede_rede_env, buffer_rede_rede_rcv; // Buffer interno entre threads

struct buffer_apli_trans buffer_apli_trans_env, buffer_apli_trans_rcv;
struct buffer_rede_enlace buffer_rede_enlace_env, buffer_rede_enlace_rcv;
struct buffer_trans_rede buffer_trans_rede_env, buffer_trans_rede_rcv;

struct file file_info;

struct ligacoes ligacao;

//Variaveis Globais à Camada de Rede

pthread_mutex_t mutex_buffer_rede_env, mutex_buffer_rede_rcv;

//Threads Inicializador
void *inicializarCamadas();
void *iniciarEnlace();
void *iniciarRede();
void *iniciarTransporte();
void *iniciarAplicacao();

// Threads da Camada de Enlace
void *enviarFrames();
void *receberFrames();

//Threads da Camada de Rede
void *enviarTabelaRotas();
void *receberTabelaRotas();
void *receberDatagramas();
void *receberSegmento();
void *enviarDatagrama();
void *enviarSegmento();

// Threads da Camada de Transporte
void *enviarSegmentos();
void *receberSegmentos();
void *enviarPacote();
void *receberPacote();

//Threads da Camada de Aplicação 
void *enviarPacotes();
void *receberPacotes();

//Funções Arquivos
void delete_espace(char* input);

//Funções da Camada de Enlace
void colocarArquivoStruct(FILE * fp);
void retirarEspaco(char * string);
void colocarBufferFrame(struct frame *frame);
void colocarDatagramaBuffer(struct frame frame);
int checkSum(struct datagrama datagram);

//Funções da Camada de Rede
void atualizarTabelaRotas(struct datagrama datagram);
void fragmentarDatagramaEnv(struct datagrama datagram);
void desfragmentarDatagramaRcv(struct datagrama datagram, int *index);
void enviarDatagramaNoNaoV(struct datagrama datagram);

void retirarDatagramaBufferRedeRedeRcv(struct datagrama *datagram);
void retirarDatagramaBufferRedeRedeEnv(struct datagrama *datagram);
void retirarDatagramaBufferRedeEnlaceRcv(struct datagrama *datagram);
void retirarSegmentoBufferTransRedeEnv(struct datagrama *datagram);

void colocarDatagramaBufferRedeRedeEnv(struct datagrama datagrama);
void colocarDatagramaBufferRedeRedeRcv(struct datagrama datagrama);
void colocarDatagramaBufferTransRedeEnv(struct datagrama datagrama);
void colocarDatagramaBufferTransRedeRcv(struct datagrama datagrama);

void colocarDatagramaBufferRedeEnlaceEnv(struct datagrama datagrama);
int retornoEnlace(struct datagrama datagram);

void resetarBuffer(struct datagrama *datagram);

void montarTabelaRotasInicial();
void montarDatagramaTabelaRotas(struct datagrama *datagram);
void enviarTabelaRotasVizinhos(struct datagrama *datagram);

//Funções da Camada de Transporte
void retirarPacoteBufferTransTransRcv(struct segmento *pacote);
void colocarPacoteBufferApliTransRcv(struct segmento pacote);

void retirarSegmentoBufferTransTransRcv(struct segmento *segment);
void colocarSegmentoBufferTransRedeRcv(struct segmento segment);

void retirarPacoteBufferApliTransEnv(struct segmento *pacote);
void colocarPacoteBufferTransRedeEnv(struct segmento pacote);

void retirarSegmentoBufferTransRedeRcv(struct segmento *segment);
void colocarSegmentoBufferTransTransRcv(struct segmento segment);

//Funções da Camada de Aplicacao
void colocarPacotesBufferApliTransEnv(struct pacote pacote);
void retirarPacotesBufferApliTransRcv(struct pacote *pacote);
void retornoTransporte(struct pacote pacote);

// API Global
int aps();
int fps(int ps);
int conectar(int env_no, int ps);
int desconectar(int ic);
void baixar(int ic, void *arq);
