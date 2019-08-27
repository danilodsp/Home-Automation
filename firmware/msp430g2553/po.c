/*
 * po.c
 *
 *  Created on: 05/06/2012
 *      Author: Danilo
 */

#include "po.h"


int IDsensor; // ID do sensor
int IDatuador; // ID do atuador

extern uint8_t  MdE_sinal; // Sinal emitido para máquina de estados
int periodo; // Período de envio dos dados
char hasAlarme; // Se tem alarme é igual a 1
int alarmeValue; // Valor do alarme
char dadoAtuador; // Parâmetro de configurarAtuador
unsigned char frameTx[36]; // Frame a ser enviado
unsigned char frameRx[32]; // Frame recebido
int cont; // Contador para frameTx
int contRx; // Contador para frameRx
char comandoAPI; // Comando do xBee em modo API recebido
int error; // Armazena o error status
int flags;
/*
 * flagDebug - Habilitado caso deseje enviar um frame qualquer pela UART - 0x0001
 * flagReceived - Habilitado se estiver recebido o frame esperado do atual estado - 0x0002
 * flagPeriodo - 0: 500ms, 1: 1min - 0x0004
 * flagConfigSensor - Habilitado se já houve configurarSensor ou configurarAtuador - 0x0008
 * flagConfigAtuador - Habilitado se já houve configurarAtuador - 0x0010
 * flagError - Houve um erro - 0x0020
 * flagAlarme - Se a medição alcançou o valor de alarme dado a regra de hasAlarme - 0x0040
 * flagChecksum - Se o checksum verificado está correto - 0x0080
 * flagConfig - Habilitado se já houve configuração dos periféricos - 0x0100
 */
int valorTimer; // Valor a ser carregado no CCR0 do Timer
int valorADC[nSample]; // Resultado do ADC
int contADC; // Contador do ADC
long int valorMedio; // Resultado da média das medidas
char valorAtuar; // Estado de atuação, Para relé 0x00: OFF, 0x01: ON

// Interrupcao UART_RX
#ifdef MSPGCC
interrupt(USCIAB0RX_VECTOR)  USCI0RX_ISR (void)
#else
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#endif
{
	frameRx[contRx] = UCA0RXBUF;
	contRx = contRx + 1;
	if((contRx>2)&&((contRx-3)>frameRx[2])){ // Verificando Length (acabou o frame)
		comandoAPI = frameRx[3];
		IE2 &= ~UCA0RXIE;
		flags |= flagReceived;
		_BIC_SR_IRQ(CPUOFF);
	}
}

// Interrupcao UART_TX
#ifdef MSPGCC
interrupt (USCIAB0TX_VECTOR)  USCI0TX_ISR(void)
#else
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#endif
{
	if(cont < (frameTx[2]+4)){
		UCA0TXBUF = frameTx[cont];
		cont = cont + 1;
	}
	else{
		IE2 &= ~UCA0TXIE;
		//_BIC_SR_IRQ(CPUOFF);
	}
}

// Interrupcao de Timer
#ifdef MSPGCC
interrupt(TIMER0_A0_VECTOR) Timer_A (void)
#else
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
#endif
{
	CCTL0 &= ~CCIE; // CCR0 interrupt enabled
	_BIC_SR_IRQ(CPUOFF);
}

// Interrupção do ADC10
#ifdef MSPGCC
interrupt(ADC10_VECTOR) ADC10_ISR(void)
#else
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#endif
{
	valorADC[contADC] = ADC10MEM;

	if(hasAlarme&BIT0){ // Se tem alarme
		if(hasAlarme&BIT1){ // Se alarme atua para valores maiores que alarmeValue
			if(valorADC[contADC] > alarmeValue){
				enviarMedicao(valorADC[contADC]);
			}
		}
		else{ // Se alarme atua para valores menores que alarmeValue
			if(valorADC[contADC] < alarmeValue){
				enviarMedicao(valorADC[contADC]);
			}
		}
	}

	contADC++;

	if(contADC>(nSample-1)){
		contADC = 0;
		calcularMedia();
	}	ADC10CTL0 |= ENC + ADC10SC;
}

