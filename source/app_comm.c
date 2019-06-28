/********************************************************************
 *    Descrição: Tratamento das mensagens recebidas do PABX
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:16/12/2008
 *******************************************************************/



#include "app_comm.h"
#include "app_msg.h"
#include "convert_types_var.h"

//#include "entity_ip.h"
#include "main_ctrl.h"
#include <sys/resource.h>
#include <errno.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "general.h"

#define THIS_FILE   "app_comm.c"


//Declaração de Variáveis globais
BOOL COMMConnected = FALSE;


//BOOL FNetwork_OUT = FALSE;
//BOOL Is_Send_Range_To_Board_Comm = FALSE;
//static TAG_MESSAGE_VOIP  MsgRxSocket;



int                 client_sockfd=0;
int                 client_len;

#if SOCKET_LINUX
struct sockaddr_un  client_address;
#else
struct sockaddr_in  client_address;
#endif //SOCKET_LINUX


//Declaração de Váriaveis Externas
extern struct app App_ctrl;
extern BOOL LOOP_Main;
extern void (*const pMsgs[])(byte *pMessage);
//extern Status_VoIP  StatusVoIP;


/*==============================================================================================
 *                      Funções
 *==============================================================================================*/

/*
 * Isto ocorre quando nao existe nada programado
 * Depois de um Ero 0,0
 */
int Aguarda_comunicacao(void)
{
  while(!COMMConnected)
    {
      if (!LOOP_Main)
        {
          app_syslog( LOG_CRIT,"%s->%s(){Fallha na comunicacao com o remote!}\n", __THIS_FILE__);
          return ERROR;       //Termina Aplicação
        }

      sleep(2);
    }

  return SUCCESS;
}




/*
 * Função que implementa o tratamento da recepção de mensagens vindas do PABX via socket
 */
int COMM_socket(void *arg)
{
 TAG_MESSAGE_VOIP  MsgRxSocket;
 word aplicacao;

// pthread_detach(pthread_self());
// boost_priority(THREAD_PRIO_PABXCOM);

#if SHOW_PID_THERAD
  Pids_Threads.Pid_PABXCOMM_socket = getpid();
#endif
  //icip_syslog( (LOG_DEBUG  ),"************PPPPPABXCOMM_socket_THREAD_2>>>%d", getpid());
  app_syslog(LOG_INFO, "%s->%s(){Waiting connection to REMOTE}\n",__THIS_FILE__ ); //"Aguardando conexão com PABXCOMM"

  InitCOMM();		//Inicia socket de comunicação com processo ROUTER

  COMMConnected = TRUE;
  while (!App_ctrl.kill_application)//  while(LOOP)
    {
      //next_Frame:
      //sem_wait(&bin_semaforoSocket);
      memset (&MsgRxSocket, 0, SIZE_HEADER );          // limpa a estrutura (comum nas listas de discussão)
      if( RecebeDadosCOMM(&MsgRxSocket) != SUCCESS )
        {
          app_syslog(LOG_WARNING, "%s->%s(){Discart message!!!}\n", __THIS_FILE__ );
          continue;
        }

      ByteToWord(&aplicacao, &MsgRxSocket.AplicHigh);
      app_syslog(LOG_WARNING, "%s->%s()<app>[%d]<msg>[%d]<size>[%d]\n", __THIS_FILE__, aplicacao, MsgRxSocket.N_MsgLow, MsgRxSocket.PayloadSizeL );

#if 00 //TODO Verificar se melhor ptratar por Ramal
     if ( TrataMensagemRamalPABX( aplicacao,(TAG_MESSAGE_VOIP*)&MsgRxSocket ) )
       continue;//goto next_Frame;//  return; //Termina Por aqui
#endif //00
//     if ( aplicacao == ADDR_ICIP_BLF_CTRL )
       {
	     TrataMensagemRecebida( (TAG_MESSAGE_VOIP*)&MsgRxSocket );//Mensag
         continue;
       }

     app_syslog( LOG_WARNING ,"%s->%s(){VoIP recebeu mensagem com destino Invalido}<iDNS>[%d]<MSG>[%d]\n", __THIS_FILE__, aplicacao, MsgRxSocket.N_MsgLow);

   }////while(LOOP)

 app_syslog(LOG_CRIT, "%s->%s(){thread communication - FINISH!}\n",__THIS_FILE__ );
 COMMConnected = FALSE;

 //CloseCOMM();
 return SUCCESS;
 //exit(1);
}//void *thread_socket(void *arg)





/*
  * Função de inicialização de socket em modo cliente
  */
