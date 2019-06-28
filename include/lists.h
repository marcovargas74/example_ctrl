/********************************************************************
 *    Descrição: Funções referentes a manipulações de Lista
 *    			 encadeadas.
 *
 *    Autor: Marco Antonio Vargas
 *
 *    Data:18/02/2009
 *******************************************************************/

#ifndef LISTS_H_
#define LISTS_H_

//Includes
//#include "defin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Defines
//#define		MAX_REG_UA      255  //Tamanho máximo de registros
#define			TAM_MAX_POINT   5   //Tamanho máximo de pontos de procura
#define                 START_NUM_POINT   50   //Apartir de quantos nós se desenja marcar


/**
 * Use this macro in the start of the structure declaration to declare that
 * the structure can be used in the linked list operation. This macro simply
 * declares additional member @a prev and @a next to the structure.
 * @hideinitializer
 */
#define DECL_LIST_MEMBER(type)                       \
                                   /** List @a prev. */ \
                                   type *prev;          \
                                   /** List @a next. */ \
                                   type *next


/**
 * This structure describes generic list node and list. The owner of this list
 * must initialize the 'value' member to an appropriate value (typically the
 * owner itself).
 * /
struct pj_list
{
    PJ_DECL_LIST_MEMBER(void);
} PJ_ATTR_MAY_ALIAS; // may_alias avoids warning with gcc-4.4 -Wall -O2
*/




 // Definicao do Elemento de uma Lista
typedef struct node
{
  //DECL_LIST_MEMBER(struct Node); //Usado pelo PJSIP depois pode Juntar os dois
  struct node* Prev; // proximo nó
  struct node* Next; // proximo nó
  void *PtrStruct;  //  conteudo
} Node;


//tabela de Registro
typedef struct
{
  Node *FirstNode; //Primeiro Nó da Lista
  Node *LastNode;  //Ultimo nó da Lista
  int   SizeList;  //Quantidade de nó da Lista

  //Node *NodeSearch[TAM_MAX_POINT+1]; //Pontos de Procura
  int  Flags;  //Uso difersos
  //pj_mutex_t   *mutex_tab;     //< Dialog's mutex.!!

} ListNode;


/*
 * Usado em controle de Lista de Operadoras
 * Todos Registros ja Foram Enviados nao existe nenum Pendente
 */
//#define ALL_START_REGISTER  0x0001  //Usado em controle de Lista de Operadoras
//#define MASK_START_REGISTER 0x1110  //Usado em controle de Lista de Operadoras

//Prototipos de funções
ListNode *CreateList(void);
Node     *CreateNode (void *ptrStruct);

void    IncludeNode( ListNode *list, Node *nodeNew, __compar_fn_t __compar );
Node    *FindNode( ListNode *list, Node *nodeFind, __compar_fn_t __compar );
int     DeleteNode( ListNode *list, Node *nodeDel, __compar_fn_t __compar );
Node    *GetNodePos( ListNode *list, int pos );
void    MarkPointSearch( ListNode *list, int nPoint );
Node    *PositionSearch( ListNode *list, Node *nodeFind, __compar_fn_t __compar );

void    FreeList    ( ListNode *list );

//void LinkNode(Node *nd1, Node *nd2);
//Node *IncludeNode(Node *aNode);


/**
 * Initialize the list.
 * Initially, the list will have no member, and function pj_list_empty() will
 * always return nonzero (which indicates TRUE) for the newly initialized
 * list.
 *
 * @param node The list head.
 */
void list_init(void * node);

/*
 *
 */
void list_erase(void *node);

#endif /* LISTS_H_ */
