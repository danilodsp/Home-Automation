 /*
 Author: Danilo Pena

Description: main firmware with state machine
 */

#include "SM.h"

#include <msp430g2553.h>
#ifdef MSPGCC
#include <legacymsp430.h>
#endif

extern uint8_t  MdE_estadoAtual;


void main(void) {

#ifdef TESTANDO  
inicializandoTeste();
#endif 
   
	MdE_estadoAtual = INICIO;

	while(1){
	  switch ( MdE_estadoAtual ) {
	  case INICIO:
	    MdE_Comecar();
					       break;

	  case CONFIGURAR_ADDRESS:
	    MdE_ConfigurandoAddress();

					       break;
	  case AGUARDAR_CONFIGURAR_ADDRESS:
	    MdE_ArmazenandoID();

					       break;
	  case CONFIGURAR_IS :
	    MdE_ConfigurandoIS();

					       break;
	  case ENVIAR_MEDICAO:
	    MdE_EnviandoMedicao();
					       break;
	  case AGUARDAR_COMANDO_ATUAR:
	    MdE_Atuando();

					       break;
	  case ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR:
	    MdE_EnviandoMedicaoAcionandoAtuador();

					       break;
	  case ERROR:
	    MdE_ERROU();

					       break;
	  default:
					    break;
	  }
	}
}
