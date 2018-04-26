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
#include "ApsGSMSIM900.h"
#include "EEPROM.h"
#include "GPS_NMEA.h"
#include "main.h"


extern char gchmessagepacket[],gchphonenumberpacket[],gchCompleteGSMDataPacket[];
extern char gchResponseFromMODEM[],gchGSMPhoneNumber[];
extern unsigned char guchFlagForSmsTobeSent;
extern char gchVBGSMPhoneNumber[];
extern char gchGPSDataPacket[];
extern unsigned char  guchGPSLatitudearray[], guchGPSLongitudearray[];
extern unsigned char guchN_S_Indicator,  guchE_W_Indicator;
unsigned char guchAuthorizedOTP[5];
unsigned char guchMessageCount=0;

unsigned int gnRxCount,gnRequiredCount;
char gchTempbuffer[50],gchTempbufferMessageToBeSent[100] ;
unsigned int gnStatusOfInbox;
unsigned char guchFlagToSendSmsOnce=0;
unsigned char guchValueToDisplay[10]="     ";
unsigned char rfid_card_no[16];
unsigned char rfid_count;
unsigned char uchBuff[30];
unsigned char gchGSMPhoneNumber1[14];
unsigned char uchflagforCard=0,uchflagATMDeactivate;
unsigned char gchPhNo1[14]="+918553334620",gchPhNo2[14]="+918553334620",gchPhNo3[14]="+918553334620";

unsigned char guchValueFromRFID[13];
unsigned char guchStringFromRFID[15];
int i;
#pragma udata udata3
unsigned char guchLCDLine1String[17],guchLCDLine2String[17];
int nFlagForElectronicLock=0,nGSMMsgReceivedStatus=0;
unsigned int a;
unsigned int guchAlcoholSensing;
unsigned char guchAlcoholSensingChar;
unsigned char uchSerialData;
unsigned char DataReceiveFromZIGBEE[4];

unsigned char guchFlagSendSMSOnce=0;

void fnMain_Project(void)
{	
	Initialisation();
//		fnModuleSelect(SELECT_ZIGBEE_ON_UART);
//		fn_Get_GPS_Coordinates();
		
	while(1)
	{
	 	guchAlcoholSensing = unfnReadADCChannel(CHANNEL_3,ADC_10BIT_MODE);	//Connect to RA3
	 	strcpypgm2ram(gchTempbufferMessageToBeSent,"outside AlcoholSensing \n\r");
		fnUARTPutString(gchTempbufferMessageToBeSent,UART2);	// Debug
	 	while(guchAlcoholSensing < 75)
	 	{	
		 while(1)
		 {	
		 	strcpypgm2ram(gchTempbufferMessageToBeSent,"inside AlcoholSensing \n\r");
			fnUARTPutString(gchTempbufferMessageToBeSent,UART2);	// Debug	
			fnMotorStartStopControl();
			fnAlcoholSense();
			
			if(HighWaySwitch==1)
			{
				fnUltraSonicSensor();
			}	
			
			Delay_in_ms(100);
			fnSpeedControlOFMotor();
			Delay_in_ms(100);
		 }
		}

	}
}

