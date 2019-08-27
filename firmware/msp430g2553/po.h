/*
 * po.h
 *
 *  Created on: 05/06/2012
 *      Author: Danilo
 */

#ifndef PO_H_
#define PO_H_

// Par�metros fixos
#define tipoSensor 0x01 // Categoria do sensor
#define tipoAtuador 0x00 // Categoria do atuador
#define timeout 5000 // Timeout de espera dos estados de RX
#define initTime 5000 // Delay para inciar os estados (ms)
#define nSample 64 // N�mero de amostras para c�lculo da m�dia (valor m�ximo recomendado = 64)

// Flags
#define flagDebug 				(0x0001)
#define flagReceived 			(0x0002)
#define flagPeriodo 			(0x0004)
#define flagConfigSensor 		(0x0008)
#define flagConfigAtuador 		(0x0010)
#define flagError 				(0x0020)
#define flagAlarme				(0x0040)
#define flagChecksum			(0x0080)
#define flagConfig				(0x0100)

// Verificar flags
#define fDebug					(flags&flagDebug)
#define fReceived				(flags&flagReceived)
#define fPeriodo				(flags&flagPeriodo)
#define fConfigSensor			(flags&flagConfigSensor)
#define fConfigAtuador			(flags&flagConfigAtuador)
#define fError					(flags&flagError)
#define fAlarme					(flags&flagAlarme)
#define fChecksum				(flags&flagChecksum)
#define fConfig					(flags&flagConfig)

// LEDs
#define LED1					P1OUT|=BIT0
#define LED2					P1OUT|=BIT6
#define xLED1					P1OUT^=BIT0
#define xLED2					P1OUT^=BIT6
#define dLED1					P1OUT&=~BIT0
#define dLED2					P1OUT&=~BIT6

// Error
#define ERROR_SEM_TIPO			(0x0001) // Firmware sem tipoSensor nem tipoAtuador cadastrado
#define ADDRESS_NOT_FOUND		(0x0002) // Address Not Found Status de um frame type 8B
#define ERROR_FRAME				(0x0003) // Erro no frame recebido
#define ERROR_COMMAND			(0x0004) // Erro por receber comando errado (Ex: um comando 0x02 quando deveria ser um 0x04)

#include "halatuador.h"
#include "MdE.h"
#include "config.h"  // Arquivos de configuração da camada HAL !!
#include <msp430g2553.h>

void init(); // Inicializa��o das variaveis
void lerMem(); // Ler mem�ria flash
char configurarAddress(); // Envia frame configurarAddress do protocolo, retorna 0 se obter um verificarStatusEnvio com sucesso
char armazenandoID(); // Armazena o ID sensor e ID atuador recebido do m�dulo central, retorna diferente de 0 caso o checksum do frame recebido esteja errado
char configurarIS();
void configurarSensor(); // Configura os par�metros do sensor
void configurarPeriodo(); // Configurar o periodo do xBee para sleep mode
void medicaoPeriodica(); // Envia as medi��es em um certo per�odo
char enviarMedicao(int valorMedido); // Envia por UART resultado das medicoes
void calcularMedia(); // Calcula media dos resultados das medicoes
char verificarStatusEnvio(unsigned int comando); // Verifica os frames de status de envio (8B), retorna 0 se houve sucesso no envio
unsigned char checksum(unsigned char *frameT); // Calcula o checksum de um frame (frameT) com n�mero de bytes igual a qtd
unsigned char checksumP(unsigned char *frameT, int posIni, int posFin); // Calcula o checksum com a POsi��o do byte inicial e final
void debug(unsigned char *frameT); // Envia um frame (frameT) para a UART
void timer(int ms); // Esse timer tamb�m desliga a CPU, somente retornando ao acabar o valor ms
void timer_us(int us); // Timer em microsegundos
void configurarAtuador(); // Configura os par�metros do atuador
void atuar(); // Comando atuar
char waitReceive(); // Esperar o flagReceived, retorna 1 se recebeu dados na serial e 0 se n�o recebeu nada
void receive(); // Recebe dados da UART (frameRx)
void send(); // Envia dados pela UART (frameTx)
char verificarChecksum(); // Verifica o checksum, retornando 0 se houve sucesso
void waitTimeout();
void erro(int erroValor); // Atribuir erro
//void PO_emitirErro(); // Emite por um LED o erro


#endif /* PO_H_ */
