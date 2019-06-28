/******************************************************************************************************
 * Copyright(c) 2019 Marco Vargas, All Rights Reserved.
 * This software is proprietary and confidential.
 *
 * File: exemple_ctrl.c
 * Author: Marco Antonio Vargas <marco@marcovargas.com.br>
 * Date: 27/06/2019
 * Description:
 *
 * Documentation:
 * 			https://github.com/marcovargas74/exemple_ctrl/docs/readme.txt
 * Revision:
 * 			19.06.27
 ******************************************************************************************************/

//#include "defin.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <syslog.h>
#include "def_types.h"
//#include "blf_ctrl.h"
//#include "app_comm.h"

#define THIS_FILE   "example_ctrl.c"

struct  app App_ctrl;
BOOL LOOP_Main = TRUE;
//Status_PID_VoIP Pids_Threads;        //NUmro dos Pids das thread usados no debbuger


/*
 * main() - Entrada do programa - Processo VoIP
 */
int main(void)
{
//  extern BOOL PABXCOMMConnected;
  int status;
//  struct sigaction sig_app;
  int count_LOOP = 0;
//  byte count_30s  = 0;  //Contador de 30 segundos
//  uLong count_1MIN = 0; //Contador de minuto
  int ret_thread;


	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */


 // boost_priority(THREAD_PRIO_MAIN);
  StartLogger();
//  TrataComandosVindoDoLinux(&sig_app);


  // Seta opções default da Aplicação
  init_parm_app();
  app_syslog( LOG_NOTICE, "%s->%s(){******APPLICATION BLF-STARTING*******}<version>[%s]",__THIS_FILE__, App_ctrl.version_software );
#if 0

  Pids_Threads.Pid_Main = getpid();


  #if EXECUTE_TDD
  ExecutaTestesIniciais();
  #endif


  //=========================== INICIALIZAÇÃO DE COMUNICAÇÂO COM PABX ==================
  app_syslog( LOG_NOTICE, "%s->%s(){ Carrega a thread de comunicacao com oPABXCOMM_socket",__THIS_FILE__ );
  ret_thread = pthread_create(&App_ctrl.router_thread, NULL, &PABXCOMM_socket, NULL);
  if (ret_thread)
    {
      app_syslog(LOG_ERR,"%s->%s(){FATAL ERROR->pthread_create}<error>[%d]", __THIS_FILE__, ret_thread );
      return ERROR;       //Termina Aplicação
    }

   if( Aguarda_comunicacao_com_PABX() != SUCCESS)
     return ERROR;       //Termina Aplicação

#if ICIP_MOD_DEBUG && EXECUTE_TDD
   //Executa TESTE
    ExecutaTestesFinais();
#endif
    app_syslog(LOG_DEBUG,"%s->%s(){LOOP_MAIN!!!}", __THIS_FILE__ );
    while (LOOP_Main)
       {
          //Executa Timer
          Executa_Timer_Control(&count_LOOP, &count_30s, &count_1MIN);
       }
     return SUCCESS;

#endif 0

 	return EXIT_SUCCESS;

}




/*
 * Função de Inicialização de diversas Variaveis
 * Usadas na Aplicação
 */
void init_parm_app(void)
{
 ClearContErros();

// list_init(&App_ctrl.Tab_BLF_key);
// App_ctrl.sizeKey=0;

// App_ctrl.thread_count   = 1;
// App_ctrl.kill_application = KILL_NOT;
// snprintf(&App_ctrl.version_software, APP_MAX_NOME_VERSAO, "%s.%s.%s", _VER_SOFT, _REV_SOFT, _REL_SOFT);
// app_syslog( LOG_INFO,"%s->%s()<status>[OK]", __THIS_FILE__ );

}//void initAplic(void)

#if 0
/*
 *
 */
