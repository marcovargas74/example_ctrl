/********************************************************************
 *    Descrição: Funções de uso Geral da placa ICIP Intelbras
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:16/12/2008
 *******************************************************************/

#include "defin.h"
#include "general.h"
#include <stdio.h>
#include <stdarg.h>
#include <sched.h>
#include <sys/resource.h>
#include "ctype.h"

#define THIS_FILE   "general.c"


//Declaração de Váriaveis externas
extern struct app App_ctrl;

//Variaveis Globais
Status_VoIP  StatusVoIP;
const char *NameArquivoGlobal;
const char *NameFuncaoGlobal;


#define _storage_func(args_x) NameArquivoGlobal = va_arg(args_x, char *); NameFuncaoGlobal = va_arg(args_x, char *) ;

/*
//Define Prioridade da Thread
void boost_priority(int priority)
{
  struct sched_param param;

  param.sched_priority = priority;
  if(sched_setscheduler(0, SCHED_RR, &param) == -1)
    {
      app_syslog( LOG_ERR, "%s->%s(){FALHA NO ESCALADOR DE PRIORIDADE }<pid>[%d]", __THIS_FILE__, getpid() );
      //exit(-1);
    }
  app_syslog( LOG_DEBUG, "%s->%s{NOVO PROCESSO}<_priority>[%d]<PID>[%d]", __THIS_FILE__, param.sched_priority, getpid() );

}//void boost_priority(void)
*/


//============================= LOGGIN  ==================================

/*
 *
 */
void app_SetLogLevel(BOOL isProg, int level)
{
 App_ctrl.Level_LOG_Prog = level;
 App_ctrl.Is_LOG_Prog    = isProg;
 setlogmask (LOG_UPTO (level));

}

//Inicia Login da aplicação
void StartLogger(void)
{
  app_SetLogLevel(TRUE, LOG_DEBUG);
  openlog("app_ctrl",0, LOG_LOCAL7);
  syslog( LOG_INFO, "%s->%s():sys/Log  Iniciado ", __THIS_FILE__);
}

void LoggerClose(void)
{
  app_syslog(LOG_INFO, "%s->%s(){APP_CTRL_FINISH}",__THIS_FILE__);
  printf("sys/Log  Finalizado\n");
  closelog();
}


/*
 *
 */
void ClearContErros(void)
{
  //  memset(&StatusVoIP, 0, sizeof(Status_VoIP) );
  StatusVoIP.ErrorAlloc = 0 ;          //Erros de alocação de memória;
  StatusVoIP.ErrorComSocket = 0;       //Erros de cominicação com sockets;
  StatusVoIP.Generic_cont1= 0;
  StatusVoIP.Generic_cont2= 0;
}//void ClearContErros(void)


/*
 *
 */
void PrintContErros(void)
{
//  memset(&StatusVoIP, 0, sizeof(Status_VoIP) );
  //Incrementa Erro Caso Tenha Perdido Registro incrementaErro.
  if ( StatusVoIP.ErrorComSocket || StatusVoIP.ErrorAlloc)
    app_syslog(LOG_CRIT, "%s->%s()<alloc>[%d]<socket>[%d]", __THIS_FILE__,
        StatusVoIP.ErrorAlloc,         //Erros de alocação de memória;
        StatusVoIP.ErrorComSocket      //Erros de cominicação com sockets;
                                  );
}//void ClearContErros(void)



//Quando Variavel for alocado pelo PJ não de deve dar free
void mem_free(void *pt_var)
{
 if (pt_var == NULL)
   return;

 free(pt_var);
 pt_var = NULL;
}

//Quando Variavel for alocado pelo PJ não de deve dar free
void unnit_close(void *pt_fd)
{
 if (pt_fd == NULL)
   return;

 close(pt_fd);
 pt_fd = NULL;
}

/* Aplicar o Padrao enm todas as Mensagens de LOG
 * Sintaxe da Mensagem de LOG
 * (funcao)    = Entre parenteses: Nome Da Funcao
 * {texto}    = Entre chaves: Texto qualquer
 * <variavel> = Entre Couchetes NOme da Variavel
 * [%x x] = Entre Couchetes Valor da Variavel

   icip_syslog( LOG_INFO ,"%s:(FUNC){Este eh umLOG de teste}<argument_1>[%d]:",THIS_FILE, arg1);
 */

int app_syslog(int syslogpri, char *format, ...)
{
  va_list args;

  if( (syslogpri > App_ctrl.Level_LOG_Prog) || (!App_ctrl.Is_LOG_Prog) )
   return ECANCELLED;

  va_start(args, format);
  vsyslog(syslogpri, format, args);
  _storage_func(args);
  va_end(args);


#if !PRINT_SYSLOG
  printf(format, args);
#endif

  return SUCCESS;
}


void printLastFunc( char* name_func)
{
  syslog(LOG_EMERG, "%s->%s()<!!!The Last FILE was>[%s()]", name_func, name_func,  NameArquivoGlobal);
  if ( *NameFuncaoGlobal==NULL )
     return;

  syslog(LOG_EMERG, "%s->%s()<!!!The Last FUNCTION Exec>[was %s()]", name_func, name_func, NameFuncaoGlobal);

}

#if 0


/*
 * icip_strncpy2
 *
 * Copia uma string para outra
 * Usado quando a string origem é uma ponteiro para char
 */
void app_strncpy(char *pchDst, char *pchSrc, int lenDst)
{
  int lenSrc;

  if (pchSrc == NULL)
    return;

  memset(pchDst ,0, lenDst);

  lenSrc = strlen(pchSrc);
  strncpy(pchDst, pchSrc, MIN_( lenSrc, lenDst));
  pchDst[POS_FIM_STRING_( lenSrc, lenDst)] = 0;
}



/*
 * icip_endstr
 *
 * poe caracter de fim de string(\0) na posição do caracter passado como parametro
 * Usado quando a string origem é uma ponteiro para char
 */
BOOL app_endstr(char *pt_str, char ch)
{
 char *fim_str;

 fim_str = strchr(pt_str, ch);            //Acha o caracter
 if (fim_str)                             //e se existir
   {
     *(fim_str) = 0;                          //poe Final de String
     return TRUE;
   }

 return FALSE;

}



/*
 * icip_endstr
 *
 * Verificar se string é valida
 */
bool isValid_str(char *pt_str)
{
 if( *pt_str==NULL)
     return FALSE;

 return ( isalnum(*pt_str) );

}

/*
 * icip_endstr
 *
 * Verificar se string é valida
 */
bool isValid_str_ascii(char *pt_str)
{
 if( *pt_str==NULL)
    return FALSE;

 return ( isascii(*pt_str) );

}


#endif //0
