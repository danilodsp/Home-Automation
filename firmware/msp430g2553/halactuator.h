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

#ifdef MSPGCC
#include <legacymsp430.h>
#endif
#include <msp430g2553.h>

//Macros
//#define CS BIT5
#define CS BIT4
#define CS_Low P1OUT &= ~CS
#define CS_High P1OUT |= CS

#define inc 0x08        // Comando para incrementar a resistência entre os terminais B e W (comando de 8bits)
#define writeTCON1 0x40 // Comando para escrever na região de memoria 04 (TCON, ver datasheet), para definir quais são os 
                        // terminais a serem conectados (comando 16bits)
#define writeTCON2 0x0F // Restando do comando anterior, para este CI só interessa os 4 ultimos bits, pois os primeios 
                        // são apra um segundo potenciometro que não existe no MCP4131. O F significa que todos os 
                        // terminais estão conectados, e que o W não está no modo SHUTDOWN, ver datasheet.

//Prototipos
void HalConfigPortsSPI(void);
void HalAtuar(unsigned char valor);
void HalSpiOpen(void);
void HalSpiClose(void);
void HalSpiWrite(unsigned char data);
void HalIncrementar();
void HalDecrementar();
