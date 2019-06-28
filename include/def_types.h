/******************************************************************* *
 *    Descrição: Definições de Variaveis e estruturas de Dados Usados
 *               em qualquer aplicação VoIP
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:09/02/2012
 *******************************************************************/


#ifndef _DEF_TYPES_H__
#define _DEF_TYPES_H__

#include "defin.h"
#include "lists.h"
//#include "keys.h"

#include <signal.h>


/*
 * Tipo (int)   - Tamanho em bytes (4 bytes)
 * Tipo (short) - Tamanho em bytes (2 bytes)
 * Tipo (byte)  - Tamanho em bytes (1 byte)
 * Tipo (long)  - Tamanho em bytes (8 bytes)
 * Tipo (float) - Tamanho em bytes (4 bytes)
 * Tipo (double) - Tamanho em bytes (8 bytes)
 * Tipo (boolean)- Tamanho em bytes (1 bit)
 * Tipo (char)   - Tamanho em bytes (1 bytes)
 */


//Definições de tipos de Variaveis  usadas
typedef  unsigned char  	byte;    // 1 byte
typedef  unsigned short 	word;    // 2 bytes
typedef  unsigned int   	dword;   // 4 bytes
typedef  unsigned long          uLong;   // 8 bytes
typedef  int            	bool;
typedef  byte 			BOOL;



// Codec descriptor:
struct codec
{
    unsigned            pt;
    char*               name;
    unsigned            clock_rate;
    unsigned            bit_rate;
    unsigned            ptime;
    char*               description;
};


/* This is a call structure that is created when the application starts
 * and only destroyed when the application quits.
 */
struct call
{
    unsigned                    index;
    void                        *pt_portIP_this;              //ponteiro para porta IP em uso
    void                        *pt_portIP_remote;       //ponteiro para porta IP Remoto ou seja com qual este está falando caso for IP
        //estado_enum                           estado;                 //Estado da máquina de estados
    //evento_enum                               evento;                 //Evento responsável por mudar o estado da máquina
    //unsigned int                      idDaMensagem;
};


/* TIPO de estrutura de dados de controle de chamada corrente
 * Usado para controlar estado d euma chamada Corrente.
 * /
typedef struct Tctrl_call_current
{

  // ***!!!ATENÇÂO Sempre que incluir um novo comapo é necessario alterar o tamamnho do ICIP_MAX_DADOS_CHAMADA_SAINTE
  enum RouteEth  InterfaceRoteamento;  //Interface de roteamento de saida  // DWORD InterfaceRoteamento;
 enum ICIPTypeCall_IP TypeCall_IP;         //Esse ENUM é do tamanhdo de 1 BYTE          //Tipo de chamamda IP TTrue se chamada for ponto a Ponto
 BOOL LerDados;                       //Tipo De Ring Da Chamada
 BOOL IdentificaOrigem;                  // Controle de identificação da origem (se setado, identificar)
 byte TipoRing;                       //Tipo De Ring Da Chamada
 byte TipoAtendeu;                     //Tipo De Atendimento/ Pode ser atendeu Falso/Disa/183
 dword MotivoDesvio;                            // Motivo do desvio (486 "Busy Here", 408"Request Timeout" ou 480"Temporarily unavailable";
 word IndNomeOrigem;                  //Indice DNS da Origin. Usado Quando destino e ramal
  char NomeOrigem[ICIP_MAX_NOME_EXTERNO];    // Nome da porta origem
  char OrigemContaPiloto[ICIP_MAX_NOME_EXTERNO];    // Nome da porta origem
  char DadosDiscados[ICIP_MAX_NOME_DISCADO]; // Dados discados no JuntorIP
  char EndIPDestino[ICIP_MAX_IP_DOMAIN];    //número IP do destino a ser chamado
  char NickOrigem[ICIP_MAX_NOME_EXTERNO]; // Campo 16: Apelido da porta originadora da chamada
  char GrupoHotel[ICIP_MAX_NOME_PORTA_2];    // Grupo/
}Tctrl_call_current;

//Atencao o enum ICIPTYPECall_IP tem TAMANHO de 1 BYTE
#define SIZE_TYPE_CTRL_CALL_CURRENT    ((3*ICIP_MAX_NOME_EXTERNO) + ICIP_MAX_NOME_DISCADO + ICIP_MAX_IP_DOMAIN + (1*ICIP_MAX_NOME_PORTA_2) + (5*SIZE_BYTE) + (2*SIZE_DWORD) + SIZE_WORD)
//#define ICIP_MAX_DADOS_CHAMADA_SAINTE  ((2*ICIP_MAX_NOME_EXTERNO) + ICIP_MAX_NOME_DISCADO + ICIP_MAX_IP_DOMAIN + (2*ICIP_MAX_NOME_PORTA_2) + (4*SIZE_BYTE) + (2*SIZE_DWORD) + SIZE_WORD)
*/
/**
 * Estrutura de sockets usados nas Interfaces de SAIDAS para formatar o campo VIA
 * General host:port pair, used for example as Via sent-by.
 */
