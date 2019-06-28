/********************************************************************
 *    Descrição: Tratamento das mensagens recebidas do PABX
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:16/12/2008
 *******************************************************************/

#include "app_msg.h"
#include "app_comm.h"
#include <sys/resource.h>
#include <errno.h>
#include <sys/stat.h>
#include "general.h"
#include <lists.h>


#define THIS_FILE   "app_msg.c"


//Declaração de Váriaveis Externas
//extern struct app App_icip;

/*==============================================================================================
 * Vetor com as Mensagens recebidas Do Pabx
 *     - Deve ser ordenado em ordem crescente, Para algoritmo de busca binária,
 *     - Sempre que acrescentar uma mensagem tem que atualizar o #define NUMERO_MENSAGENS_PROTOCOLO
 *==============================================================================================*/
static word ListaDeMensagens[] =
  {
      VOX_FRM_SUBSCRIBE,
      VOX_FRM_NOTIFY_BLF
  };

/*==============================================================================================
 * Vetor de Ponteiros de funções de Mensagens recebidas Do Board_Com
 *==============================================================================================*/
void (*const pMsgs[])(byte *pMessage) =
  {
          FrmVoIPSubscriber,
          FrmPBXNotify
  };



/*==============================================================================================
 *                      Funções
 *==============================================================================================*/
/*
/***********************************************************************
* Função retorna índice da mensagem em tabela de ponteiro para funções
* de tratamento das mensagens recebidas do PABX
* Entrada: Código da mensagem enviada pelo PABX
* Saída: Índice correspondente ao código do comando na tabela de
* ponteiros de funções
***********************************************************************/
byte PesquisaIndiceComando(word comando)
{
  byte posicaoInferior = 0;
  byte posicaoSuperior = ARRAY_SIZE(ListaDeMensagens);//NUMERO_MENSAGENS_PROTOCOLO;
  byte posicaoMeio;

  //BUSCA SIMPLES
  while (posicaoInferior <= posicaoSuperior)
    {
      if (comando == ListaDeMensagens[posicaoInferior])
        return posicaoInferior;

      posicaoInferior++ ;
    }//while

  return FALHA_NA_PESQUISA;                                     //Não encontrou comando

  //TODO Se aumentar o numero de Mensagem Fazer busca binaria
  /*BUSCA BINARIA
  while (posicaoInferior <= posicaoSuperior)
    {
      posicaoMeio=(posicaoInferior+posicaoSuperior)/2;
      if (comando == ListaDeMensagens[posicaoMeio])
        return posicaoMeio;

      if (comando < ListaDeMensagens[posicaoMeio] )
        posicaoSuperior = posicaoMeio-1;
      else
        posicaoInferior = posicaoMeio+1;
    }//while

  return FALHA_NA_PESQUISA;                                     //Não encontrou comando
  */

}//byte PesquisaIndiceComando(byte comando)


/*
 *----------------------ICIP RECEBE------------------------------------
 */

/*
 * Mensagem com Solicitacao dos Status dos BLF
 * Vem do App_voip
 *
 */
void FrmVoIPSubscriber (byte *ptrDataMessage)
{
#if 0
  TAG_SUBSCRIBE_BLF msgTx;
  memcpy(&msgTx, ptrDataMessage, SIZE_TAG_SUBSCRIBE_BLF );

  app_syslog( LOG_DEBUG,"%s->%s()<type>[%d]<key>[%s]<dns>[%d]", __THIS_FILE__ ,
      msgTx.RequestType, msgTx.BLF_TargetSubscriber, msgTx.IndDNSDriverIP  );

  if ( (msgTx.IndDNSDriverIP < APP_MIN_INDEX ) || ( msgTx.IndDNSDriverIP > APP_MAX_INDEX  ) )
    {
      app_syslog( LOG_WARNING,"%s->%s(){index[%d] Invalid!}", __THIS_FILE__, msgTx.IndDNSDriverIP );
      return;
    }

  //Se nao tiver na Lista ainda pede o Status para o PABX
  if( (isNewBLFInList(&msgTx.BLF_TargetSubscriber) ) && (msgTx.BLF_SubscriptionExpirationTime) )
      FrmSubscribeSEND_TOPABX (&msgTx);


  //TECLA MOnitorada - DNS que quer saber - ESTADO
  IncludeNewDNSInKey(&msgTx);
  FrmNotifySEND_VOIP ( &msgTx);
#endif //0

}//void FrmVoIPSubscriberGeneral (byte *ptrStructMsg)

/*
 * Mensagem com os Estados das Portas
 * Recebida do PABX e tratada aqui
 * e Enviada aos Terminais que tem essa tecla Programada
 */
void FrmPBXNotify (byte *ptrDataMessage)
{
#if 0
	TAG_NOTIFY_BLF msgTx;
  memcpy(&msgTx, ptrDataMessage, SIZE_TAG_NOTIFY_BLF );

  app_syslog( LOG_DEBUG,"%s->%s()<status>[%d]<key>[%s]<direction>[%d]", __THIS_FILE__ ,
      msgTx.BLF_status, msgTx.BLF_TargetSubscriber, msgTx.BLF_DirectionCall  );

  ChangeStatusInKey(msgTx.BLF_TargetSubscriber, msgTx.BLF_status, msgTx.BLF_DirectionCall);

#endif //0
}//void FrmVoIPSubscriberGeneral (byte *ptrStructMsg)


//==========================================================================================
//-----------------------MENSAGEM ENVIADAS PARA APP_VOIP -----------------------------------
/*
 * Envia Mensagem de NOtify com estado da Tecla para APP_VOIP
 * Passa o DNS do Ramal e a tecla
 */