void Executa_Timer_Control(int *ptr_count_LOOP, byte *ptr_count_30s, uLong *ptr_count_1MIN)
{
//  *ptr_count_LOOP += 1;
//  app_syslog( LOG_DEBUG,"%s->%s()###===LOOP 30s.[%d]min", __THIS_FILE__, *ptr_count_1MIN );
//  if( *ptr_count_LOOP % TIME_EXEC_30SEC  )
//    return;
  sleep(30);
  //a cada 30segundos
  //app_syslog( LOG_DEBUG,"%s->%s()###===LOOP 30s..[%d]min", __THIS_FILE__, *ptr_count_1MIN );

  *ptr_count_30s ^= 1; //Alterana 0 e 1
  if(*ptr_count_30s)//Se 30 segundos igual a 1
    return;

  // 1 minuto
  *ptr_count_1MIN += 1; //Inclrementa 1 Minuto
  app_syslog( LOG_DEBUG,"%s->%s()###===LOOP 1min..[%d]min", __THIS_FILE__, *ptr_count_1MIN );
  /*if( !(*ptr_count_1MIN % TIME_EXEC_3MIN) )// Se o Resto da divisao der zero. É o numero pretendido.
    {
      app_syslog( LOG_DEBUG,"%s->%s()###===LOOP 3 minuto..[%d]min", __THIS_FILE__, *ptr_count_1MIN );
      PrintContErros();
    }*/

  if( !(*ptr_count_1MIN % TIME_EXEC_10MIN) )
    {
      app_syslog( LOG_DEBUG,"%s->%s()###===LOOP 10 minuto..[%d]min", __THIS_FILE__, *ptr_count_1MIN );
      Resfresh_status();
      PrintContErros();
    }

}//Excuta_timer_control


void CloseApp_blf_ctrl(void)
{
 #if MOD_DEBUG
 app_syslog(LOG_ALERT, "%s->%s() Reboot line %d\n", __THIS_FILE__, __LINE__);
 //return;
 #endif

 app_syslog(LOG_ALERT, "%s->%s()(PID=%d){++++++++++++ APPLICATION BLF - FINISH ++++++++++++}",__THIS_FILE__, (int)getpid () );
 sleep(2);
 ClosePABXCOMM();
 LoggerClose();
 LOOP_Main = FALSE;
 exit(0);
}


void app_trata_seg_fault(int signum)
{
  syslog( LOG_EMERG,"%s->%s()<EMERGENCY>[!!!SEGMENT FAULT!!! Failure BLF_CTRL App]", THIS_FILE, __func__);
  printLastFunc(__func__);
  //generate core dump
  sleep(1);
  CloseApp_blf_ctrl();
  //signal(signum, SIG_DFL);
  //kill(getpid(), signum);
}


void app_trata_cond_urgent_socket(int signum)
{
 // fail_msg_syslog();
  app_syslog( LOG_EMERG,"!!!URGENT CONMDICION SOCKET!!! Failure BLF_CTRL App in %s() line %d\n", __func__, __LINE__);
  sleep(1);
  //generate core dump
  //signal(signum, SIG_DFL);
  //kill(getpid(), signum);
}

/*
 * QUIT
 */
void app_trata_quit(int signum)
{
  int tmp_pid = getpid();
  syslog( LOG_EMERG,"%s->%s()[!!!QUIT!!! BLF_CTRL thread:%d = %d]", __THIS_FILE__ , tmp_pid , Pids_Threads.Pid_Main);
  //generate core dump
  if (Pids_Threads.Pid_Main == tmp_pid )
      CloseApp_blf_ctrl();

  sleep(1);
  exit(0);
  //signal(signum, SIG_DFL);
  //kill(getpid(), signum);
}


/*
 * Quando receber o sinal SIGUSR1 executa a ação
 * determinada pela estrutura sig
 */
void TrataComandosVindoDoLinux(struct sigaction* sig_app)
{
//  void icip_trata_seg_fault(int signum);

  /* Vincula o handler com uma função */
  sig_app->sa_handler = CloseApp_blf_ctrl;
  //Comando veio do LInux ou outra aplicacao
  //User-defined signal 1 (POSIX).
  sigaction(SIGUSR1, &sig_app, 0);

  //Caso A aplicacao ocorra algum segment FAULT Causado por um NULL
  signal(SIGSEGV, app_trata_seg_fault);

  //Caso A aplicacao ocorra algum Urgent condition on socket
  signal(SIGURG, app_trata_cond_urgent_socket);

  //Caso A aplicacao receba um QUIT
  signal(SIGQUIT, app_trata_quit);

}

#endif //0
