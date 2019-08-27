//****************************************************************************//
//  MSP430 state machine
//  MdE.c
//  Author: Danilo Pena
//
//  Describtion:
//    A state machine for the MSP430
//
//  Generated with hand :)
//  Date:  05/06/2012        Time:  17:38:00
//
//****************************************************************************//

#include "SM.h"  // 
#include <msp430.h>

uint8_t  MdE_estadoAtual;
uint8_t  MdE_sinal;


#ifdef TESTANDO
void inicializandoTeste(){
  P1DIR |= BIT5;
}

void acenderLEDverde(){
  P1OUT |= BIT6;
}

void acenderLEDvermelho(){
  P1OUT |= BIT0;
}

void acenderLEDamarelo(){
  P1OUT |= BIT5;
}

void apagarLEDamarelo(){
  P1OUT &= ~BIT5;
}

void apagarLEDverde(){
  P1OUT &= ~BIT6;
}

void apagarLEDvermelho(){
  P1OUT &= ~BIT0;
}
#endif

/* ############################ Testes de transição ##################

1 BIT -> Verde
2 BIT -> Vermelho
3 BIT -> Amarelo
 
  123
0 000 -> INICIO
1 001 -> CONFIGURAR_ADDRESS
2 010 -> AGUARDAR_CONFIGURAR_ADDRESS 
3 011 -> CONFIGURAR_IS
4 100 -> ENVIAR_MEDICAO
5 101 -> AGUARDAR_COMANDO_ATUAR
6 110 -> ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR
7 111 -> ERROR
 */


void MdE_Comecar(){     // Cheguei ao estado INICIO
   PO_LerMem();

#ifdef TESTANDO
   apagarLEDverde(); // estado 000
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   //   MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_INICIO;
   //MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_INICIO;
   //MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_DO_INICIO;
   //MdE_sinal = PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_INICIO;
   //MdE_sinal = PASSANDO_PARA_ERROR_DO_INICIO;
   //MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
#endif

  switch ( MdE_sinal ) {
  case PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_INICIO:     //Transição 1
    	MdE_estadoAtual = CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_INICIO:  //Transição 2
 	MdE_estadoAtual = ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR;
	break;

  case PASSANDO_PARA_ENVIAR_MEDICAO_DO_INICIO:   //Transição 3
 	MdE_estadoAtual = ENVIAR_MEDICAO;
	break;

  case PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_INICIO: //Transição 4
 	MdE_estadoAtual = AGUARDAR_COMANDO_ATUAR;
	break;

  case PASSANDO_PARA_ERROR_DO_INICIO: //Transição 5
 	MdE_estadoAtual = ERROR;
	break;

  case PASSANDO_PARA_INICIO_D_QQ_ESTADO: //Transição 6
 	MdE_estadoAtual = INICIO;
	break;
  }
}

void MdE_ConfigurandoAddress(){    // Cheguei ao estado CONFIGURAR_ADDRESS
#ifndef SEM_PO
  PO_ConfigurarAddress();
#endif

#ifdef TESTANDO

   apagarLEDverde(); // zerando
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   apagarLEDverde(); // estado 001
   apagarLEDvermelho();
   acenderLEDamarelo();
   timer(1000);
   //MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS;
   //MdE_sinal = PASSANDO_PARA_AGUARDAR_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS;
   //MdE_sinal = PASSANDO_PARA_ERROR_DO_CONFIGURAR_ADDRESS;
   //MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
#endif

  switch ( MdE_sinal ) {

  case  PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS: //Transição 1
    MdE_estadoAtual = CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_AGUARDAR_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS://Transição 2
    MdE_estadoAtual = AGUARDAR_CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_ERROR_DO_CONFIGURAR_ADDRESS://Transição 3
    MdE_estadoAtual = ERROR;
	break;

  case PASSANDO_PARA_INICIO_D_QQ_ESTADO://Transição 4
    MdE_estadoAtual = INICIO;
	break;

  }
}

void MdE_ArmazenandoID(){// Cheguei ao estado AGUARDAR_CONFIGURAR_ADDRESS 	
#ifndef SEM_PO
  PO_ArmazenandoID();
#endif

#ifdef TESTANDO
   apagarLEDverde(); // zerando
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   apagarLEDverde(); // estado 010
   acenderLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   //     MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_AGUARDAR_CONFIGURAR_ADDRESS;
   //MdE_sinal = PASSANDO_PARA_CONFIGURAR_IS_DO_AGUARDAR_CONFIGURAR_ADDRESS;
     //  MdE_sinal = PASSANDO_PARA_ERROR_DO_AGUARDAR_CONFIGURAR_ADDRESS;
     // MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
#endif

  switch ( MdE_sinal ) {

  case PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_AGUARDAR_CONFIGURAR_ADDRESS://Transição 1
    MdE_estadoAtual = CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_CONFIGURAR_IS_DO_AGUARDAR_CONFIGURAR_ADDRESS://Transição 2
    MdE_estadoAtual = CONFIGURAR_IS;
	break;

  case  PASSANDO_PARA_ERROR_DO_AGUARDAR_CONFIGURAR_ADDRESS://Transição 3
    MdE_estadoAtual = ERROR;
	break;

  case PASSANDO_PARA_INICIO_D_QQ_ESTADO://Transição 4
    MdE_estadoAtual = INICIO;
	break;
  }

}


