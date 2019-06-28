/********************************************************************
 *    Descrição: Tratamento das mensagens recebidas do PABXCOMM
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:16/12/2008
 *******************************************************************/

#ifndef APP_COMM_H_
#define APP_COMM_H_

#include "def_types.h"
#include "def_enum.h"
#include <sys/un.h>

//_________________ DEFINES __________________________________
#define     SERVER_PATH                    "/tmp/server"
/*====================================================================================================
 *			Prototipos de funções
 *====================================================================================================
 */
int     Aguarda_comunicacao_com_PABX(void);
int 	PABXCOMM_socket(void *arg);  			//Prototipagem de função
void 	InitPABXCOMM(void);						//Inicialização de socket cliente
int 	RecebeDadosPABXCOMM(byte *data);				//Função de recepção de dados via socket
void 	EnviaDadosPABXCOMM( byte *umaMsg, word sizeFrame);

//void    TrataMensagemRecebida (TAG_MESSAGE_VOIP *ptrMsg);
void    FormataFrameSEND(word aplicDest,byte tipo ,word nMsg, byte pos, byte sel, byte *ptrStruct, word sizeStruct);

int     icip_critic_situation(void);

/*
 * le o tipo do descritor do Dispositivo
 */
//int icip_hardware_readyType(void);


#endif //APP_COMM_H_


