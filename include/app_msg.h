/********************************************************************
 *    Descrição: Tratamento das mensagens recebidas do PABXCOMM
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:16/12/2008
 *******************************************************************/

#ifndef APP_MSG_H_
#define APP_MSG_H_

#include "def_types.h"
#include "def_enum.h"
#include <sys/un.h>

//_________________ DEFINES __________________________________

//tamanho (bytes) do header de uma mensagem
#define     APP_MAX_TAM_PAYLOAD	   1500																										//
#define     APP_MAX_TAM_MSG		   ICIP_HEADER_MSG_SIZE + ICIP_HEADER_PAY_SIZE + ICIP_MAX_TAM_PAYLOAD// tamanho (bytes) da maior mensagem

//#define     NUMERO_MENSAGENS_PROTOCOLO    15+1
#define     NUMERO_DE_TIPOS_DE_MENSAGEM   4            //Mensagem de configuração, Sinalização, Dados e Alarme
#define     NUMERO_MENSAGENS_POR_TIPO     8            //Número máximo de mensagens de um tipo

#define     FALHA_NA_PESQUISA             0xFF
#define     CAMPO_VAGO                    0xFFF0
#define     CAMPO_INVALIDO                0xFFFF

#define     VOID_SELECT                 0xFE
#define     VOID_POSITION               0xFE

/* Formato da mensagem enviada pela aplicação Roteadora
 *
 * +----------------+
 * -  APLICACAO (H) -
 * +----------------+
 * -  APLICACAO (L) -
 * +----------------+
 * -  PAYLOADSIZE(H)-
 * +----------------+
 * -  PAYLOADSIZE(L)-
 * +----------------+
 * -      TIPO      -
 * +----------------+
 * -    N_MSG (L)   -
 * +----------------+
 * -    N_MSG (H)   -
 * +----------------+
 * -     SELECT     -
 * +----------------+
 * -    POSITION    -
 * +----------------+
 * -     DADOS      -
 * +----------------+
 *
 * APLICACAO - identificação da aplicação destino da mensagem;
 *
 * PAYLOADSIZE - número de bytes contidos no Payload da mensagem
 *
 * TIPO - Tipo de mensagem recebida - Mensagem de configuração, sinalização, dados e alarme
 *
 * ID_DESTINO - Identificador do destino da mensagem (antigo CANAL do protocolo)
 *
 * N_MSG - Número da mensagem
 *
 * Dados - Informação contida na mensagem
 */

// Mensagem do Serviço Voip.
typedef struct
{
  byte AplicHigh;     //parte mais significativa, da identificação da aplicação destino da mensagem;
  byte AplicLowh;     //parte menos significativa, da identificação da aplicação destino da mensagem;
  byte PayloadSizeH;  //parte mais significativa,  do número de bytes contidos no Payload da mensagem;
  byte PayloadSizeL;  //parte mais significativa,  do número de bytes contidos no Payload da mensagem;

  byte Tipo; 		  // TIPO - Tipo de mensagem recebida - Mensagem de configuração, sinalização, dados e alarme
  byte N_MsgLow ;     // N_MSG - Número da mensagem - Parte Menos Significativa
  byte N_MsgHigh;     // N_MSG - Número da mensagem - Parte Mais Significativa
  byte Select;        // parte integrante do endereço de hardware dos ramais e juntores IP, no PABX.
  byte Position;      // parte integrante do endereço de hardware dos ramais e juntores IP, no PABX.

  byte Dados[APP_MAX_TAM_PAYLOAD];   //Dados - Informação contida na mensagem

} TAG_MESSAGE_VOIP;


//Tag do Header usado para receber dados do PABXCOMM VIA socket
typedef struct
{
  byte AplicHigh;     //parte mais significativa, da identificação da aplicação destino da mensagem;
  byte AplicLowh;     //parte menos significativa, da identificação da aplicação destino da mensagem;
  byte PayloadSizeH;  //parte mais significativa,  do número de bytes contidos no Payload da mensagem;
  byte PayloadSizeL;  //parte mais significativa,  do número de bytes contidos no Payload da mensagem;
} TAG_HEADER_SOCKET;
#define     SIZE_HEADER_SOCKET    4                                                                                                                                                                                                                            // tamanho (bytes) do header de uma mensagem
#define     SIZE_HEADER_MSG       5                                                                                                                                                                                                                            // tamanho (bytes) do header de uma mensagem

#define SIZE_HEADER  (  SIZE_HEADER_SOCKET + SIZE_HEADER_MSG )
#define SIZE_TAG_MESSAGE_VOIP  (  SIZE_HEADER + ICIP_MAX_TAM_PAYLOAD )