void InitCOMM(void)
{
  struct sockStatusFromServer serverStatus;
  struct sockCommandFromClient clientCommand;
  int result, nReadSock;
  //byte cntFalha = 0;

  app_syslog(LOG_WARNING, "%s->%s(){Start... }\n", __THIS_FILE__ );
  //Valida a Integridade do vetor de Mensagens recebidas no MAnager
  //ValidaTabelaMSG();
   // prepara o socket do cliente
#if SOCKET_LINUX
   client_sockfd               = socket(AF_UNIX, SOCK_STREAM, 0);    // cria o socket, local, com conexao, protocolo default
   memset (&client_address, 0, sizeof(struct sockaddr_un));          // limpa a estrutura (comum nas listas de discussão)
   client_address.sun_family   = AF_UNIX;                            // formata um endereço: define o tipo
   strcpy(client_address.sun_path, SERVER_PATH);                     // formata um endereço: define o nome do servidor
   client_len                  = sizeof(client_address);             // guarda o tamanho da estrutura
#else

   client_sockfd        = socket(PF_INET, SOCK_STREAM, 0);    // cria o socket, local, com conexao, protocolo default
   memset (&client_address, 0, sizeof(struct sockaddr_in));          // limpa a estrutura (comum nas listas de discussão)
   client_address.sin_family = AF_INET;                            // formata um endereço: define o tipo

   //Seta os IPS (A constante INADDR_ANY e todos os ips ou qualquer ip) htonl -> conversao
    //serverExtComm_addr.sin_addr.s_addr = INADDR_ANY;  //htonl(INADDR_ANY);
   client_address.sin_addr.s_addr = inet_addr(&SERVER_ADDR_IP);
   client_address.sin_port = htons(portaServidor);
#endif //SOCKET_LINUX
   do
     {//Fica tentando até conseguir se conectar a Aplicação PABXCOMM
       //result = connect(client_sockfd, (struct sockaddr *)&client_address, SUN_LEN(&client_address));
       result = connect(client_sockfd, (struct sockaddr *)&client_address, sizeof(struct sockaddr));
       if(result == SUCCESS)
         break;

       app_syslog(LOG_WARNING, "%s->%s(){Can't access device }<ERROR>[%d:%s]\n", __THIS_FILE__ , errno, strerror( errno ));
       sleep(3);//Dá um tempinho 3s para tentar novamente
       //if (cntFalha++ > 3);
       //  return;
       //  system("clear");

     } while (result != SUCCESS );



   clientCommand.Command = SOCKET_REGISTER;
   clientCommand.Client = BLF_CTRL;

   // registra o nome da Aplicaçao (APL_VOIP) no servidor
   EnviaDadosCOMM(&clientCommand, SIZE_sockCommandFromClient );

   //Receber status da conexão
   nReadSock = recv(client_sockfd,&serverStatus, SIZE_sockStatusFromServer, 0);
   if( nReadSock != SIZE_sockStatusFromServer )
       app_syslog(LOG_WARNING, "%s->%s(){SERVER NOT RECEIVED ALL DATA}\n", __THIS_FILE__ );

   app_syslog(LOG_INFO, "%s->%s(){SUCCESS TO CONNECTION IN SERVER}\n", __THIS_FILE__ );
   //printf(">>>>>>>   ICIP ->ServerStatus do GALO: %d - %d\n",serverStatus.Status, PABXCOMMConnected);
   //FsmDispatch( (Fsm *)&StateManager, NULL );  //Vai para estado Aguardando Configuração

}//void iniciaSocket(void)




void CloseCOMM(void)
{
  TAG_SOCKET_UNREGISTER_CLIENT   unregisterClient;
  int nReadSock;
  struct sockStatusFromServer serverStatus;

 //-------- -Manda Mensagem Para PABXCOMM Fechar a Conexão
  unregisterClient.ClientSocketId = ICIP_VOIP;
  //FormataFrameSEND(ADDR_ICIP_PBXCOMM, TYPE_CONFIGURACAO, VOX_FRM_SOCKET_UNREGISTER_CLIENT, VOID_POSITION, VOID_SELECT, &unregisterClient, SIZE_TAG_SOCKET_UNREGISTER_CLIENT );

  //!!aguarda o Galo responter com a mesma mensagem!!
  nReadSock = recv(client_sockfd,&serverStatus, SIZE_sockStatusFromServer, 0);
  app_syslog(LOG_CRIT, "%s->%s()<nReadSock>[%d]\n", __THIS_FILE__ , nReadSock);
  shutdown (client_sockfd, SHUT_RDWR);

  //Termina Thread
  //pthread_join(App_ctrl.router_thread);
  //pthread_exit(App_icip.router_thread);
  App_ctrl.router_thread = 0;

}


