/*
 * config.h
 *
 *  Created on: 31/05/2012
 *      Author: Danilo
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <msp430g2553.h>

void config();					// Configura��o do MSP430
void configCLK();				// Configura clock
void configWDT();				// Configura watchdog
void configPORT();				// Configura PORT
void configTIMER();				// Configura��o do Timer
void configUART();				// Configura��o da UART
void configADC();				// Configura��o do AD
void startADC(int pin);			// Inicializa o AD para um pino de 0 a 7
void initSensor(int tipo);		// Configura um sensor atrav�s do tipoSensor
void initAtuador(int tipo);		// Configura um atuador atrav�s do tipoAtuador

#endif /* CONFIG_H_ */
