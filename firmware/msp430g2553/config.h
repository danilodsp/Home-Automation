/*
 * config.h
 *
 *  Created on: 31/05/2012
 *      Author: Danilo
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <msp430g2553.h>

void config();					// Configuração do MSP430
void configCLK();				// Configura clock
void configWDT();				// Configura watchdog
void configPORT();				// Configura PORT
void configTIMER();				// Configuração do Timer
void configUART();				// Configuração da UART
void configADC();				// Configuração do AD
void startADC(int pin);			// Inicializa o AD para um pino de 0 a 7
void initSensor(int tipo);		// Configura um sensor através do tipoSensor
void initAtuador(int tipo);		// Configura um atuador através do tipoAtuador

#endif /* CONFIG_H_ */