//Tag do corpo da Mensagem usado para receber dados do PABXCOMM VIA socket
typedef struct
{
  byte Tipo;              // TIPO - Tipo de mensagem recebida - Mensagem de configuração, sinalização, dados e alarme
  byte N_MsgLow ;     // N_MSG - Número da mensagem - Parte Menos Significativa
  byte N_MsgHigh;     // N_MSG - Número da mensagem - Parte Mais Significativa
  byte Select;        // parte integrante do endereço de hardware dos ramais e juntores IP, no PABX.
  byte Position;      // parte integrante do endereço de hardware dos ramais e juntores IP, no PABX.

  byte Dados[APP_MAX_TAM_PAYLOAD];   //Dados - Informação contida na mensagem
} TAG_BODY_SOCKET;

#define     SIZE_BODY_SOCKET     (APP_MAX_TAM_PAYLOAD + SIZE_HEADER_MSG)                                                                                                                                                                                                                            // tamanho (bytes) do header de uma mensagem


//ESTRUTURAS SOCKET
struct sockCommandFromClient
{
  byte   Command;
  byte   Client;
  byte   :8;
};
#define SIZE_sockCommandFromClient  3

struct sockStatusFromServer
{
  byte   Status;
  byte   :8;
};

#define SIZE_sockStatusFromServer  2

typedef enum
{
      SOCKET_UNREGISTER,
      SOCKET_REGISTER,
      SOCKET_STATUS,
} MsgClientControl;

typedef enum
{
      REGISTER_FAILED,
      REGISTER_SUCCESSSFULL,
} MsgServerControl;




//ESTRUTURAS TESTE SOCKET
typedef enum
{
  ICIP_MANAGER,
  CABLE_MANAGER,
  ICIP_VOIP,
  ICIP_ICTI,
  BOARD_CMD,
  ICIP_FAX_VIRTUAL,
  BLF_CTRL,
} IcipClient;





// Aplicações de sistema
#define    ADDR_SERVICOS   0			        // endereço dos serviços 0001 a 0999
#define    ADDR_ICIP       20000
//#define	  ADDR_CTI_MONITOR		(ADDR_SERVICOS+135)   // interface programação
#define    ADDR_IP_BOARD_MANAGER   (ADDR_SERVICOS+137)  // gerencia da placa ICIP (lado PABX)

//#define    ADDR_ICIP_MANAGER  (ADDR_ICIP+0)             // gerencia VoIP (lado ICIP)
#define    ADDR_ICIP_VOIP_MANAGER  (ADDR_ICIP+1)        // Gerencia Voip
#define    ADDR_ICIP_WEB           (ADDR_ICIP+2)           // servidor web embarcado
#define    ADDR_ICIP_EMAIL         (ADDR_ICIP+3)           // cliente de email embarcado
#define    ADDR_ICIP_SNMP          (ADDR_ICIP+4)           // agente SNMP embarcado
#define    ADDR_ICIP_CABLE_MANAGER (ADDR_ICIP+5)           // agente SNMP embarcado
#define    ADDR_ICIP_PBXCOMM       (ADDR_ICIP+6)           // agente SNMP embarcado
#define    ADDR_ICIP_IPMOM         (ADDR_ICIP+7)
#define    ADDR_ICIP_FAX_MANAGER   (ADDR_ICIP+8)
#define    ADDR_ICIP_BLF_CTRL      (ADDR_ICIP+9)



// Tipo: tipos de mensagem do protocolo entre PABX e placa IP (1 byte)
#define TYPE_CONFIGURACAO		0
#define TYPE_SINALIZACAO		1
#define TYPE_DADOS			2
#define TYPE_ALARME			3

// Tipo: tipos de mensagem do protocolo entre a REDE e placa IP (1 byte)
#define TYPE_SIP                        100

// Tipo: tipos de mensagem do protocolo Usado Internamente na placa IP (1 byte) //
#define TYPE_ICIP                       101

#define TYPE_INVALID			0xFF			// tipo não permitido

#define	ULTIMO_TIPO_DE_MENSAGEM		TYPE_ALARME+1	//Limite superior - válido - para tipo de mensagem

//Status do canal
#define CHN_NULL        0xFF            // canal não valido
#define CHN_OK          0x55            // canal ok
#define CHN_NOK         0xAA            // canal nok


// -----------------------------------------------------------------
//Mensagens do tipo TYPE_CONFIGURACAO, placa PABX -> ICIP
// -----------------------------------------------------------------
//#if !NEW_MSG_UNNITI //usado paela nas icips #TODO Retirar depois
//#define         VOX_FRM_NOTIFY                           13
//#endif


#define	        VOX_FRM_STARTUP				 0

//Mensagens do tipo TYPE_SINALIZACAO, PABX -> placa ICIP
#define         VOX_FRM_NOTIFY_LIGHT                     13
#define         VOX_FRM_NOTIFY_BLF                       20

#define		VOX_FRM_DESLIGAR_DEVICE			 222

// -----------------------------------------------------------------
//Mensagens do tipo TYPE_CONFIGURACAO, placa PABX -> ICIP
// -----------------------------------------------------------------
#define         VOX_FRM_SOCKET_UNREGISTER_CLIENT         500

