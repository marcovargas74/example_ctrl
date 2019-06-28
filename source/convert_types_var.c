/********************************************************************
 *    Descrição: Funções usadas para converter alguns tipos.
 *
 *    Autor: Marco Antonio Vargas
 *    Autor: Jairo Jair Silva
 *
 *    Data:10/02/2012
 *******************************************************************/


#include "convert_types_var.h"

#define THIS_FILE   "convert_types_var.c"



////Declaração de Um Vetor de Char usado para convertes Uma Valor Hex em Char
const char VetorChar[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


/******************************************************************************************
        Function: byteswap2
        Description: Realiza a inversão de bytes em um short.
*******************************************************************************************/
word byteswap2(word val)
{
  return (((val >> 8) & 0xFF) | ((val & 0xFF) << 8));
}

/******************************************************************************************
        Function: byteswap4
        Description: Realiza a inversão de bytes em um int.
*******************************************************************************************/
dword byteswap4(dword val)
{
  unsigned int result;
  short tm1,tm2;

  result = val;
  tm1 = result;
  tm2 = (result >> 16);

  tm2 = ((tm2 >> 8) & 0xFF) | ((tm2 & 0xFF) << 8);
  tm1 = ((result >> 8) & 0xFF) | ((result & 0xFF) << 8);
  result = (tm1 << 16) | tm2;

  return result;

}


//-----------------------------------------------------------------------------
//FUNCÇÕES DE CONVERSAO DE TIPOS DE VARIÁVEIS
//OBS: Funções auxiliares como: pj_swap16(pj_int16_t val16), podem ser encontradas em <types.h>
//-----------------------------------------------------------------------------
/* Converte 2 bytes em uma WORD
 * Entra  :  Um vetor de Byte (0x01 0x23)
 * Retorna:  Uma Word         (0x0123)
 */
void ByteToWord ( word *valor, const byte *ptrMsg )
{
   *valor = *ptrMsg;
   *valor <<= 8;
   *valor |= *(ptrMsg+1);
}

/* Converte 2 bytes em uma WORD Invertida
 * Entra  :  Um vetor de Byte  (0x01 0x23)
 * Retorna:  Uma Word Invertida (0x2301)
 */
void ByteToWordInv ( word *valor, const byte *ptrMsg )
{
   *valor = *(ptrMsg+1);
   *valor <<= 8;           //Desloca 8 bits
   *valor |= *(ptrMsg);
}

/* Converte 1 WORD em 2  bytes
 * Entra   : Uma Word (0x0123)
 * Retorna : Um vetor de Byte  (0x01 0x23)
 */
void WordToByte( byte *valor, const word umaWord)
{
  valor[1] =  umaWord;
  valor[0] = (umaWord >> 8) ;
}

/* Converte 1 WORD em 2  bytes Invertidos
 * Entra   : Uma Word (0x0123)
 * Retorna : Um vetor de Byte  (0x23 0x01)
 */
void WordToByteInv( byte *valor, const word umaWord)
{
  memcpy(valor, &umaWord, SIZE_WORD );
}


/* Converte 1 DWORD em 4  bytes
 * Entra   : Uma DWord (0x12 34 56 78)
 * Retorna : Um vetor de Byte  (0x12 0x34 0x56 0x78 )
 */
void DWordToByte( byte *valor, const dword umaDWord)
{
  valor[3] =  umaDWord;
  valor[2] = (umaDWord >> 8) ;
  valor[1] = (umaDWord >> 16);
  valor[0] = (umaDWord >> 24) ;
}

/* Converte 1 DWORD em 4  bytes Invertidos
 * Entra   : Uma DWord (0x12 34 56 78)
 * Retorna : Um vetor de Byte  (0x78 0x56 0x34 0x12)
 */
void DWordToByteInv( byte *valor, const dword umaDWord)
{
  memcpy(valor, &umaDWord, SIZE_DWORD );
}

/* Converte 4 bytes em uma DWORD
 * Entra  :  Um vetor de Byte (0x12 0x34 0x56 0x78 )
 * Retorna:  Uma Word         (0x12345678)
 */
void ByteToDWord ( dword *valor, const byte *ptrMsg )
{
  *valor = *ptrMsg;
  *valor <<= 8;

  *valor |= *(ptrMsg+1);
  *valor <<= 8;

  *valor |= *(ptrMsg+2);
  *valor <<= 8;

  *valor |= *(ptrMsg+3);

}


/* Converte 4 bytes em uma DWORD Invertida
 * Entra  :  Um vetor de Byte   (0x12 0x34 0x56 0x78 )
 * Retorna:  Uma Word Invertida (0x78 56 34 12)
 */
void ByteToDWordInv ( dword *valor, const byte *ptrMsg )
{
   *valor = *(ptrMsg+3);
   *valor <<= 8;           //Desloca 8 bits

   *valor |= *(ptrMsg+2);
   *valor <<= 8;           //Desloca 8 bits

   *valor |= *(ptrMsg+1);
   *valor <<= 8;           //Desloca 8 bits

   *valor |= *(ptrMsg);

}

 /* Entra   : Um Byte (0x12 )
 * Retorna : Uma string de tamanho 2  (0x31 0x32 )
 * COMO USAR:
 *       char str[2];
 *       byte argByte = 0x12;
 *           ByteToStr( str, argByte);
 *
 */
void ByteToStr( byte *valor, const byte umByte)
{
  valor[0] = VetorChar[(umByte >> 4)];
  valor[1] = VetorChar[(umByte & 0x0F)];
}


//--------------------------------------------------------------------------------------
// Function: AsciiToHex
// Description: Convert ASCII char to Hex number.
//--------------------------------------------------------------------------------------
int AsciiToHex(char c)
{
   int num = (int) c;
   if(num < 58 && num > 47) {
      return num - 48;
   }
   if(num < 103 && num > 96) {
      return num - 87;
   }
   return num;
}


/* Converte um vetor de Hex em uma string
 * Entra   : Um Byte (0x12 0x34 0x56 0x78 0x9A 0xBC 0xDE 0xF0)
 * Retorna : Uma string de tamanho 2  (0x31 0x32..... 0x70 0x30 )
 * COMO USAR:
 *       char str[tamVetor*3+1];
 *       byte vetorByte[tamVetor];
 *           ByteToStr( str, vetorByte);
 *obs: SEMPRE USAR TAMANHO DA strinHex 3 VEZES MAIOR QUE A VARIVEL umVetorBytes mais 1 byte para o "/0"
 */
void ConvHexToStr( byte *stringHex, const byte *umVetorBytes, int sizeHex)
{
  int i=0;
  int j=0;

  for (i=0; i < sizeHex; i++)
    {
      ByteToStr( &stringHex[j], umVetorBytes[i]  );
      j += 2;
      stringHex[j++] = '-';
    }

  stringHex[j-1] = 0;
}

