/********************************************************************
 *    Descrição: Funções referentes a manipulações de Lista
 *    			 encadeadas.
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:18/02/2009
 *******************************************************************/

#ifndef _KEYS_H_
#define _KEYS_H_

//Includes
#include "def_types.h"
#include "app_msg.h"

/*
 *
 */
void *GetKey(char *nameKey);


/*
 * Só usar na Funcao GetKey
 * Pois ele nao faz Validacao do nome da TKEY
 */
void *findKey(char *nameKey);

/*
 *
 */
void *FindKey(char *nameKey);


int isNewInBLFList(char *nameKey);

/*
 * Inclui um Ramal/Juntor na Lista da Tecla
 * Isso srá usado para enviar o status da tecla
 * Quando mudar de STATUS
 */
//int IncludeNewDNSInKey(char *nameKey,  word indDNSDriverIP, byte position, byte selelect);
int IncludeNewDNSInKey(TAG_SUBSCRIBE_BLF *ptr_blfdata);


/*
 *
 */
int isInListPort(void *list, word index);

/*
 *
 */
int ChangeStatusInKey(char *nameKey,  word newStatus, enum  DirChamada directionCall);


/*
 * Ixclui um Ramal/Juntor na Lista da Tecla
 */
int ExcludeDNSInKey(void *list, word indDNSDriverIP);


/*
 * Faz uma vareedura e envia para o PABX
 * Para manter os estados ativos
 */
void Resfresh_status(void);

#endif /* KEYS_H_ */