void MdE_ConfigurandoIS(){// Cheguei ao estado CONFIGURAR_IS
#ifndef SEM_PO
  PO_ConfiguraIS();
#endif 

#ifdef TESTANDO
   apagarLEDverde(); // zerando
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   apagarLEDverde(); // estado 011
   acenderLEDvermelho();
   acenderLEDamarelo();
   timer(1000);
   //MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_IS;
   //MdE_sinal = PASSANDO_PARA_CONFIGURAR_IS_DO_CONFIGURAR_IS;
   //MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS;
   //MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_DO_CONFIGURAR_IS;
   //MdE_sinal = PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS;
   //MdE_sinal = PASSANDO_PARA_ERROR_DO_CONFIGURAR_IS;
   //MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
#endif

  switch ( MdE_sinal ) {

  case  PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_IS://Transição 1
    MdE_estadoAtual = CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_CONFIGURAR_IS_DO_CONFIGURAR_IS://Transição 2
    MdE_estadoAtual = CONFIGURAR_IS;
	break;

  case PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS://Transição 3
    MdE_estadoAtual = ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR;
	break;

  case PASSANDO_PARA_ENVIAR_MEDICAO_DO_CONFIGURAR_IS://Transição 4
    MdE_estadoAtual = ENVIAR_MEDICAO;
	break;

  case PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS: //Transição 5
    MdE_estadoAtual = AGUARDAR_COMANDO_ATUAR;
	break;

  case PASSANDO_PARA_ERROR_DO_CONFIGURAR_IS: //Transição 6
    MdE_estadoAtual = ERROR;
	break;

  case PASSANDO_PARA_INICIO_D_QQ_ESTADO://Transição 7
    MdE_estadoAtual = INICIO;
	break;
  }
}


void MdE_EnviandoMedicao(){// Cheguei ao estado ENVIAR_MEDICAO
#ifndef SEM_PO
  PO_EnviarMedicao();
#endif

#ifdef TESTANDO
   apagarLEDverde(); // zerando
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   acenderLEDverde(); // estado 100
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   //   MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_ENVIAR_MEDICAO;
   //MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_DO_ENVIAR_MEDICAO;
   //MdE_sinal = PASSANDO_PARA_ERROR_DO_ENVIAR_MEDICAO;
   //MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
#endif

  switch ( MdE_sinal ) {

  case PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_ENVIAR_MEDICAO://Transição 1
    MdE_estadoAtual = CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_ENVIAR_MEDICAO_DO_ENVIAR_MEDICAO://Transição 2
    MdE_estadoAtual = ENVIAR_MEDICAO;
	break;

  case PASSANDO_PARA_ERROR_DO_ENVIAR_MEDICAO://Transição 3
    MdE_estadoAtual = ERROR;
	break;

  case PASSANDO_PARA_INICIO_D_QQ_ESTADO://Transição 4
    MdE_estadoAtual = INICIO;
	break;
  }
}


void MdE_Atuando(){// Cheguei ao estado AGUARDAR_COMANDO_ATUAR
#ifndef SEM_PO
  PO_Atuar();
#endif

#ifdef TESTANDO
   apagarLEDverde(); // zerando
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   acenderLEDverde(); // estado 101
   apagarLEDvermelho();
   acenderLEDamarelo();
   timer(1000);
   //MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_AGUARDAR_COMANDO_ATUAR;
   //MdE_sinal = PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_AGUARDAR_COMANDO_ATUAR;
   //MdE_sinal = PASSANDO_PARA_ERROR_DO_AGUARDAR_COMANDO_ATUAR;
   //MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
#endif

  switch ( MdE_sinal ) {

  case PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_AGUARDAR_COMANDO_ATUAR://Transição 1
    MdE_estadoAtual = CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_AGUARDAR_COMANDO_ATUAR://Transição 2
    MdE_estadoAtual = AGUARDAR_COMANDO_ATUAR;
	break;

  case PASSANDO_PARA_ERROR_DO_AGUARDAR_COMANDO_ATUAR://Transição 3
    MdE_estadoAtual = ERROR;
	break;

  case PASSANDO_PARA_INICIO_D_QQ_ESTADO://Transição 4
 	MdE_estadoAtual = INICIO;
	break;
  }
}


void MdE_EnviandoMedicaoAcionandoAtuador(){// Cheguei ao estado ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR
#ifndef SEM_PO
  PO_EnviarMedicaoComAtuacao();
#endif

#ifdef TESTANDO
   apagarLEDverde(); // zerando
   apagarLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   acenderLEDverde(); // estado 110
   acenderLEDvermelho();
   apagarLEDamarelo();
   timer(1000);
   //MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR;
   //MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR;
   //MdE_sinal = PASSANDO_PARA_ERROR_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR;
   //MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
#endif

  switch ( MdE_sinal ) {

  case PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR://Transição 1
    MdE_estadoAtual = CONFIGURAR_ADDRESS;
	break;

  case PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR://Transição 2
    MdE_estadoAtual = ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR;
	break;

  case PASSANDO_PARA_ERROR_DO_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR://Transição 3
 	MdE_estadoAtual = ERROR;
	break;

  case PASSANDO_PARA_INICIO_D_QQ_ESTADO://Transição 4
 	MdE_estadoAtual = INICIO;
	break;
  }
}


void MdE_ERROU(){// Cheguei ao estado ERROU !  ## FIM DO JOGO ## VC PERDEU !!..
#ifndef SEM_PO
  PO_Error();
#endif

#ifdef TESTANDO

  acenderLEDverde(); // estado 111
  acenderLEDvermelho();
  acenderLEDamarelo();
  timer(1000);
  apagarLEDverde(); // estado 111
  apagarLEDvermelho();
  apagarLEDamarelo();
  timer(1000);
#endif


  switch ( MdE_sinal ) {

  case PASSANDO_PARA_ERROR_DO_ERROR://Transição SEM VOLTA;
 	MdE_estadoAtual = ERROR;
	break;
  }
}