void fnSpeedControlOFMotor(void) 
{
	fnModuleSelect(SELECT_ZIGBEE_ON_UART);
	Delay_in_ms(50);
	uchSerialData=uchfnReceive_Serial(UART2);
	while(uchSerialData != '*')
	{
		uchSerialData=uchfnReceive_Serial(UART2);
	}
	; // Wait infinitely for the * Value
	//if(uchSerialData=='*')
	{		
		for(i=0;i<3;i++)
		DataReceiveFromZIGBEE[i]=uchfnReceive_Serial(UART2);
		DataReceiveFromZIGBEE[i]=0;
	}
	//fnModuleSelect(SELECT_PC_ON_UART);
	fnUARTPutString(DataReceiveFromZIGBEE,UART2);
	fnModuleSelect(SELECT_GPS_ON_UART);	
	
	if(DataReceiveFromZIGBEE[2]=='0')
	{
		Honk = 0;
	}
	else
	{
		Honk = 1;
	}	
	
	if(DataReceiveFromZIGBEE[0]=='1')
	{
		strcpypgm2ram(guchLCDLine1String,"  SCHOOL ZONE   ");
		strcpypgm2ram(guchLCDLine2String,"  DRIVE SLOWLY  ");
		fnDisplayLCD16X2();
		Buzzer =1;
		delay_in_half_seconds(2);
		Buzzer =0;
		if(DataReceiveFromZIGBEE[1]=='1')
		{
			Motor1 = 1;
			Control1	= 1;
			Control2	= 0;
			Control3	= 0;
			delay_in_seconds(5);	
		}
		else if(DataReceiveFromZIGBEE[1]=='2')
		{
			Motor1 = 1;
			Control1	= 0;
			Control2	= 1;
			Control3	= 0;
			delay_in_seconds(5);
		}
		else if(DataReceiveFromZIGBEE[1]=='3')
		{
			Motor1 = 1;
			Control1	= 0;
			Control2	= 0;
			Control3	= 1;
			delay_in_seconds(5);
		}
		else
		{
			Motor1 = 1;
			Control1	= 0;
			Control2	= 0;
			Control3	= 1;
		}
		delay_in_seconds(5);
	}
	if(DataReceiveFromZIGBEE[0]=='2')
	{
		strcpypgm2ram(guchLCDLine1String,"  HOSPITAL ZONE ");
		strcpypgm2ram(guchLCDLine2String,"  DRIVE SLOWLY  ");
		fnDisplayLCD16X2();
		Buzzer =1;
		delay_in_half_seconds(2);
		Buzzer =0;
		
		if(DataReceiveFromZIGBEE[1]=='1')
		{
			Motor1 = 1;
			Control1	= 1;
			Control2	= 0;
			Control3	= 0;
			delay_in_seconds(5);
		}
		else if(DataReceiveFromZIGBEE[1]=='2')
		{
			Motor1 = 1;
			Control1	= 0;
			Control2	= 1;
			Control3	= 0;
			delay_in_seconds(5);
		}
		else if(DataReceiveFromZIGBEE[1]=='3')
		{
			Motor1 = 1;
			Control1	= 0;
			Control2	= 0;
			Control3	= 1;
			delay_in_seconds(5);
		}
		else
		{
			Motor1 = 1;
			Control1	= 0;
			Control2	= 0;
			Control3	= 1;
			
		}
		delay_in_seconds(5);
		
		
	}
	fnCLearingZigBeeData();
}

void fnCLearingZigBeeData(void)
{
	for(i=0;i<5;i++)
	{
		DataReceiveFromZIGBEE[i] = 0;
	}
}

void fnMotorStartStopControl(void)
{
	if(MotorTurnONSWITCH==1)
	{
		Motor1 = 1;
		Motor2 = 1;
		Control1	= 0;
		Control2	= 0;
		Control3	= 1;
	}
	else
	{
		Motor1 = 0;
		Motor2 = 0;
	}	
}

void fnUltraSonicSensor(void)
{
	printf("TOCON = %0x     ",T0CON);    	
	LATBbits.LATB1 = 1;
	Pulse();
	LATBbits.LATB1 = 0;
	TMR0L = TMR0H =0x00;
	while(!PORTBbits.RB0);           	//Waiting for Echo
	T0CONbits.TMR0ON = 1;             	//Timer Starts
	while(PORTBbits.RB0);            	//Waiting for Echo goes LOW
	T0CONbits.TMR0ON = 0;               //Timer Stops
	printf("TL= %d    ",TMR0L);
	printf("TH= %d    ",TMR0H);
	a = (TMR0L | (TMR0H*256));   		//Reads Timer Value
//	printf("Basic value A = %d cm\r\n",a);
	a = a/58.82;                		//Converts Time to Distance
	a = a * 2;
	a = a + 1;
	
	if((a>=2)&&(a<=400))          //Check whether the result is valid or not
	{
		Motor1 = 0;
		Motor2 = 0;
	}	
	strcpypgm2ram(guchLCDLine1String,"OBJECT DISTANCE ");
	sprintf(guchLCDLine2String,"      %04d Cm   ",a);
	fnDisplayLCD16X2();
	delay_in_half_seconds(2);
	strcpypgm2ram(gchTempbuffer,"\n\r Range Sensing = ");
	strcat(gchTempbuffer,guchLCDLine2String);
	fnUARTPutString(guchLCDLine2String,UART2);	// Debug	
}

