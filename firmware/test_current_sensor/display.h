
//
// MSP430 LCD Code
//

#ifndef __msp430g2553_h__  // Change this header file to include the MSP430 uC you are using.
	#include <msp430g2553.h> // Header must only be included if none of the files linking to
	#include <stdio.h> // Header must only be included if none of the files linking to
	#define __msp430g2553_h__ // this file include it already
#endif


#define     LCM_DIR               P1DIR
#define     LCM_OUT               P1OUT

//
// Define symbolic LCM - MCU pin mappings
// We've set DATA PIN TO 4,5,6,7 for easy translation
//
#define     LCM_PIN_RS            BIT2          // P1.0
#define     LCM_PIN_EN            BIT3          // P1.1
#define     LCM_PIN_D7            BIT7          // P1.7
#define     LCM_PIN_D6            BIT6          // P1.6
#define     LCM_PIN_D5            BIT5          // P1.5
#define     LCM_PIN_D4            BIT4          // P1.4


#define     LCM_PIN_MASK  ((LCM_PIN_RS | LCM_PIN_EN | LCM_PIN_D7 | LCM_PIN_D6 | LCM_PIN_D5 | LCM_PIN_D4))

#define     FALSE                 0
#define     TRUE                  1

//
// Routine Desc:
//
// This is the function that must be called
// whenever the LCM needs to be told to
// scan it's data bus.
//
// Parameters:
//
//     void.
//
// Return
//
//     void.
//
void PulseLcm()
{
	//P1OUT=0;
    //
    // pull EN bit low
    //
    LCM_OUT &= ~LCM_PIN_EN;
    __delay_cycles(200);

    //
    // pull EN bit high
    //
    LCM_OUT |= LCM_PIN_EN;
    __delay_cycles(200);

    //
    // pull EN bit low again
    //
    LCM_OUT &= (~LCM_PIN_EN);
    __delay_cycles(200);
}



//
// Routine Desc:
//
// Send a byte on the data bus in the 4 bit mode
// This requires sending the data in two chunks.
// The high nibble first and then the low nible
//
// Parameters:
//
//    ByteToSend - the single byte to send
//
//    IsData - set to TRUE if the byte is character data
//                  FALSE if its a command
//
// Return
//
//     void.
//
void SendByte(char ByteToSend, int IsData)
{

    //
    // clear out all pins
    //
    LCM_OUT &= (~LCM_PIN_MASK);
    //
    // set High Nibble (HN) -
    // usefulness of the identity mapping
    // apparent here. We can set the
    // DB7 - DB4 just by setting P1.7 - P1.4
    // using a simple assignment
    //
    LCM_OUT |= (ByteToSend & 0xF0);

    if (IsData == TRUE)
    {
        LCM_OUT |= LCM_PIN_RS;
    }
    else
    {
        LCM_OUT &= ~LCM_PIN_RS;
    }

    //
    // we've set up the input voltages to the LCM.
    // Now tell it to read them.
    //
    PulseLcm();
     //
    // set Low Nibble (LN) -
    // usefulness of the identity mapping
    // apparent here. We can set the
    // DB7 - DB4 just by setting P1.7 - P1.4
    // using a simple assignment
    //
    LCM_OUT &= (~LCM_PIN_MASK);
    LCM_OUT |= ((ByteToSend & 0x0F) << 4);

    if (IsData == TRUE)
    {
        LCM_OUT |= LCM_PIN_RS;
    }
    else
    {
        LCM_OUT &= ~LCM_PIN_RS;
    }

    //
    // we've set up the input voltages to the LCM.
    // Now tell it to read them.
    //
    PulseLcm();
}


//
// Routine Desc:
//
// Set the position of the cursor on the screen
//
// Parameters:
//
//     Row - zero based row number
//
//     Col - zero based col number
//
// Return
//
//     void.
//
void LcmSetCursorPosition(char Row, char Col)
{

    char address;

    //
    // construct address from (Row, Col) pair
    //
    if (Row == 0)
    {
        address = 0;
    }
    else
    {
        address = 0x40;
    }

    address |= Col;

    SendByte(0x80 | address, FALSE);
}