/**
 * Estrutura de sockets usados nas Interfaces de SAIDAS para formatar o campo VIA
 * General host:port pair, used for example as Via sent-by.
 *
typedef struct icip_host_port
{
  int id;
  pjsip_transport *p_transport; //ponteiro para o Transporte onde é guardado o IP do Socket de saida
  //pjsip_tpfactory *p_factory;
 // pjsip_tpfactory *p_factoryTLS;
  pj_str_t host;                // Host part or IP address.
  int      port;                // Port number.

} icip_host_port;


typedef struct
{
  byte     Channels;                   // Campo 1: Numero de canais. Default: 0x00
  byte     TDMmap[ICIP_MAX_CANAL];     // Ex1: se TDMmap[0]  = 0x12, significa que o canal 1   do chip deve usar o timeslot 18d
  icip_media_stream *media[ICIP_MAX_CANAL];     //Ponteiro para a chamada em curso usado na thread media
  word     ChannelInUse_Trunk;                  //Canais Que estão sendo Usados por juntores IPs
  word     ChannelInUse_Subscriber;             //Canais que estão sendo Usados por ramais IP
  word     ChannelInUse_NotReserved;            //Canais que estao sendo usados sem reserva
  word     ChannelMAX_Trunk;                    //Maximo de canais que podem ser usados pelos Juntores.

}TAG_TDM_MAP_CHAMADA;                      // Ex2: se TDMmap[32] = 0x00, significa que o canal 33  do chip deve usar o timeslot 00d


//Estrutura de Contadores de Erro
//Usado para Canais TDM
typedef struct
{
  byte  status_TDM_Curr;         //Estado Atual dos Canais TDM
  int   NumCall_Curr;       //Num chamada correntes
  int   Count_OCP_Crit;      //Numero de vezes que o sistema entrou em Nivel Critico
  int   Count_OCP_Busy;      //Numero de vezes que o sistema ocupou todos os canais TDM
} Status_TDM_Channel;
*/

/*
 *
 */
typedef struct
{
  DECL_LIST_MEMBER(struct TypePortDNS);    /**< Generic list member.   */
  byte    position;
  byte    selelect;
  word    IndiceDNS;       // Índice DNS do driver/PABX  Usado Como Chave
  dword   expiration;      // Para o serviço BLF: é o tempo em que a subscrição expira (em segundos; se zero cancela)

} TypePortDNS;


typedef struct
{
  DECL_LIST_MEMBER(struct TKeys);    /**< Generic list member.   */
  enum  DirChamada    BLF_DirectionCall;
  word          Index;
  word          status_blf;
  byte          key_name[APP_MAX_NOME_PORTA];
  TypePortDNS   ListIndDNS;

} TKeys;



// Application's global variables
struct app
{
    unsigned                    thread_count;
    pthread_t                   router_thread;         // Thread de recepção e roteamento das mensagens vindas do PABX* /
    byte                        kill_application;
    //Variaveis referentes ao proxy
//    unsigned                    name_cnt;     //Numero de dominios
    //TABELAS DE Uso Geral
    TKeys                 Tab_BLF_key;
    int                   sizeKey;
    int                         Level_LOG_Prog;
    BOOL                        Is_LOG_Prog;
//    BOOL                        icip_DSP_Ready;                   //O DSP ja esta pronto para realizar chamadas
    //pj_mutex_t                  *mutex_blf;                    //Mutex usado para compartilhar o recurso de rede
    char                        version_software[APP_MAX_NOME_VERSAO];      //Versao da aplicação Default

};//app;

#if 0


///////////////////   maquina de estados semswitch:case ///////////////////////
typedef struct Fsm Fsm;
//        State é uma Tipo de variável ponteiro para função e data os seus parâmetros da função
typedef void  (*State)(Fsm *me,  unsigned const data);

struct Fsm
{
  State state_;       // protected current state
};

#define FsmInit(me_) ( (*(me_)->state_)((me_), 0) )   //Define de um Ponterido de Função
#define FsmDispatch(me_, data_) ( (*(me_)->state_)((me_), data_) )   //Define de um Ponterido de Função
//#define FsmDispatch (me_, data_) ( (*(me_)->state_)( (me_), data) )   //Função que dispara tratamento da mensagem para o estado ()Previamente definido)
#define TRAN(target_) ( ((Fsm *)me)->state_ = (State)(target_) )     //Função que Força Mudança de estados


typedef struct StateMach
{
   Fsm super_;        // herda da classe Fsm
} StateMach;


#define _SHOW_APP_PERROR(status) show_app_perror(__THIS_FILE__ , __LINE__, status);


////////////////////////// DEFINECAO DE STRUCTS//////////////////////////////////////////////////////

#endif //0

//Estrutura de Contadores de Erro
typedef struct
{
  byte  ErrorAlloc;          //Erros de alocação de memória;
  byte  ErrorComSocket;       //Erros de cominicação com sockets;
  uLong Generic_cont1;
  uLong Generic_cont2;
} Status_VoIP;


//Estrutura de Pids Das Threads
typedef struct
{
  int   Pid_Main;               //Pid da Thread Principal;
  int   Pid_PABXCOMM_socket;    //Pid da Thread de comunicação com o PABX;
} Status_PID_VoIP;


#endif /*DEF_TYPES_H*/