void Initialisation(void)
{
	Dir_HighWaySwitch 		= SET_INPUT;
	Dir_Motor1				= SET_OUTPUT;
	Dir_Motor2				= SET_OUTPUT;
	Dir_MotorTurnONSWITCH	= SET_INPUT;
	Dir_Honk				= SET_OUTPUT;
	Dir_Buzzer				= SET_OUTPUT;
	
	ANS_HighWaySwitch 		= SET_DIGITAL;
	ANS_Motor1 				= SET_DIGITAL;
	ANS_Motor2 				= SET_DIGITAL;
	ANS_MotorTurnONSWITCH	= SET_DIGITAL;
	ANS_Honk				= SET_DIGITAL;
	ANS_Buzzer				= SET_DIGITAL; 
	
	Motor1 = 0;
	Motor2 = 0;
	Honk = 1;
	Buzzer = 0;
	
	Dir_Control1 = SET_OUTPUT;
	Dir_Control2 = SET_OUTPUT;
	Dir_Control3 = SET_OUTPUT;
	
	ANS_Control1 = SET_DIGITAL;
	ANS_Control2 = SET_DIGITAL;
	ANS_Control3 = SET_DIGITAL;
	
	Control1	= 0;
	Control2	= 0;
	Control3	= 0;
	
	TRISBbits.TRISB0 	= 1;
	ANSELBbits.ANSB0 	= 0;
	TRISBbits.TRISB1 	= 0;
	ANSELBbits.ANSB1 	= 0;
	T0CON				= 0x00;
	
}

void Pulse(void)
{
	INTCONbits.TMR0IF =0;
//	T0CON	= 0x00;
	TMR0L	= 0xFB;
	TMR0H	= 0xFF;
	T0CONbits.TMR0ON = 1;
	printf("TOCON pulse = %0x     ",T0CON);
	while(INTCONbits.TMR0IF);
	T0CONbits.TMR0ON = 0;
	INTCONbits.TMR0IF =0;
	
}	

void fnAlcoholSense(void)
{

	guchAlcoholSensing = unfnReadADCChannel(CHANNEL_3,ADC_10BIT_MODE);	//Connect to RA3
	
	Delay_in_ms(500);	

	if((guchAlcoholSensing>100)&& (guchFlagSendSMSOnce==0))
	{
		strcpypgm2ram(guchLCDLine1String,"Alcohol Detected");
		strcpypgm2ram(guchLCDLine2String,"                ");
		fnDisplayLCD16X2();
		guchFlagSendSMSOnce=1;
		fn_Get_GPS_Coordinates();
		delay_in_half_seconds(6);  // Delay after Alcohol sensing to vehicle stop.
		Motor1 = 0;
		while(1);
			
	}
	else
	{
		guchFlagSendSMSOnce=0;
	
	}
	
	sprintf(guchLCDLine1String,"ALCOHOL     %04d",guchAlcoholSensing);
	strcpypgm2ram(guchLCDLine2String,"                ");
	fnDisplayLCD16X2();
	delay_in_half_seconds(2);
	fnUARTPutString(guchLCDLine1String,UART2);	// Debug
}

void fnModuleSelect(unsigned char uchModuleSelect)
{
	
	switch(uchModuleSelect)
	{

		case SELECT_GPS_ON_UART:
					M_SELECT0	= 1;
					M_SELECT1	= 1;
					break;
		case SELECT_ZIGBEE_ON_UART:
					M_SELECT0	= 0;
					M_SELECT1	= 0;
					break;
		case SELECT_PC_ON_UART:
					M_SELECT0	= 0;
					M_SELECT1	= 1;
					break;

		default:
					M_SELECT0	= 0;
					M_SELECT1	= 0;
					break;
	}
}
	
