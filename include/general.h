/********************************************************************
 *    Descrição: Funções de uso Geral da placa ICIP Intelbras
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:16/12/2008
 *******************************************************************/

#ifndef __GENERAL_H_
#define __GENERAL_H_

#include <sys/stat.h>
#include <unistd.h>
#include "defin.h"
#include "app_msg.h"

//Defines
//#define POLICY	SCHED_FIFO  //Define Usado na Prioridade de Thread

/*
#define MergeFieldsToShort(SourceBitFieldPrefix)   (\
    (unsigned short)(((SourceBitFieldPrefix##H)<<8)&0xFF00)\
    + (unsigned short)(((SourceBitFieldPrefix##L)   )&0x00FF)\
)*/

/** Utility macro to compute the number of elements in static array. */
#define ARRAY_SIZE(a)    (sizeof(a)/sizeof(a[0]))

//Prototipos de funções

/*
 *
 */
void boost_priority(int priority);

//Inicia Login da aplicação
void StartLogger(void);

/*
 *
 */
void LoggerClose(void);

/*
 *
 */
void ClearContErros(void);

/*
 *
 */
void PrintContErros(void);

/*
 *
 */
void Daemonize(void);

/*
 *
 */
void app_SetLogLevel(BOOL isProg, int level);

/*
 *
 */
int app_syslog(int syslogpri, char *format, ...);

/*
 *
 */
void printLastFunc( const char* name_func);

/*
 * poe caracter de fim de string(\0) na posição do caracter passado como parametro
 * Usado quando a string origem é uma ponteiro para char
 */
BOOL app_endstr(char *pt_str, char ch);


/*
 * Verificar se string é valida
 */
bool isValid_str(char *pt_str);

/*
 * Verificar se string é valida
 */
bool isValid_str_ascii(char *pt_str);


#endif //GENERAL_H_
