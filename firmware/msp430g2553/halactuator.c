//******************************************************************************
//Interface MSP430G2553 e MCP4131.							   
//Author: Danilo Pena
//Descrição: Comunicação por SPI com o MCP4131 (slave) com o MSP430g2553,
//MCP4131: 7 bit Single SPI Digital POT   
//						   
//
//MCLK = DCO/8 = (1MHz/8) = 125KHz
//SMCLK = DCO/8 (1MHz/8) = 125KHz
//				
//******************************************************************************

#include "halactuator.h"
//#include "haltest.h"

/*
//Função Main
void main(void){
	
	ConfigWDT();
	ConfigClocks();

	atua(0x01);     // Função para testar o comando write que escolhe a resistencia desejada, o bit argumento representa a resistencia entre os terminais W e B.
	// 0x00 -> resistencia de  110 Ohm (Rwb);
	// 0x64 -> resistencia de 9240 Ohm (Rab);
	incrementar();
	// Função para testar o comando para incrementar a resistencia aumentando um bit na região de memoria 0000.
	decrementar();
	// Função para testar o comando para decrementar a resistencia diminuindo um bit na região de memoria 0000.


	__bis_SR_register(LPM0_bits + GIE);       	// CPU off, enable interrupts	

	while(1);
}
*/

unsigned int calcularValor(unsigned char atuacao){
	//unsigned char pode ser de 0x00 a 0x64
	//Valor maximo do atuador 9240 -> 0x64
	//Valor mínimo do atuador  110 -> 0x00
	// N = (Rwb - Rw)*128/Rab
	// Rw = 110;
	// Rab = 9240;

	// Rwb (Ohm) = 91.3 (Ohm/%) * PorcetagemAtuacao (%) + 110 (Ohm)
	// N = [(91.3 * PorcetagemAtuacao (%) + 110) - 110]*128/9240
	// N = [(91.3 * PorcetagemAtuacao (%)]*0.01385
	// N = [(91.3 * PorcetagemAtuacao (%)]*0.01385
	// N = [1.264 * PorcetagemAtuacao (%)]
	return (128*atuacao)/100;
}

void HalAtuar(unsigned char valor){
	HalConfigPortsSPI();
	HalSpiOpen();
	CS_Low;
	HalSpiWrite(writeTCON1);
	HalSpiWrite(writeTCON2);

	HalSpiWrite(0x00);  // Comando para escrever na região de memória 0000, comando write 00, os outros 00 são XX (comando de 16 bits)
	HalSpiWrite(calcularValor(valor));

	CS_High;

	HalSpiClose();
}

void HalIncrementar(){
	HalConfigPortsSPI();
	HalSpiOpen();
	CS_Low;
	HalSpiWrite(writeTCON1);
	HalSpiWrite(writeTCON2);

	HalSpiWrite(0x04);  // Comando para escrever na região de memória 0000, comando incremento 01, os outros 00 são XX (comando de 16 bits)

	CS_High;

	HalSpiClose();
}

void HalDecrementar(){
	HalConfigPortsSPI();
	HalSpiOpen();
	CS_Low;
	HalSpiWrite(writeTCON1);
	HalSpiWrite(writeTCON2);

	HalSpiWrite(0x08);  // Comando para escrever na região de memória 0000, comando incremento 01, os outros 00 são XX (comando de 16 bits)

	CS_High;

	HalSpiClose();
}

//Função de configuração das saídas
void HalConfigPortsSPI(void){
	P1OUT |= CS;
	P1DIR |= CS + BIT7 + BIT5;
	P1SEL |= BIT7 + BIT5;
	P1SEL2 |= BIT7 + BIT5;
}

/*Função de inicialização do módulo USCI
UCA0CTL0: Configuração do registo
BIT7	Data is changed on the first UCLK edge and captured on the following edge.
BIT6	The inactive state is high.
BIT5	MSB first.
BIT4	8-bit data.
BIT3	Master mode.
BIT2-1	3-pin SPI.
BIT0	Synchronous mode*/
void HalSpiOpen(void){
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 |= UCCKPL + UCMSB + UCMST + UCMODE0 + UCSYNC;
	UCB0CTL1 |= UCSSEL_2;				//Clock Source SMCLK = DCO/8 (1MHz/8) = 125KHz
	UCB0BR0 |= 0x02;                          	// /2
	UCB0BR1 = 0;                              	//

	UCB0CTL1 &= ~UCSWRST;
	IE2 &= ~UCB0TXIE;                          	// Disable USCI0 RX interrupt*/
}

//Função de transmissão de dados
void HalSpiWrite(unsigned char Data){
	UCB0TXBUF = Data;                     		// Transmit first character
	while (!(IFG2 & UCB0TXIFG));             	// USCI_A0 TX buffer ready?
	IFG2 &= ~UCB0TXIFG;				//Clear the TX flag
	__delay_cycles(5);				//Add some delay between transmissions
}          

//Função de desactivação do módulo USCI
void HalSpiClose(void){
	UCB0CTL1 |= UCSWRST;
}