/*
 * Funcao que verifica se deu erro no retorno
 * da leitura/escrita do socket
 * Se ok retorna SUCCESS
 */
int isErrorIn_Socket(int check_return, int line )
{

  if (check_return > APP_MAX_TAM_PAYLOAD)
    {
      app_syslog(LOG_CRIT, "%s->%s()Message So long[%d]", __THIS_FILE__ ,check_return );
      return ENOMSG;
    }

  if (check_return > 0)
    return SUCCESS;

  app_syslog(LOG_CRIT, "%s->%s(){SERVER close connection} in line [%d]\n", __THIS_FILE__, line );
  //Se alterar o Numero do file description, nao será possivel fechar ele.
  //client_sockfd=ERROR;

  app_syslog( LOG_CRIT,"%s->%s()<ALERT>[CRITICAL ERROR]<cError>[%d]\n", __THIS_FILE__, client_sockfd );
  CloseApp_ctrl();

  /*  StatusVoIP.ErrorComSocket++;
  if( !StatusVoIP.ErrorComSocket)
    {
     app_syslog( LOG_CRIT,"%s->%s()<ALERT>[CRITICAL ERROR]<cError>[%d]", __THIS_FILE__, (StatusVoIP.ErrorComSocket+255) );
     CloseApp_blf_ctrl();
    }*/

  return ENOMSG;
}



/*
 * Função que recebe dados via socket
 */
int RecebeDadosCOMM(byte *data)
{
  TAG_HEADER_SOCKET *HeaderMsgRx;
  int nRead;
  int bytesToRead, bytesReceived;

  nRead = recv(client_sockfd, data, SIZE_HEADER_SOCKET, 0);    // Primeiro Só Pega o Header
  if (isErrorIn_Socket(nRead, __LINE__))
     return ENOMSG;

  if( nRead != SIZE_HEADER_SOCKET )
    {
      app_syslog(LOG_WARNING, "%s->%s(){FAIL on RECEIVED HEAD}[%d]\n", __THIS_FILE__, nRead );
      nRead = recv(client_sockfd, data, SIZE_BODY_SOCKET, 0);
      isErrorIn_Socket(nRead, __LINE__);
      return EMSGSIZE;
    }

  HeaderMsgRx = (TAG_HEADER_SOCKET*)data;  //Copia do Header da Mensagem
  bytesToRead = (  (HeaderMsgRx->PayloadSizeH << 8) + HeaderMsgRx->PayloadSizeL  );
  data += SIZE_HEADER_SOCKET; //Desloca o ponterio para receber o resto (Corpo)da Mensagem
  bytesReceived = 0;
  while (bytesReceived < bytesToRead)
    {
      nRead = recv(client_sockfd, (data+bytesReceived), (bytesToRead-bytesReceived), 0);
      if (isErrorIn_Socket(nRead, __LINE__))
        return ENOMSG;

      if (nRead != bytesToRead)
        {
          app_syslog(LOG_CRIT, "%s->%s(){DIDN'T READ everthing}<nRead>[%d]<bytesToRead>[%d]\n", __THIS_FILE__ ,  nRead , bytesToRead );//"O Servidor Fechou a Conexao\n";
          sleep(1);
          return EMSGSIZE;
        }

      bytesReceived += nRead;
    }//while (bytesReceived < bytesToRead)


#if SHOW_FRM_FROM_PABXCOMM
  ConvHexToStr(&msgString, data, bytesReceived);
  icip_syslog(LOG_DEBUG, "%s->%s(){%s}", __THIS_FILE__, msgString ) ;
#endif

 return SUCCESS;
}//void RecebeDadosDoSocket(void)


/*
 * Função que envia dados ao socket
 */
void EnviaDadosCOMM( byte *umaMsg, word sizeFrame)
{
  int nWrite;
  int totalSend = 0;
  int restaSend = 0 ;

  if( sizeFrame == 0 )
    return;


  if( client_sockfd == ERROR )
    {
      app_syslog(LOG_CRIT, "%s->%s(0){Almost already connection}\n", __THIS_FILE__  );
      return;
    }

  //Dados Enviados Para PABXCOMM
  restaSend = sizeFrame;
  while (totalSend < sizeFrame)
    {
      nWrite = send(client_sockfd, umaMsg+totalSend, restaSend, 0 );    // encaminha ao destino    n = send (socket_descriptor, buffer+total, bytes_resta, 0);
      if (isErrorIn_Socket(nWrite, __LINE__))
        {
          app_syslog(LOG_CRIT, "%s->%s(1){Almost already connection}[%d]\n", __THIS_FILE__, nWrite  );
          return;
        }


      totalSend += nWrite;
      restaSend -= nWrite;
    }

#if SHOW_FRM_TO_PABXCOMM
  ConvHexToStr(&msgString, umaMsg, sizeFrame);
  icip_syslog(LOG_DEBUG, "%s->%s(){%s}", __THIS_FILE__, msgString ) ;
#endif
}//void EnviaDadosAoSocket(char *umaMsg)