void PO_LerMem(){
	init();

	lerMem();

	// Verificação do cadastro do sensor e atuador
	if(fConfig==0x00){
		MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
	}
	else if(((tipoSensor)&&((IDsensor==0x00)|(fConfigSensor==0x00))) || ((tipoAtuador)&&((IDatuador==0x00)|(fConfigAtuador==0x00)))){
		MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_INICIO;
	}
	else if(((tipoSensor)&&(IDsensor)&&(fConfigSensor)) && ((tipoAtuador)&&(IDatuador)&&(fConfigAtuador))){
		MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_INICIO;
	}
	else if((tipoSensor)&&(IDsensor)){
		MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_DO_INICIO;
	}
	else if((tipoAtuador)&&(IDatuador)){
		MdE_sinal = PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_INICIO;
	}
	else if((tipoSensor==0x00)&&(tipoAtuador==0x00)){
		MdE_sinal = PASSANDO_PARA_ERROR_DO_INICIO;
		erro(ERROR_SEM_TIPO);
	}
}

void PO_ConfigurarAddress()
{
	//init();
	int status;
	status = configurarAddress();
		
	if(fConfig==0x00){
		MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
	}
	else if(status){
		MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS;
	}
	else{
		MdE_sinal = PASSANDO_PARA_AGUARDAR_CONFIGURAR_ADDRESS_DO_CONFIGURAR_ADDRESS;
		}
}

void PO_ArmazenandoID()
{
	//init();
	int status;
	status = armazenandoID();

	if(fConfig==0x00){
		MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
	}
	else if(status==0x01){
		MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_AGUARDAR_CONFIGURAR_ADDRESS;
	}
	else if(status==0x02){
		MdE_sinal = PASSANDO_PARA_ERROR_DO_AGUARDAR_CONFIGURAR_ADDRESS;
	}
	else if(status==0x03){
		MdE_sinal = PASSANDO_PARA_CONFIGURAR_IS_DO_AGUARDAR_CONFIGURAR_ADDRESS;
	}
}

void PO_ConfiguraIS()
{
	//init();
	int status;
	status = configurarIS();

	if(fConfig==0x00){
		MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
	}
	else if((status==0x00)&&(tipoSensor)&&(tipoAtuador)){
		MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_E_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS;
	}
	else if((status==0x00)&&(tipoSensor)){
		MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_DO_CONFIGURAR_IS;
	}
	else if((status==0x00)&&(tipoAtuador)){
		MdE_sinal = PASSANDO_PARA_AGUARDAR_COMANDO_ATUAR_DO_CONFIGURAR_IS ;
	}
	else if(status==0x01){
		MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_CONFIGURAR_IS;
	}
	else if(status==0x02){
		MdE_sinal = PASSANDO_PARA_CONFIGURAR_IS_DO_CONFIGURAR_IS;
	}
	else{
		MdE_sinal = PASSANDO_PARA_ERROR_DO_CONFIGURAR_IS;
	}
}

void PO_EnviarMedicao()
{
	//init();
	int status;
	status = enviarMedicao(valorMedio);

	if(fConfig==0x00){
		MdE_sinal = PASSANDO_PARA_INICIO_D_QQ_ESTADO;
	}
	else if(status==0x02){
		MdE_sinal = PASSANDO_PARA_CONFIGURAR_ADDRESS_DO_ENVIAR_MEDICAO;
	}
	else if(status==0x00){
		MdE_sinal = PASSANDO_PARA_ENVIAR_MEDICAO_DO_ENVIAR_MEDICAO;
	}
	else{
		MdE_sinal = PASSANDO_PARA_ERROR_DO_ENVIAR_MEDICAO;
	}
}

void PO_Atuar()
{

}

void PO_EnviarMedicaoComAtuacao()
{

}

void PO_Error()
{

}

void init()
{
	cont = 0;
	contRx = 0;
	error = 0;
	flags = 0;
	contADC = 0;

	flags |= flagConfig;
	config();
	_BIS_SR(GIE);
}