void  FrmNotifySEND ( word aplicDest, TKeys *aBlfKey)
{
#if 0
	TAG_NOTIFY_BLF msg_notify_blf;

  memset(&msg_notify_blf,0, SIZE_TAG_NOTIFY_BLF);

  //MONTA ESTADO
  msg_notify_blf.BLF_DirectionCall=aBlfKey->BLF_DirectionCall;
  msg_notify_blf.BLF_status=aBlfKey->status_blf;

  memcpy(&msg_notify_blf.BLF_TargetSubscriber, aBlfKey->key_name, APP_MAX_NOME_PORTA);
  app_syslog( LOG_DEBUG,"%s->%s()<status>[%d]<dir>[%d]<key>[%s]<dest>[%d]", __THIS_FILE__,
      msg_notify_blf.BLF_status,
      msg_notify_blf.BLF_DirectionCall,
      msg_notify_blf.BLF_TargetSubscriber,
      aplicDest
                                      );
  FormataFrameSEND(aplicDest, TYPE_SINALIZACAO, VOX_FRM_NOTIFY_BLF, VOID_POSITION, VOID_SELECT, &msg_notify_blf, SIZE_TAG_NOTIFY_BLF );
#endif// 0
}


/*
 * Procura a tecla na Lista e envia para o Ramal
 */
void  FrmNotifySEND_VOIP ( const TAG_SUBSCRIBE_BLF *ptrDataMessage)
{
#if 0
	TKeys *aBlfKey ;

/*  app_syslog( LOG_DEBUG,"%s->%s()<req>[%d]<Key>[%s]<exp>[%d]", __THIS_FILE__,
      ptrDataMessage->RequestType,
      ptrDataMessage->BLF_TargetSubscriber,
      ptrDataMessage->BLF_SubscriptionExpirationTime
                                      );
*/
  if (!ptrDataMessage->BLF_SubscriptionExpirationTime)
    return;

  aBlfKey = FindKey(&ptrDataMessage->BLF_TargetSubscriber);
  if ( aBlfKey == NULL  )
    {
      app_syslog( LOG_WARNING,"%s->%s(){KEY Invalid!}", __THIS_FILE__ );
      return;
    }

  FrmNotifySEND ( ptrDataMessage->IndDNSDriverIP, aBlfKey);
#endif 0
}//FrmSubscribeSEND




//==========================================================================================
//-----------------------MENSAGEM ENVIADAS PARA O PABX -----------------------------------
void  FrmSubscribeSEND_TOPABX ( TAG_SUBSCRIBE_BLF *ptsubscriber)
{
#if 0
	TAG_SUBSCRIBE msgTx;

  //app_syslog (LOG_DEBUG, "%s->%s()<expires>[%d]", __THIS_FILE__, ptsubscriber->BLF_SubscriptionExpirationTime);
  ptsubscriber->BLF_SubscriptionExpirationTime = EXPIRATION_DEFAULT;

  memset(&msgTx, 0, SIZE_TAG_SUBSCRIBE);        //Limpa Estrutura
  msgTx.RequestType = ICIP_SBSCRB_BLF;
  app_strncpy (&msgTx.BLF_TargetSubscriber, ptsubscriber->BLF_TargetSubscriber, APP_MAX_NOME_PORTA);
  msgTx.BLF_SubscriptionExpirationTime=ptsubscriber->BLF_SubscriptionExpirationTime;
  app_syslog (LOG_DEBUG, "%s->%s(){SUBS_BLF}<blf_ramal>[%s]<expires>[%d]", __THIS_FILE__, msgTx.BLF_TargetSubscriber, msgTx.BLF_SubscriptionExpirationTime);
  FormataFrameSEND(ptsubscriber->IndDNSDriverIP, TYPE_SINALIZACAO, VOX_FRM_SUBSCRIBE, ptsubscriber->Position, ptsubscriber->Select, &msgTx, SIZE_TAG_SUBSCRIBE );
#endif //0
}

/*
 * Envia para o PABX
 * Refresh das teclas ainda ativas
 */
void FrmSubscribeRefresh_TOPABX(TKeys *exist_key)
{
#if 0

	TAG_SUBSCRIBE_BLF msgTx;

  TypePortDNS *firstport;
  firstport =  exist_key->ListIndDNS.next;

  memset(&msgTx, 0, SIZE_TAG_SUBSCRIBE);        //Limpa Estrutura
  app_strncpy (&msgTx.BLF_TargetSubscriber, exist_key->key_name, APP_MAX_NOME_PORTA);
  //Pega sempre o Primeiro Porta da Lista

  msgTx.IndDNSDriverIP = ((TypePortDNS *)exist_key->ListIndDNS.next)->IndiceDNS;
  msgTx.Position = ((TypePortDNS *)exist_key->ListIndDNS.next)->position;
  msgTx.Select   = ((TypePortDNS *)exist_key->ListIndDNS.next)->selelect;

  msgTx.RequestType = ICIP_SBSCRB_BLF;
  msgTx.BLF_SubscriptionExpirationTime=EXPIRATION_DEFAULT;

  app_syslog( LOG_DEBUG,"%s->%s()<type>[%d]<key>[%s]<dns>[%d]", __THIS_FILE__ ,
      msgTx.RequestType, msgTx.BLF_TargetSubscriber, msgTx.IndDNSDriverIP  );

  FrmSubscribeSEND_TOPABX (&msgTx);
#endif //0
}

