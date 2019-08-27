/*
 * config.c
 *
 *  Created on: 31/05/2012
 *      Author: Danilo
 */

#include "config.h"

void config()
{
 	configWDT();
 	configCLK();
 	configPORT();
	configUART();
	configTIMER();
	configADC();
}

void configCLK()
{
	BCSCTL1 = CALBC1_1MHZ;					// Configuração do DCO para 1.0 MHz
	DCOCTL = CALDCO_1MHZ;
	BCSCTL2 = 0x00;
}

void configWDT()
{
	WDTCTL = WDTPW + WDTHOLD;				// Desliga watchdog
}

void configPORT()
{
	P1DIR |= BIT0 + BIT1 + BIT2 + BIT6;		// Configuração de IO
	P1OUT = 0x00;							// Reset pinos

	// Configuracão de interrupção
	/* P1IE |= 0x08; 						// Interrupção P1.3
	P1IES = 0x00;							// Low-to-High
	P1IFG &= ~0x08;							// Flag P1.3 clear */
}

void configTIMER()
{
	TACTL = TASSEL_2 + MC_1;		// SMCLK e Mode
}

void configUART()
{
	P1SEL = BIT1 + BIT2 ;           // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ;          // P1.1 = RXD, P1.2=TXD
	UCA0CTL0 = 0x00;
	UCA0CTL1 |= UCSSEL_2;           // CLK = SMCLK // CLK = ACLK
	UCA0BR0 = 0x68;                 // 1MHz/9600 = 104 // 32kHz/9600 = 3.41 0x03
	UCA0BR1 = 0x00;                 //
	UCA0MCTL = UCBRS0;              // Modulation UCBRSx = 1 // UCBRSx = 3 UCBRS1 + UCBRS0
	UCA0CTL1 &= ~UCSWRST;           // Initialize USCI state machine
}

void configADC()
{
	ADC10CTL0 |= ADC10SHT_2 + ADC10ON + ADC10IE; 		// ADC10ON, interrupt enabled
	ADC10CTL1 |= ADC10SSEL_1 + ADC10DIV_5;     			// ACLK, DIV/6
}

void startADC(int pin)
{
	switch(pin){
		case 0:
			ADC10AE0 |= BIT0;
			ADC10CTL1 |= INCH_0;
			break;
		case 1:
			ADC10AE0 |= BIT1;
			ADC10CTL1 |= INCH_1;
			break;
		case 2:
			ADC10AE0 |= BIT2;
			ADC10CTL1 |= INCH_2;
			break;
		case 3:
			ADC10AE0 |= BIT3;
			ADC10CTL1 |= INCH_3;
			break;
		case 4:
			ADC10AE0 |= BIT4;
			ADC10CTL1 |= INCH_4;
			break;
		case 5:
			ADC10AE0 |= BIT5;
			ADC10CTL1 |= INCH_5;
			break;
		case 6:
			ADC10AE0 |= BIT6;
			ADC10CTL1 |= INCH_6;
			break;
		case 7:
			ADC10AE0 |= BIT7;
			ADC10CTL1 |= INCH_7;
			break;
	}

	ADC10CTL0 |= ENC + ADC10SC;
}

void initSensor(int tipo)
{
	if(tipo==0x01){
		//tipoMedida = 0;
	}
}

void initAtuador(int tipo)
{
	if(tipo==0x01){ // Relé
		P1DIR |= BIT3;
	}
}