//
// Routine Desc:
//
// Clear the screen data and return the
// cursor to home position
//
// Parameters:
//
//    void.
//
// Return
//
//     void.
//
void ClearLcmScreen()
{
	P1OUT=0;
    //
    // Clear display, return home
    //
    SendByte(0x01, FALSE);
    SendByte(0x02, FALSE);
    P1OUT=0;
}


//
// Routine Desc:
//
// Initialize the LCM after power-up.
//
// Note: This routine must not be called twice on the
//           LCM. This is not so uncommon when the power
//           for the MCU and LCM are separate.
//
// Parameters:
//
//    void.
//
// Return
//
//     void.
//
void InitializeLcm(void)
{
	P1OUT=0;
    //
    // set the MSP pin configurations
    // and bring them to low
    //
    LCM_DIR |= LCM_PIN_MASK;
    LCM_OUT &= ~(LCM_PIN_MASK);


    //
    // wait for the LCM to warm up and reach
    // active regions. Remember MSPs can power
    // up much faster than the LCM.
    //
    __delay_cycles(100000);


    //
    // initialize the LCM module
    //
    // 1. Set 4-bit input
    //
    LCM_OUT &= ~LCM_PIN_RS;
    LCM_OUT &= ~LCM_PIN_EN;

    LCM_OUT = 0x20;
    PulseLcm();

    //
    // set 4-bit input - second time.
    // (as reqd by the spec.)
    //
    SendByte(0x28, FALSE);

    //
    // 2. Display on, cursor on, blink cursor
    //
    SendByte(0x0E, FALSE);

    //
    // 3. Cursor move auto-increment
    //
    SendByte(0x06, FALSE);
}


//
// Routine Desc
//
// Print a string of characters to the screen
//
// Parameters:
//
//    Text - null terminated string of chars
//
// Returns
//
//     void.
//
void PrintStr(char *Text)
{

    char *c;

    c = Text;

    while ((c != 0) && (*c != 0))
    {
        SendByte(*c, TRUE);
        c++;
    }

}

void delay(char d)
{ // The Hallowed delay function

    int i=0;
	for(i = 0; i <2*d;i++);
}

void strobe(void)
{ // This is a small function which pulls the Enable pin of the LCM low for some time

   P1OUT=P1OUT & 0xF7; //Enable Low
   delay(50);
   //delay(200);
   P1OUT=P1OUT | 0x08; //Enable High
   delay(40);
   //delay(180);

}

void lcm_line2(void)
{

   // Jump to line 2 of LCM
   //HN
   P1OUT=0xCF;
   P1OUT=P1OUT & 0xFB;
   strobe();

   //LN
   P1OUT=0x0F;
   P1OUT=P1OUT & 0xFB;
   strobe();
   P1OUT=0;
}

//
// Routine Desc
//
// main entry point to the sketch
//
// Parameters
//
//     void.
//
// Returns
//
//     void.
//
//char str[17];
char str[17];
char* convertefloat(float value) {


int d1 = value;            // Get the integer part (678).
float f2 = value - d1;     // Get fractional part (0.01234567).
int d2 = (int)(f2 * 10);   // Turn into integer (123).
float f3 = f2 * 10 - d2;   // Get next fractional part (0.4567).
int d3 = (int)(f3 * 10);   // Turn into integer (4567).

sprintf(str, "%d.%d%d", d1, d2, d3);

return(str);

}




void printg(char* L1, char* L2)
{

    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer

//    InitializeLcm();
//
      ClearLcmScreen();

    __delay_cycles(1000);

	PrintStr(L1); 	//IMPRIME NA LINHA 1

    lcm_line2();		//FUNÇÃO QUE MOVE O CURSOR PARA A LINHA 2

    //y=26.7569;

    //PrintFloat(L2); 		//FUNÇÃO QUE CONVERTE UM FLOAT EM STRING

	PrintStr(L2);		//IMPRIME NA LINHA 2
    P1OUT=0;

}

//int main(void){
//
////convertefloat(valor); COM ESTA FUNÇÃO É POSSÍVEL RETORNAR STRINGS A PARTIR DE FLOAT
//printg("O RESULTADO E:", convertefloat(29.58));
//
//}