/***********************************************************************
* Função que trata a recepção e o
* roteamento das mensagens vindas do PABX
* Entrada: Ponteiro para BUFFER de mensagem recebida
* Saída: Nenhuma
***********************************************************************/
/*
 * Formato da mensagem enviada pela aplicação Roteadora
 *
 * +----------------+
 * -      TIPO      -
 * +----------------+
 * - ID_DESTINO (L) -
 * +----------------+
 * - ID_DESTINO (H) -
 * +----------------+
 * -    N_MSG (L)   -
 * +----------------+
 * -    N_MSG (H)   -
 * +----------------+
 * -     DADOS      -
 * +----------------+
 *
 * TIPO - Tipo de mensagem recebida - Mensagem de configuração, sinalização, dados e alarme
 *
 * ID_DESTINO - Identificador do destino da mensagem (antigo CANAL do protocolo)
 *
 * N_MSG - Número da mensagem
 *
 * Dados - Informação contida na mensagem
*/


void TrataMensagemRecebida(TAG_MESSAGE_VOIP *ptrMsg)
{
  byte num_Msg = FALHA_NA_PESQUISA;
  word nMsg = 0;

  ByteToWordInv( &nMsg, (byte*)&ptrMsg->N_MsgLow );   //printf("\n bYTE WORD %x ValorH %x - ValorL %x\n",nMsg, ptrMsg->N_MsgLow, ptrMsg->N_MsgHigh );
  num_Msg   = PesquisaIndiceComando( nMsg );
  // Se     Existe o Tipo de Mensagem         E        Achou Mensagem            E         é uma Mensagem Válida
  if ( (ptrMsg->Tipo < ULTIMO_TIPO_DE_MENSAGEM)  &&  (num_Msg != FALHA_NA_PESQUISA)  &&  (ptrMsg->Tipo != TYPE_INVALID)  )
    {
      (*pMsgs[num_Msg])( ptrMsg->Dados );//executa a função Passada como ponteiro
      return;
    }

  app_syslog(LOG_WARNING, "%s->%s(){INVALID MESSAGE! }<TIPO>[%d]\n", __THIS_FILE__, nMsg );// "ICIP RECEBEU mensagem invalida>>Tipo: ");
  //FrmRespOKNOKSEND (TRUE);


}//void TrataMensagemRecebida(unsigned char *apontaBufferRecebido)

#if 0
/***********************************************************************
* Função formata Frame para ser enviada ao PABXCOMM
* Entrada: Aplicacao Destino, Tipo da mensagem, numero da Mensagem(N_MSG),
*                  ponteiro para a strutura d Mensagem(N_MSG), tamanho da N_MSG)
***********************************************************************/
void FormataFrameSEND(word aplicDest,byte tipo ,word nMsg, byte pos, byte sel, byte *ptrStruct, word sizeStruct)
{
  TAG_MESSAGE_VOIP ptrFrame;
  int count =0;
  extern BOOL IN_TEST;

  #if EXECUTE_TDD
  if (IN_TEST)
    return;
  #endif
  //- - - - - - - -Prepara Mensagem para enviar para o PABXCOMM- - - - - - - -
  //........................Cabeçalho do FRAME....................................
   WordToByte(&ptrFrame.AplicHigh, aplicDest );     // identificação da aplicação destino da mensagem;
   WordToByte(&ptrFrame.PayloadSizeH,( SIZE_HEADER_MSG + sizeStruct)  ); // número de bytes no data + Header da mensagem;

  //........................Cabeçalho do PayLoad....................................
   ptrFrame.Tipo      = tipo;                                            // TIPO - Tipo de mensagem
   WordToByteInv(&ptrFrame.N_MsgLow, nMsg);              // N_MSG - Número da mensagem -

   ptrFrame.Position = pos;
   ptrFrame.Select   = sel;
   memcpy( &ptrFrame.Dados, ptrStruct, sizeStruct );

   EnviaDadosPABXCOMM((byte*)&ptrFrame, (SIZE_HEADER_SOCKET + SIZE_HEADER_MSG + sizeStruct) );

}

#endif //0