void lerMem()
{
	IDsensor = 0x00;
	IDatuador = 0x00;

	// Ler configuração sensor e atuador
	flags |= flagConfigSensor;
	//flags |= flagConfigAtuador;

	periodo = 0x0F;
	hasAlarme = 0x00;
	alarmeValue = 0x03F0;
	dadoAtuador = 0x00;
}

void receive()
{
	flags &= ~flagReceived;
	contRx = 0;
	IE2 |= UCA0RXIE;
	_BIS_SR(CPUOFF + GIE);
}

void send()
{
	cont = 0;
	IE2 |= UCA0TXIE;
}

char configurarAddress()
{
	// Construcao do frame para enviar o comando 0x01
	frameTx[0] = 0x7E; // Start Delimiter
	frameTx[1] = 0x00; // Length
	frameTx[2] = 0x11;
	frameTx[3] = 0x10; // Frame Type
	frameTx[4] = 0x54; // Frame ID
	frameTx[5] = 0x00; // Address 64-bit
	frameTx[6] = 0x00;
	frameTx[7] = 0x00;
	frameTx[8] = 0x00;
	frameTx[9] = 0x00;
	frameTx[10] = 0x00;
	frameTx[11] = 0x00;
	frameTx[12] = 0x00;
	frameTx[13] = 0xFF; // Address 16-bit
	frameTx[14] = 0xFE;
	frameTx[15] = 0x00; // No de hops
	frameTx[16] = 0x00;
	frameTx[17] = 0x01; // Dados: Comando
	frameTx[18] = tipoSensor; // Tipo Sensor
	frameTx[19] = tipoAtuador; // Tipo Atuador
	frameTx[20] = checksum(frameTx); // Checksum

	send(); // 21 bytes

	receive(); // Recebe 8B
	if(verificarStatusEnvio(0x8B))
		return 1;
	else
		return 0;
}

char armazenandoID()
{
	receive();

	if(verificarChecksum()){
		//LED1;
		return 1;
	}

	if((frameRx[0]==0x7E)&(frameRx[15]==0x01)){
		IDsensor = frameRx[17];
		IDatuador = frameRx[18];
	}
	else{
		erro(ERROR_FRAME);
		return 1;
	}

	if((IDsensor==0x00) && (IDatuador==0x00)){
		return 2;
	}
	else if((tipoSensor)&&(tipoAtuador)&&(IDsensor==0x00 || IDatuador==0x00)){
		return 2;
	}
	else if((tipoSensor)&&(IDsensor==0x00) || (tipoAtuador)&&(IDatuador==0x00)){
		return 2;
	}
	else{
		return 3;
	}
}

char verificarStatusEnvio(unsigned int comando)
{
  if((frameRx[0]==0x7E)&&(frameRx[3]==comando)){ // Frame status
	  
		if(comando==0x8B){
			if(frameRx[8]==0x00){ // Success
				return 0;
			}
			else if(frameRx[8]==0x24){ // Address Not Found
				erro(ADDRESS_NOT_FOUND);
				return 24;
			}
			else{
				return 1;
			}
		}
		else if(comando==0x88){ // AT Command Response
			if(frameRx[7]==0x00){
				// ...
			}
		}
	}
  
	return 1;
}

char configurarIS()
{
	receive();
	if(verificarChecksum()){
		//LED1;
		return 1;
	}

	if((frameRx[0]==0x7E)&(frameRx[15]==0x02)){
		IDsensor = frameRx[17];
		configurarSensor();
	}
	else if((frameRx[0]==0x7E)&(frameRx[15]==0x04)){
		IDatuador = frameRx[18];
		configurarAtuador();
	}


	if((tipoSensor && fConfigSensor) && (tipoAtuador && fConfigAtuador)){
		return 0;
	}
	else if((tipoSensor && fConfigSensor) || (tipoAtuador && fConfigAtuador)){
		return 0;
	}
	else if(fConfigSensor || fConfigAtuador){
		return 2;
	}
	else{
		return 3;
	}
}

