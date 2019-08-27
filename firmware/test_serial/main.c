/*
Author: Danilo Pena
Description: ADC reading and general tests

 */

#include <msp430g2553.h>
#include "test.h"

int valorTimer;

void main(void) {
	//char *ptr;

	// Configura��es gerais
 	WDTCTL = WDTPW + WDTHOLD;		// Desliga watchdog
	BCSCTL1 = CALBC1_1MHZ;			// Configura��o do DCO para 1.0 MHz
	DCOCTL = CALDCO_1MHZ;
	P1DIR |= BIT0 + BIT1 + BIT2 + BIT6;			// Configura��o de IO
	P1OUT = 0x00;					// Reset pinos

	/*
	FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator

	ptr = (char *)0x1040;
	FCTL1 = FWKEY + ERASE;                    // Set Erase bit
	FCTL3 = FWKEY;  						  // Clear Lock bit
	*ptr = 0x00;
	FCTL1 = FWKEY + WRT;
	*ptr++ = 0x01;
	*ptr++ = 0x02;
	*ptr++ = 0x03;
	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit
	*/

	// Habilitando LPM e Interrup��es
	//_BIS_SR(GIE);
	_BIS_SR(CPUOFF);
}

void erase_flash(void)
{
	char *Flash_ptr; //Aloca ponteiro para Flash

	WDTCTL = WDTPW + WDTHOLD; //P�ra Watch Dog Timer
	_disable_interrupts(); //Desbilita interrup��es
	Flash_ptr = (char *)0xFA00; //Ponteiro para Flash
	FCTL2 = FWKEY + FSSEL0 + 20; //Define senha, fonte de CLK, Fator div CLK
	FCTL1= FWKEY + ERASE; //Apagamento apenas do segmento espec�fico
	FCTL3 = FWKEY + LOCKA; //Senha, Desabilita prote��o
	*Flash_ptr=0; //Limpa ponteiro
	FCTL1 = FWKEY; //Senha
	FCTL3 = FWKEY + LOCKA + LOCK; //
	Flash_ptr = (char *)0xFC00; //Aponta outro segmento
	FCTL2 = FWKEY + FSSEL0 + 20; //Define senha, fonte de CLK, Fator div CLK
	FCTL1= FWKEY + ERASE; //Apagamento apenas do segmento espec�fico
	FCTL3 = FWKEY; //Senha
	*Flash_ptr=0; //Limpa ponteiro
	FCTL1 = FWKEY; //Senha
	FCTL3 = FWKEY + LOCKA + LOCK; //
	_enable_interrupts(); //Habilita interrup��es

}
//Rotina para gravar na Flash
void write_flash(int endereco, char value)
{
	char *Flash_ptr; //Aloca ponteiro

	_disable_interrupts(); //Desabilita interrup��es
	FCTL2 = FWKEY + FSSEL0 + 20; //Senha, Fonte de CLK, Fator div CLK
	FCTL3 = FWKEY + LOCKA;
	FCTL1= FWKEY + WRT;
	Flash_ptr=(char *)0xFA00 + endereco; //Aponta para end. no segmento A
	*Flash_ptr=value; //Coloca valor no end
	FCTL1= FWKEY; //Senha
	FCTL3 = FWKEY + LOCKA + LOCK; //
	_enable_interrupts(); //Habilita interrup��es
}

//Rotina para ler conte�do da Flash
char read_flash(int endereco)
{
	char *Flash_ptr; //Aloca ponteiro
	Flash_ptr=(char *)0xFA00 + endereco; //Aponta para valor
	return(*Flash_ptr); //Retorna valor apontado
}

