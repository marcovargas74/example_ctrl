/********************************************************************
 *    Descrição: Funções de uso Geral da placa ICIP Intelbras
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:16/12/2008
 *******************************************************************/

#ifndef __CONVERT_TYPES_VAR_H_
#define __CONVERT_TYPES_VAR_H_


#include "def_types.h"


//Prototipos de funções

/******************************************************************************************
        Function: byteswap2
        Description: Realiza a inversão de bytes em um short.
*******************************************************************************************/
word byteswap2(word val);


/******************************************************************************************
        Function: byteswap4
        Description: Realiza a inversão de bytes em um int.
*******************************************************************************************/
dword byteswap4(dword val);


//-----------------------------------------------------------------------------
//FUNCÇÕES DE CONVERSAO DE TIPOS DE VARIÁVEIS
//OBS: Funções auxiliares como: pj_swap16(pj_int16_t val16), podem ser encontradas em <types.h>
//-----------------------------------------------------------------------------
/* Converte 2 bytes em uma WORD
 * Entra  :  Um vetor de Byte (0x01 0x23)
 * Retorna:  Uma Word         (0x0123)
 */
void ByteToWord ( word *valor, const byte *ptrMsg );


/* Converte 2 bytes em uma WORD Invertida
 * Entra  :  Um vetor de Byte  (0x01 0x23)
 * Retorna:  Uma Word Invertida (0x2301)
 */
void ByteToWordInv ( word *valor, const byte *ptrMsg );


/**
 * Converte 1 WORD em 2  bytes
 * Entra   : Uma Word (0x0123)
 * Retorna : Um vetor de Byte  (0x01 0x23)
 */
void WordToByte( byte *valor, const word umaWord);


/**
 * Converte 1 WORD em 2  bytes Invertidos
 * Entra   : Uma Word (0x0123)
 * Retorna : Um vetor de Byte  (0x23 0x01)
 */
void WordToByteInv( byte *valor, const word umaWord);


/**
 * Converte 1 DWORD em 4  bytes
 * Entra   : Uma DWord (0x12 34 56 78)
 * Retorna : Um vetor de Byte  (0x12 0x34 0x56 0x78 )
 */
void DWordToByte( byte *valor, const dword umaDWord);


/**
 * Converte 1 DWORD em 4  bytes Invertidos
 * Entra   : Uma DWord (0x12 34 56 78)
 * Retorna : Um vetor de Byte  (0x78 0x56 0x34 0x12)
 */
void DWordToByteInv( byte *valor, const dword umaDWord);


/**
 * Converte 4 bytes em uma DWORD
 * Entra  :  Um vetor de Byte (0x12 0x34 0x56 0x78 )
 * Retorna:  Uma Word         (0x12345678)
 */
void ByteToDWord ( dword *valor, const byte *ptrMsg );


/**
 * Converte 4 bytes em uma DWORD Invertida
 * Entra  :  Um vetor de Byte   (0x12 0x34 0x56 0x78 )
 * Retorna:  Uma Word Invertida (0x78 56 34 12)
 */
void ByteToDWordInv ( dword *valor, const byte *ptrMsg );


/**
 * Converte 1 Byte em 2 char
 * Entra   : Um Byte (0x12 )
 * Retorna : Uma string de tamanho 2  (0x31 0x32 )
 * COMO USAR:
 *       char str[2];
 *       byte argByte = 0x12;
 *           ByteToStr( str, argByte);
 *
 */
void ByteToStr( byte *valor, const byte umByte);


/**
 * Converte um vetor de Hex em uma string
 * Entra   : Um Byte (0x12 0x34 0x56 0x78 0x9A 0xBC 0xDE 0xF0)
 * Retorna : Uma string de tamanho 2  (0x31 0x32..... 0x70 0x30 )
 * COMO USAR:
 *       char str[tamVetor*3+1];
 *       byte vetorByte[tamVetor];
 *           ByteToStr( str, vetorByte);
 *obs: SEMPRE USAR TAMANHO DA strinHex 3 VEZES MAIOR QUE A VARIVEL umVetorBytes mais 1 byte para o "/0"
 */
void ConvHexToStr( byte *stringHex, const byte *umVetorBytes, int sizeHex);

#endif //__CONVERT_TYPES_VAR_H_