void fnCheckInboxStatus(void)
{
	gnStatusOfInbox = fnnCheckInboxStatus();
	strcpypgm2ram(gchTempbuffer,"Inbox Checked \n\r");
	fnUARTPutString(gchTempbuffer,UART2);
	switch(gnStatusOfInbox)
	{
		case NEWMESSAGE:
			strcpypgm2ram(gchTempbuffer,"\n\r NEW Message Present in INBOX");
			fnUARTPutString(gchTempbuffer, UART2);
			fnExtractPhoneNoAndMessage();
			strcpypgm2ram(gchTempbuffer,"\n\r Phone Number = ");
			fnUARTPutString(gchTempbuffer, UART2);
			strcpy(gchTempbuffer,gchphonenumberpacket);
			fnUARTPutString(gchTempbuffer, UART2);
			strcpypgm2ram(gchTempbuffer,"\n\r Message = ");
			fnUARTPutString(gchTempbuffer, UART2);
			strcpy(gchTempbuffer,gchmessagepacket);
			fnUARTPutString(gchTempbuffer, UART2);
			strcpypgm2ram(gchTempbuffer,"\n\r");
			fnUARTPutString(gchTempbuffer, UART2);
			fnDecryptCommandsInSMS();
			break;
		case NONEWMESSAGE:
			strcpypgm2ram(gchTempbuffer,"\n\r NO new message in inbox");
			fnUARTPutString(gchTempbuffer, UART2);	
			strcpypgm2ram(gchTempbuffer,"\n\r Phone Number = ");
			fnUARTPutString(gchTempbuffer, UART2);	
			strcpy(gchTempbuffer,gchphonenumberpacket);
			fnUARTPutString(gchTempbuffer, UART2);
			strcpypgm2ram(gchTempbuffer,"\n\r Message = ");
			fnUARTPutString(gchTempbuffer, UART2);
			strcpy(gchTempbuffer,gchmessagepacket);
			fnUARTPutString(gchTempbuffer, UART2);
			strcpypgm2ram(gchTempbuffer,"\r\n");
			fnUARTPutString(gchTempbuffer, UART2);
			break; 
		case INBOXEMPTY:
			strcpypgm2ram(gchTempbuffer,"\n\r INBOX EMPTY\r\n");
			fnUARTPutString(gchTempbuffer, UART2);
			break; 
		default:
			strcpypgm2ram(gchTempbuffer,"\n\r ERROR Reading Status");
			fnUARTPutString(gchTempbuffer, UART2);
			break; 						
	}
	
	strcpypgm2ram(gchTempbuffer,"\n\r Inbox Status Checked and Message Decoded if any present \n\r");
	fnUARTPutString(gchTempbuffer, UART2);
	fnnDeleteAllMessages();
	strcpypgm2ram(gchTempbuffer,"\n\r All Messages Deleted \n\r");
	fnUARTPutString(gchTempbuffer, UART2);			
}

void fnDecryptCommandsInSMS(void)
{

	
}

void fnCleargchmessagepacket(void)
{
	unsigned char i;
	for(i=0; i<150; i++)
	{
		gchmessagepacket[i]= 0;
		gchTempbufferMessageToBeSent[i]=0;
		gchphonenumberpacket[i]=0;
	}
}

void fnTempbufferMessageToBeSent(void)
{
	unsigned char i;
	for(i=0; i<150; i++)
	{
		gchTempbufferMessageToBeSent[i]=0;
	}
}

void fn_Get_GPS_Coordinates(void)
{
	fnModuleSelect(SELECT_GPS_ON_UART);
	Delay_in_ms(100);
	delay_in_half_seconds(1);

	fnDecodeGPRMC();
	fnDecodeGPRMC();
	fnDecodeGPRMC();
	fnDecodeGPRMC();
	fnDecodeGPRMC();
	fnDecodeGPRMC();
	fnDecodeGPRMC();
	fnDecodeGPRMC();
	INTCONbits.GIE = 1;    // ### Enable Global interrupt				
	delay_in_half_seconds(1);
//	fnModuleSelect(SELECT_PC_ON_UART);
	Delay_in_ms(100);

	strcpypgm2ram(gchTempbufferMessageToBeSent,"ATTENTION : ALCOHOL SENSED and the Co Ordinates are = ");
	strcat(gchTempbufferMessageToBeSent,gchGPSDataPacket);
	fnUARTPutString(gchTempbufferMessageToBeSent, UART2);
	delay_in_half_seconds(1);		
	fnGSMSendMessage(gchGSMPhoneNumber,gchTempbufferMessageToBeSent);
//	fnUARTPutString(gchGPSDataPacket,UART2);
	fnClearTempbufferMessageToBeSent();	
	delay_in_half_seconds(1);	
}

void fnDisplayLCD16X2(void)
{
	fn_lcd_select_line_and_location(LCD_LINE_1,LOCATION_0);
	fn_Display_String_LCD(guchLCDLine1String);
	fn_lcd_select_line_and_location(LCD_LINE_2,LOCATION_0);
	fn_Display_String_LCD(guchLCDLine2String);

}

void fnClearTempbufferMessageToBeSent(void)
{
	unsigned char i;
	for(i=0; i<100; i++)
	{
		gchTempbufferMessageToBeSent[i]=0;
	}
}
