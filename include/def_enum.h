/******************************************************************* *
 *    Descrição: Definições de ENUM Usados
 *               em qualquer aplicação VoIP
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:09/02/2012
 *******************************************************************/

#ifndef DEF_ENUM_H_
#define DEF_ENUM_H_




enum MotivoDeslig
{
  DSL_NORMAL            = 1,         // desligamento normal (o outro lado desligou)      [Aqui libera Sala]
  DSL_ORDEM             = 2,         // veio ordem para desligar (sai da sala mas não libera) [ só tira da sal]
  DSL_DRVIVER_FORA_SERV = 3,         // desligamento anormal (ex: perda de sincronismo) [
  DSL_DESVIO            = 4,         // destino desligou e solicita desvio, não permite desvio recursivo
  DSL_TIMEOUT           = 5,         // desligamento forçado (o outro lado desligou)     [Aqui libera Sala e se reconectar deve criar nova sala]
  DSL_BLOQUEIO_PRG      = 6,         // desligamento por bloqueio programado
  DSL_BLOQUEIO_DET      = 7,         // desligamento por bloqueio detectado
  DSL_DIRECAO           = 8,         // desligamento por direção proibida
  DSL_BLOQUEIO_DDC_PRG  = 9,         // desligamento por bloqueio de acobrar programado
  DSL_RM_ECONOMICO      = 10,        // desligamento devido a ramal econômico = DSL_NORMAL, mas pode-se criar mensagem para isto
  DSL_RM_PRE_PAGO       = 11,        // desligamento devido ao Pre-pago = DSL_NORMAL, mas pode-se criar mensagem para isto
  DSL_FUGA_DESVIO       = 12,        // destino desligou e solicita desvio, atraves da função fuga, que permite um novo desvio
  DSL_OUTRO_ATENDEU     = 13,        // desligamento por atendimento de outro [ só tira da sal]
  DSL_DESVIO_CORREIO    = 14,        // destino desligou e solicita desvio para correio
  DSL_CHAMADA_LIMBO     = 15,        // Enviado repetidadmento quando o ramal está no limbo,
  DSL_RELEASE           = 16,        // usado pelo E1 (cits)
  DSL_TI_INICIC         = 17,        // TI envia quando terminou de inicializar
  DSL_NORMAL_COM_ESPERA = 18,        // para ramais IP apenas, quando existe chamada na espera
  DSL_NORMAL_COM_DISA   = 19,        // para ramais IP apenas, quando em programação de ramal, mensagem de DISA
  DSL_SEMCADASTRO       = 20,        // numero sem cadastro no controler
  DSL_SEMPREPAGO        = 21,        // pre-pago desconectado
  DSL_TRF_FILA          = 22,        // transfere direto da fila
  DSL_ATENDEU_FILA      = 23,        // ouve atendimento de ligação da fila
  DSL_REMOVIDO          = 24,        // Ramal/Juntor IP Removido
  DSL_INVALIDO          = 0xFFFFFFFF //

};

/*
 * ENUM tamanho de 1 Byte
 */
enum typeBroadcast
{
  BROADCAST_LAN_OFF     = 1,         // Cabo da LAN foi desconectado
  BROADCAST_LAN_ON      = 2,         // Cabo da LAN foi RE conectado
  BROADCAST_WAN_OFF     = 3,         // Cabo da WAN foi desconectado
  BROADCAST_WAN_ON      = 4,         // Cabo da WAN foi Re desconectado
  BROADCAST_RESET       = 5,         // RESET APLICACAO
  BROADCAST_START       = 6,         // INICIA APLICACAO
  BROADCAST_TIMOEUT_RESET = 7,         // Reseta depois de um tempo
  BROADCAST_TIMEOUT_START = 8,         // Inicia depois de um tempo
  BROADCAST_TIMEOUT_STOP  = 9,         // Para depois de um tempo
  BROADCAST_INVALIDO   = 0xFF //
};


enum ICIPSubscribeType
{
  ICIP_SBSCRB_VOID                = 0,
  ICIP_SBSCRB_VOICEMAIL           = 1,
  ICIP_SBSCRB_READY_TO_DISK_TONE  = 2,
  ICIP_SBSCRB_BLF                 = 3,          // Busy Lamp Field
  ICIP_SBSCRB_BLA                 = 4,          // Bridge Line Appearance
  ICIP_SBSCRB_HOTLINE             = 5,
  ICIP_SBSCRB_NEW_FIRMWARE        = 6,
  ICIP_SBSCRB_NEW_LOGIN           = 7,                                                                                        // admin no Programador web alterou o login do dispositivo IP
  ICIP_SBSCRB_NEW_PASSWORD        = 8,                                                                                        // admin no Programador web alterou o login do dispositivo IP
  ICIP_SBSCRB_NEW_USER            = 9,                                                                                        // admin no Programador web alterou o login do dispositivo IP
  ICIP_SBSCRB_SUB_SYSTEM          = 10,                                                                                        // admin no Programador web alterou o login do dispositivo IP
  ICIP_SBSCRB_INVALIDO            = 0xFFFFFFFF
};


enum DirChamada
{
  DIR_CALL_OUT,                                                                                                                                                           // chamada de saida
  DIR_CALL_IN,                                                                                                                                                            // chamada de entrada
  DIR_NO_CALL,                                                                                                                                                                     // sem chamada
  DIR_CALL_INVALIDO  = 0xFFFFFFFF
};

enum Estado_LED_BLF
{
  LED_APAGADO        = 0,     // off
  LED_VERMELHO_ACESO = 1,     // confirmed
  LED_VERDE_ACESO    = 2,     // terminated
  LED_VERMELHO_PISCA = 3,     // early
  LED_INVALIDO       = 0xFFFF   //Erro de Leitura

};


#endif /*DEF_ENUM_H_*/

