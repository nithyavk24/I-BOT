#include <p18f46k22.h>    
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "ProjectMain.h"  
#include "serial_codes.h"
#include "ADC.H" 
#include "PinAllocation.h"
#include "LCD.h"
#include "delay.h"
#include "timers.h"
unsigned int gunTemperature=0;
unsigned char uchBuff[8];
unsigned char guchLCDLine1String[17],guchLCDLine2String[17];
char gchTempbuffer[10];
unsigned char uchBuffDisplay[] = "APSIS SOLUTIONS\n\r";
unsigned char uchBuffDisplay1[];



void fnMain_Project()
{
	TRISC = 0xFF;
//	ANSELC = 0x00;
	PORTC = 0x00;
	
	TRISA = 0x00;
	ANSELA = 0x00;
	LATA = 0x00;
	
 	while(1)
	{
	
		
		while(RLPBit1==1)
		{
			fnTrafficSide1ControlON();
		}
		while(RLPBit2==1)
		{
			fnTrafficSide2ControlON();
		}
		while(RLPBit3==1)
		{
			fnTrafficSide3ControlON();
		}
		while(RLPBit4==1)
		{
			fnTrafficSide4ControlON();
		}
		fnTrafficSide1ControlON();
		delay_in_seconds(2);
		fnTrafficSide2ControlON();
		delay_in_seconds(2);
		fnTrafficSide3ControlON();
		delay_in_seconds(2);
		fnTrafficSide4ControlON();
		delay_in_seconds(2);
						
	}	
		
}		
	
void fnTrafficSide1ControlON(void)
{
		LATAbits.LATA0 = 1;    // Even Pins are Green
		LATAbits.LATA2 = 0;
		LATAbits.LATA4 = 0;
		LATAbits.LATA6 = 0;
		
		LATAbits.LATA1 = 0;		// Even Pins are Red
		LATAbits.LATA3 = 1;
		LATAbits.LATA5 = 1;
		LATAbits.LATA7 = 1;
}	
	
void fnTrafficSide2ControlON(void)
{
		LATAbits.LATA0 = 0;
		LATAbits.LATA2 = 1;
		LATAbits.LATA4 = 0;
		LATAbits.LATA6 = 0;
		
		LATAbits.LATA1 = 1;
		LATAbits.LATA3 = 0;
		LATAbits.LATA5 = 1;
		LATAbits.LATA7 = 1;
}	

void fnTrafficSide3ControlON(void)
{
		LATAbits.LATA0 = 0;
		LATAbits.LATA2 = 0;
		LATAbits.LATA4 = 1;
		LATAbits.LATA6 = 0;
		
		LATAbits.LATA1 = 1;
		LATAbits.LATA3 = 1;
		LATAbits.LATA5 = 0;
		LATAbits.LATA7 = 1;
}		
		
void fnTrafficSide4ControlON(void)
{
		LATAbits.LATA0 = 0;
		LATAbits.LATA2 = 0;
		LATAbits.LATA4 = 0;
		LATAbits.LATA6 = 1;
		
		LATAbits.LATA1 = 1;
		LATAbits.LATA3 = 1;
		LATAbits.LATA5 = 1;
		LATAbits.LATA7 = 0;
}	
	