/******************************************************************************************************
 * Copyright(c) 2010 Intelbras S/A, All Rights Reserved.
 * This software is proprietary and confidential.
 *
 * File: gerencia_rede.h
 * Author: João Paulo Bodanese <jpb@intelbras.com.br>
 * Date: 01/07/2009
 * Description:
 *
 * Documentation:
 * 			https://icorpsvn.intelbras.com.br/ICIP_Software/browser/docs/descricao_tecnica/ICIP_8_12_Canais/Linux_Pbxcom/PBXCOM.odt
 * Revision:
 * 			0.0 (01/07/2009):
 ******************************************************************************************************/

#ifndef _MAIN_CONTROL_H
#define	_MAIN_CONTROL_H


#define   TIME_EXEC_30SEC       2//  3000 para 10ms
#define   TIME_EXEC_3MIN        3
#define   TIME_EXEC_10MIN       10


/*
 *  FUNCTION DECLARATION
 */
void CloseApp_ctrl(void);

void TrataComandosVindoDoLinux(void);
 

/*
 * Função de Inicialização de diversas Variaveis
 * Usadas na Aplicação
 */
void init_parm_app(void);

#endif //_BLF_CONTROL_H