// -----------------------------------------------------------------
//Mensagens do tipo TYPE_SINALIZACAO, placa ICIP -> PABX
//-----------------------------------------------------------------------------
#define         VOX_FRM_SUBSCRIBE                        12
// -----------------------------------------------------------------
//Mensagens do tipo TYPE_SIP, REDE -> placa ICIP
// -----------------------------------------------------------------

/*************************************** PABX->ICIP *************************************************
 Definição das estruturas de FRM RECEBIDAS placa ICIP (PABX->ICIP)
 Mensagem TYPE_CONFIGURACAO (Configuração)
*****************************************************************************************************/


/*****************************************************************************************************
 VOX_FRM_STARTUP (  )
*****************************************************************************************************/
typedef struct
{
  byte Status;            // Default = CHN_OK (0x55)
}TAG_STARTUP;
#define SIZE_TAG_STARTUP       1



/*****************************************************************************************************
  VOX_FRM_NOTIFY_BLF   ( 20d )
*****************************************************************************************************/
typedef struct
{
  enum  DirChamada      BLF_DirectionCall;                              // Campo 1: Para o serviço BLF: direção da ligação
  word                  BLF_status       ;        			// Campo 2: Status da Portas
  char                  BLF_TargetSubscriber[APP_MAX_NOME_PORTA];      // Campo 3: Para o serviço BLF: é o ramal monitorado
}TAG_NOTIFY_BLF;

#define SIZE_TAG_NOTIFY_BLF ( SIZE_DWORD+SIZE_WORD+APP_MAX_NOME_PORTA )



/*****************************************************************************************************
 VOX_FRM_RESP_OKNOK (  )
*****************************************************************************************************/
typedef struct
{
  BOOL	RespOk;
}TAG_RESP_OKNOK;

#define SIZE_TAG_OKNOK  1


/*****************************************************************************************************
 VOX_FRM_SOCKET_UNREGISTER_CLIENT (500d)
*****************************************************************************************************/
typedef struct
{
  byte ClientSocketId; //O ID do cliente é definido pelo enum IcipClient
  byte vago0;
  byte vago1;
  byte vago2;
}TAG_SOCKET_UNREGISTER_CLIENT;
#define SIZE_TAG_SOCKET_UNREGISTER_CLIENT  4


/*****************************************************************************************************
  VOX_FRM_SUBSCRIBE (12d )
*****************************************************************************************************/
typedef struct
{
  enum ICIPSubscribeType        RequestType;
  char                          BLF_TargetSubscriber[APP_MAX_NOME_PORTA];    // Para o serviço BLF: é o ramal destino que se deseja monitorar
  byte                          vago;
  dword                         BLF_SubscriptionExpirationTime;                // Para o serviço BLF: é o tempo em que a subscrição expira (em segundos; se zero cancela)
} TAG_SUBSCRIBE;
#define SIZE_TAG_SUBSCRIBE ( (2*SIZE_DWORD)+APP_MAX_NOME_PORTA+SIZE_BYTE )


/*****************************************************************************************************
//Esse estrutura é usada somente para receber BLF do APP_VOIP
*****************************************************************************************************/
typedef struct
{
  enum ICIPSubscribeType        RequestType;
  byte  Select;                 // select do endereço de hardware do driver
  byte  Position;               // Position do endereço de hardware do driver
  word  IndDNSDriverIP;         // Indice DNS do driver da porta ramal IP
  dword                         BLF_SubscriptionExpirationTime;                // Para o serviço BLF: é o tempo em que a subscrição expira (em segundos; se zero cancela)
  char                          BLF_TargetSubscriber[APP_MAX_NOME_PORTA];    // Para o serviço BLF: é o ramal destino que se deseja monitorar
} TAG_SUBSCRIBE_BLF;
#define SIZE_TAG_SUBSCRIBE_BLF ( (2*SIZE_DWORD) + (2*SIZE_BYTE) + SIZE_WORD + APP_MAX_NOME_PORTA )


/*
 * =================== FIM =========================================================================================================
 */
//===============================================================================================
//		Prototipos de funções
//===============================================================================================
byte    PesquisaIndiceComando (word comando);

/*
 *
 */
void FrmVoIPSubscriber (byte *ptrDataMessage);

/*
 *
 */
void FrmPBXNotify (byte *ptrDataMessage);



//-----------------------MENSAGEM ENVIADAS PARA O APP_VOIP -----------------------------------
/*
 *
 */
void  FrmNotifySEND ( word aplicDest, TKeys *aBlfKey);

/*
 *
 */
void  FrmNotifySEND_VOIP ( const TAG_SUBSCRIBE_BLF *ptrDataMessage);


//-----------------------MENSAGEM ENVIADAS PARA O PABX -----------------------------------
void  FrmSubscribeSEND_TOPABX ( TAG_SUBSCRIBE_BLF *ptsubscriber);
/*
 * Envia para o PABX
 * Refresh das teclas ainda ativas
 */
void FrmSubscribeRefresh_TOPABX(TKeys *exist_key);

#endif

