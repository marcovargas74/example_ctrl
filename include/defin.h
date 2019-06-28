/******************************************************************* *
 *    Descrição: Defines para ICIP - Aplicação VoIP
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:22/07/2008
 *******************************************************************/

#ifndef DEFIN_H_
#define DEFIN_H_


// Includes.
#include <syslog.h>
#include <sys/socket.h>
#include "def_enum.h"

//static const char STR_COMMAND_NEW_STATUS[] = { "Command: new_status\r\n" };


#define         MAX_TYPE_ACCEPT         3 //Numero maximo de Tipos de suscribe

#define  SIZE_NIBBLE_BIT  4

#define  SIZE_BOOL      1
#define  SIZE_BYTE      1
#define  SIZE_WORD      2
#define  SIZE_DWORD     4

#define  NUM_BITS_BYTE   8
#define  NUM_BITS_WORD   16

//---------------------- DEFINES ----------------------------------
//Defines de Versao
#define _VER_SOFT    "19"     //Versao da Aplicação VOIP - ANO
#define _REV_SOFT    "06"     //Revisao da Aplicação VOIP - MES
#define _REL_SOFT    "27"    //RELEASE Aplicação VOIP - DIA
//#define _V_DBG_SOFT  "DBG"  //Versao da Aplicação ICIP Modo DEBUG

#define PRINT_SYSLOG  1
#if MOD_DEBUG
  #define LEVEL_LOG   3
#else //MODO RELEASE
  #define LEVEL_LOG   1
#endif //ICIP_MOD_DEBUG

#define   SUCCESS       0               // Status está OK.
#define   ERROR	       -1               // ERRO

#define ERRNO_START      20000
#define ECANCELLED       (ERRNO_START + 14)/* 20014 */
#define EINVALID         (ERRNO_START + 15)/* 20014 */
#define ENEXIST          (ERRNO_START + 16)/* 20014 */

//Usado em váriaveis Bool
#define   true          1               //Verdadeiro
#define   false         0               //Falso
#define   TRUE	        1               //Verdadeiro
#define   FALSE         0               //Falso
#define   ON	        1               //Ligado
#define   OFF	        0               //Desligado
#define   LOOP	        1               //Loop
#define   FAIL          0xFA            //FALHA

#define   INVALID_RTP_PORT 0xFFFF


//Ficou definido casa dos 10 prioridade blf_ctrl
//QUANTO MAIS ALTO MAIOR PRIORIDADE
#define   THREAD_PRIO_BASE         10  //Prioridade BASE das threads da ICIP
#define   THREAD_PRIO_MAIN         THREAD_PRIO_BASE+0  //Prioridade BASE das threads da ICIP
#define   THREAD_PRIO_PABXCOM THREAD_PRIO_BASE+1  //Prioridade da thread  do socket de comunicação com o PABX
//Defines de compilação do Código
#define   SHOW_PID_THERAD                   0    //Mostra Numero do PID da THREAD

//.................. DEFINICOES DE TESTE DE IP ..............................................
  //1 Ativa o novo modo de definir o IP que será Usado pela placa ICIP em cada Chamada
#define   TESTE_IP_NOK                       0    //Teste NOK
#define   TESTE_IP_OK                        1    //Teste OK
#define   TESTE_IP_TESTANDO                  2    //Teste OK

#define KILL_NOT   0
#define KILL_NOW   1
#define KILL_WAIT  2

//1% - 450
#define MAX_MEMO_TO_KILL_WAIT   15000//(30*500)  //30%
#define MAX_MEMO_TO_KILL_NOW    17500//(35*500)  //35%

//Defines de Log
//#define   SHOW_LOGGER                       1    // ATIVA LOGGER 1 SHOW LOGGER copia dados para o syslog
#define   LOG_PRINTF                        0    // 1 Modo mostra os printf do syslog Para Tudo
#define   SHOW_FRM_TO_PABXCOMM              0    // Mostra Log de mensgens enviadas pelo socket PABXCOMM
#define   SHOW_FRM_FROM_PABXCOMM            0    // Mostra mensagens recebidas do pabxcomm via socket

#define SOCKET_LINUX    0

//#define   IP_LOCAL_HOST                 "127.0.0.1"
//#define   SIP_DEFAULT_PORT	        5060
#define   ERR_ADDR_IN_USE                 120098       //Address Already In USe


//  Defines das TAGS das Mensagens
#define   APP_MAX_NOME_VERSAO            16
#define   APP_MAX_DATA_BROADCAST          10
#define   APP_MAX_STATUS_DEVICE          52
#define   APP_MAX_NOME_PORTA             11
#define   APP_MAX_NOME_PORTA_2           12
#define   APP_MAX_STR_DEFAULT            50
#define   APP_MIN_INDEX                  100
#define   APP_MAX_INDEX                  65000
#define   EXPIRATION_DEFAULT             690

#define   EMPTY                        -1
#define   CANAL_VAGO                   0xFF
#define   OFFSET_EST                   0

#define   SWAP_BYTE(Data) 	      ((Data >> 8) & 0xFF) | ((Data & 0xFF) << 8);

#define MIN_(var,val)    ((int)val < (int)var) ? val : var
#define msleep_(mseg)    usleep(mseg*1000)


//a Variavel var-max sempre é o limiteSe
//#define POS_FIM_STRING_( var, var_max) ((int)var_max <= (int)var) ? (var_max-1) : var
#define POS_FIM_STRING_( var, var_max) ((int)var < (int)var_max) ? var : (var_max-1)

//Cuidado este define vai pagar o nome do arquivo e funcao aonde for chamado
#define __THIS_FILE__ THIS_FILE, __func__

#endif /*DEFIN_H_*/