void configurarSensor()
{
	flags |= flagConfigSensor;

	if(frameRx[16]==IDsensor){ // Verifica o comando do protocolo
		periodo = frameRx[17];
		hasAlarme = frameRx[18];
		alarmeValue = (frameRx[19] << 8) + frameRx[20];
	}
	else{
		erro(ERROR_FRAME);
	}

	flags |= (periodo>>7)<<2; // 10000000 -> 00000100, flagPeriodo
	/*
	// Transformando o periodo em 10*ms
	periodo = periodo & ~0x80; // Eliminando a flagPeriodo do valor do período
	if(flags & flagPeriodo){ // Se o período é multiplo de 1min
		periodo = periodo * 6000;
	}
	else{ // Se o período é multiplo de 500ms
		periodo = periodo * 50; // Será modificado para uma rotina de multiplicação mais simples ...
	}*/
	periodo = 0x64;
}

void configurarAtuador()
{
	flags |= flagConfigAtuador;

	if(frameRx[16]==IDatuador){ // Verifica o comando do protocolo
		dadoAtuador = frameRx[17];
	}
	else{
		erro(ERROR_FRAME);
	}
}

unsigned char checksum(unsigned char *frameT)
{
	int soma = 0;
	int i = 3;
	for(i=3;i<(frameT[2]+4);i++){
		soma += frameT[i];
	}

	return (0xFF - soma);
}

unsigned char checksumP(unsigned char *frameT, int posIni, int posFin)
{
	int soma = 0;
	int i = posIni;
	for(i=posIni;i<(posFin);i++){
		soma += frameT[i];
	}

	return (0xFF - soma);
}

void debug(unsigned char *frameT)
{
	int i = 0;
	for(i=0;i<(frameT[2]+4);i++){
		frameTx[i] = frameT[i];
	}
	flags |= flagDebug;
	send(); // qtd bytes
}

void timer(int ms)
{
	//valorTimer = (ms<<1) + ms; // Utilizando VLO 12kHz/4 = 3kHz (ms<<1 + ms = ms*3)
	int i=0;
	for(i=0; i<ms; i++){
		timer_us(1000);
	}
}

void timer_us(int us)
{
	valorTimer = us; // * 500; // Utilizando SMCLK 1MHz/2 = 500kHz
	CCR0 = valorTimer;
	CCTL0 = CCIE; // CCR0 interrupt enabled
	_BIS_SR(CPUOFF+GIE);
}

void configurarPeriodo()
{
	// 7E 00 05 08 54 53 4D 04 FF 7E 00 04 08 54 57 52 FA 7E 00 06 08 54 53 50 00 64 9C 7E 00 04 08 54 57 52 FA

	frameTx[0] = 0x7E; // Start Delimiter
	frameTx[1] = 0x00; // Length
	frameTx[2] = 0x05;
	frameTx[3] = 0x08; // Frame Type
	frameTx[4] = 0x54; // Frame ID
	frameTx[5] = 0x53; // S
	frameTx[6] = 0x4D; // M
	frameTx[7] = 0x04; // 4
	frameTx[8] = checksum(frameTx); // Checksum

	frameTx[9] = 0x7E; // Start Delimiter
	frameTx[10] = 0x00; // Length
	frameTx[11] = 0x04;
	frameTx[12] = 0x08; // Frame Type
	frameTx[13] = 0x54; // Frame ID
	frameTx[14] = 0x57; // W
	frameTx[15] = 0x52; // R
	frameTx[16] = checksumP(frameTx, 9, 16); // Checksum

	frameTx[17] = 0x7E; // Start Delimiter
	frameTx[18] = 0x00; // Length
	frameTx[19] = 0x06;
	frameTx[20] = 0x08; // Frame Type
	frameTx[21] = 0x54; // Frame ID
	frameTx[22] = 0x53; // S
	frameTx[23] = 0x50; // P
	frameTx[24] = (periodo >> 8); // Período High
	frameTx[25] = (periodo & 0xFF); // Período Low
	frameTx[26] = checksumP(frameTx, 17, 26); // Checksum

	frameTx[27] = 0x7E; // Start Delimiter
	frameTx[28] = 0x00; // Length
	frameTx[29] = 0x04;
	frameTx[30] = 0x08; // Frame Type
	frameTx[31] = 0x54; // Frame ID
	frameTx[32] = 0x57; // W
	frameTx[33] = 0x52; // R
	frameTx[34] = checksumP(frameTx, 27, 34); // Checksum

	send(); // 35 bytes

	//timer(1000);
	//verificarStatusEnvio(0x88);
}

