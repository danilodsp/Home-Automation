
//******************************************************************************
//  MSP430G25353
//  Abril 2012
//  Built with CCS Version 4.2.0
//******************************************************************************

#include "msp430g2553.h"
#include "display.h"
#include <math.h>

#define Qamostras 150

//******************************************************************************
						//Declaração de variáveis

int y[Qamostras],intervalo=0, cont=0;
int i;
unsigned long int x=0;
float Vrms=0, Irms=0, p=0, Va=0, Vrmsfinal[3]={0,0,0};
float consumo=0, Va2=0;

//******************************************************************************



//********************************************************************************************************************
	  //Cálculo de tensão, corrente e pontencia RMS, bem como o consumo de acordo com dados da Coser(26/04/2012)

void calculo(){
	x=0;

	for(i=0;i<Qamostras;i++){
		x=(x+pow(y[i],2));							//Soma quadrática das amostras para posterior média(RMS)
	}

	Vrms=(sqrt((x/Qamostras))*(3.6/1023));                 //Valor da tensão RMS, considerando os níveis de quantização
	if(Vrms<0.01){
		Vrms=0;
	}
	Va=floor(((Vrms-floor(Vrms))*100));
	Vrms=floor(Vrms)+Va/100;
	Vrmsfinal[cont]=Vrms;


	if(cont==3||cont>3){
		Va2=(Vrmsfinal[0]+Vrmsfinal[1]+Vrmsfinal[2])/3;
		Va=floor(((Va2-floor(Va2))*100));
		Va2=floor(Va2)+Va/100;
		if(Vrms<Va2){
			Vrms=Va2;
		}
		Irms=((Vrms-1.8053)/0.185);				  	//Valor da corrente RMS

		if(Irms<=0.02){
			Irms=0;
			consumo=0;
		}
		p=220*Irms;
		consumo=consumo+(p/1000)*0.34472; 			   	//Cálculo do consumo para consumidores tarifados no grupo B da Cosern
		cont=0;
	}
cont++;
}
//********************************************************************************************************************


//********************************************************************************************************************
	  		//Função que mostrará os dados no display, está função utiliza a biblioteca display.h
void atualizalcd(){

	ClearLcmScreen();
	printg("POTYCHIP"," ");
	__delay_cycles(1800000);
	ClearLcmScreen();

	printg("CORRENTE: ", convertefloat(Irms));
	__delay_cycles(1800000);
	ClearLcmScreen();

	printg("POTENCIA: ", convertefloat(p));
	__delay_cycles(1800000);
	ClearLcmScreen();

	printg("CONSUMO: ", convertefloat(consumo));
	__delay_cycles(1800000);
	ClearLcmScreen();
}
//********************************************************************************************************************


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                		 	// Stop WDT
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	ADC10CTL0 |= ADC10SHT_3 + ADC10ON + ADC10IE; 		// ADC10ON, interrupt enabled
	ADC10CTL1 |= INCH_1 + ADC10SSEL_3 + ADC10DIV_7;     // input A1
	ADC10AE0 |= 0x02;                        	  		// PA.1 ADC option select

	CCTL0 = CCIE;                             			// CCR0 interrupt enabled
	CCR0 = 8333;
	TACTL = TASSEL_2 + MC_1;                  	// ACLK, contmode
	_BIS_SR(GIE);                 // Enter LPM0 w/ interrupt

	P1DIR |= 0x01;                            			// Set P1.0 to output direction
	P2DIR |= 0x01;

	InitializeLcm();

	for(;;){
 		ADC10CTL0 |= ENC + ADC10SC;             		// Sampling and conversion start
		TACCTL0 |= CCIE;
	}
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void){
  	if(intervalo==0){
		for(i=0;i<Qamostras;i++){
			y[i]=0;									   //Somando as amostras para posterior média(RMS)
		}
	}

  y[intervalo] = ADC10MEM;				//Preenchendo um vetor de 10 elementos de amostra do sinal
  intervalo++;
}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void){
	P2OUT^= 0x01;


	if(intervalo==Qamostras||intervalo>Qamostras){
		calculo();
		intervalo=0;
	}
	
	if(cont==3||cont>3){
		//atualizalcd();
	}
	//TACCTL0 &= ~CCIE;
	CCTL0 &= ~CCIE;
	//CCR0=50000;
}