void calcularMedia()
{
	int i;
	valorMedio = 0;

	for(i=0;i<nSample;i++){
		valorMedio += valorADC[i];
	}

	valorMedio = (valorMedio / nSample);
	//valorMedio = valorMedio >> 7; // Dividindo o valor por 128
}

void medicaoPeriodica()
{
	while(1){
		//xLED1;
		timer(2000);
		enviarMedicao(valorMedio);
	}
}

char enviarMedicao(int valorMedido)
{
	char byteAlarme = 0x00;
	if(fAlarme){
		byteAlarme = 0x01;
	}

	// Construcao do frame para enviar as medições
	frameTx[0] = 0x7E; // Start Delimiter
	frameTx[1] = 0x00; // Length
	frameTx[2] = 0x13;
	frameTx[3] = 0x10; // frameTx Type
	frameTx[4] = 0x54; // frameTx ID
	frameTx[5] = 0x00; // Address 64-bit
	frameTx[6] = 0x00;
	frameTx[7] = 0x00;
	frameTx[8] = 0x00;
	frameTx[9] = 0x00;
	frameTx[10] = 0x00;
	frameTx[11] = 0x00;
	frameTx[12] = 0x00;
	frameTx[13] = 0xFF; // Address 16-bit
	frameTx[14] = 0xFE;
	frameTx[15] = 0x00; // No de hops
	frameTx[16] = 0x00;
	frameTx[17] = 0x03; // Dados: Comando
	frameTx[18] = IDsensor; // ID Sensor
	frameTx[19] = (valorMedido>>8); // Valor High
	frameTx[20] = (valorMedido & 0xFF); // Valor Low
	frameTx[21] = byteAlarme;
	frameTx[22] = checksum(frameTx); // Checksum

	send(); // 23 bytes

	receive(); // Receber 8B
	if(verificarStatusEnvio(0x8B))
		return 1;

	// TIMEOUT
	// waitTimeout();

	receive(); // Receber resposta do comando 0x03
	while(verificarChecksum()){
		receive();
	}

	if((frameRx[0]==0x7E)&(frameRx[15]==0x03)){
		// frameRx[16] indica sucesso ou falha do comando 0x03
		if(frameRx[16]==0x11){
			return 2;
		}
		else if(frameRx[16]==0x00){
			return 0;
		}
	}

	return 0;
}

void atuar()
{
	if((frameRx[0]==0x7E)&(frameRx[15]==0x05)&(frameRx[16]==IDatuador)){ // Verifica o comando do protocolo
		valorAtuar = frameRx[17];
	}

	if(tipoAtuador==0x01){
		//P1OUT |= ((valorAtuar&0x01)<<6); // ON, OFF em BIT6
		if(valorAtuar==0x01){
			//LED2;
		}
		else if(valorAtuar==0x00){
			//dLED2;
		}
	}
	else if(tipoAtuador==0x02){
		if(valorAtuar>0x64)
			valorAtuar = 0x64;

		HalAtuar(valorAtuar);
	}
}

char waitReceive()
{
	flags &= ~flagReceived;

	timer(timeout);
	if(!fReceived)
		return 0;
	else
		return 1;
}

void waitTimeout()
{

}

char verificarChecksum()
{
	int length;
	flags &= ~flagChecksum;
	length = frameRx[2]+4;

	if(frameRx[length-1] != checksum(frameRx)){
		flags |= flagChecksum;
		return 1;
	}
	else
		return 0;
}

void erro(int erroValor)
{
	//LED1;
	flags |= flagError;
	error |= erroValor;
}
