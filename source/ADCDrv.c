/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    DisplayDrv.c
*   Author:
*   Description:    Display on screen/LEDs.. API. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/23/2012
*
******************************************************************************/

#include "C8051F350.h"
#include "ADCDrv.h"
#include "Hw.h"

ADCProc data sADCProc = 0;

/*-----------------------------------------------------------------------------------*\
 *
 *  HardWare initilization section.
 *
 *  Used HW resource:
 *  - ADC0
\*-----------------------------------------------------------------------------------*/

void vADC0DrvInitHW()
{
   	// Use Internal Vref: 
    ADC0CN = 0x00; 					// Internal Bias Generator Off. Internal Reference Buffer Off. Gain = 1

    ADC0L = 0x00;
    ADC0M = 0x00;
    ADC0H = 0x00;

	// ADC0 voltage reference from VREFA pin.ADC0ÖÐ¶ÏÔ´Ñ¡Ôñ:SINC3ÂË²¨Æ÷
   	// Use internal Vref:
		ADC0CF = 0x00; 
   	ADC0CLK = (SYSCLK_HZ / MDCLK)-1;	// Generate MDCLK for modulator.
                                	// Ideally MDCLK = 2.4576MHz

   	// Program decimation rate for desired OWR
   	ADC0DEC = ((unsigned long) MDCLK / (unsigned long) (OWR) /
              (unsigned long) 128) - 1;

   	ADC0BUF = 0x00;              	// Turn off Input Buffers
   	ADC0MUX = 0x08;                 // Select AIN0.0

   	ADC0MD = 0x81;                  // Start internal calibration
   	while(AD0CALC != 1);            // Wait until calibration is complete

   	//EIE1   |= 0x08;                	// Enable ADC0 Interrupts
   	ADC0MD  = 0x80;               	// Enable the ADC0 (IDLE Mode)
}

/*-----------------------------------------------------------------------------------*\
 *
 *  API section.
 *
\*-----------------------------------------------------------------------------------*/

void vInitADC0Drv( )
{
}

void vResetCompleteFlag( )
{
	AD0INT = 0;    // Clear ADC0 conversion complete flag
}

void vADC0EnableInt( eBOOL argEnable )
{
	if ( argEnable )
   		EIE1   |= 0x08; // Enable ADC0 Interrupts
	else
		EIE1   &= ~0x08; // Disable ADC0 Interrupts
}

// Set ADC0 MUX
void vSetADC0MUX( eADCMux aMuxP, eADCMux aMuxN )
{
	tUINT8  temp = 0, temp2= 0;
	switch ( aMuxP )
	{
	   case ADC_AIN0_0: 
	   	temp = 0x00;
	   	break;
	   case ADC_AIN0_1:
	   	temp = 0x10;
	   	break;
	   case ADC_AIN0_2:
	   	temp = 0x20;
	   	break;
	   case ADC_AIN0_3:
	   	temp = 0x30;
	   	break;
	   case ADC_AIN0_4:
	   	temp = 0x40;
	   	break;
	   case ADC_AIN0_5:
	   	temp = 0x50;
	   	break;
	 case ADC_AIN0_6:
	   	temp = 0x60;
	   	break;
	   case ADC_AIN0_7:
	   	temp = 0x70;
	   	break;
	   case ADC_AIN0_TEMPE :
	   	temp = 0xf0;
	   	break;
	   case ADC_AGND  :
	   	temp = 0x80;   	
	}
	
	switch ( aMuxN )
	{
	   case ADC_AIN0_0: 
	   	temp2 = 0x00;
	   	break;
	   case ADC_AIN0_1:
	   	temp2 = 0x01;
	   	break;
	   case ADC_AIN0_2:
	   	temp2 = 0x02;
	   	break;
	   case ADC_AIN0_3:
	   	temp2 = 0x03;
	   	break;
	   case ADC_AIN0_4:
	   	temp2 = 0x04;
	   	break;
	   case ADC_AIN0_5:
	   	temp = 0x05;
	   	break;
	 case ADC_AIN0_6:
	   	temp2 = 0x06;
	   	break;
	   case ADC_AIN0_7:
	   	temp2 = 0x07;
	   	break;
	   case ADC_AIN0_TEMPE :
	   	temp2 = 0x0f;
	   	break;
	   case ADC_AGND  :
	   	temp2 = 0x08;
   	}
   	
   	ADC0MUX  = temp | temp2;
}

void vSetADC0Filter(eADCFilter filter )
{
	tUINT8  temp = ADC0CF;
	/// bit4 of ADC0CF
	if ( filter ==	ADC_FILTER_SINC3 )
		temp &= 0xef; 
	else //ADC_FILTER_FAST
		temp |= 0x10;
	
	ADC0CF = temp;	
}

eADCFilter vGetADC0Filter()
{
	tUINT8  temp = ADC0CF;
	
	return (eADCFilter)(temp&0x10);
}

void vADC0SetMode(eADCMode mode)
{
	tINT8  temp = ADC0MD;

	ADC0MD =  (temp&0xf8)|((tINT8)mode);
}

eADCMode vADC0GetMode()
{
	tUINT8  temp = ADC0MD;
	
	return (eADCMode)(temp&0x07);
}


/*-----------------------------------------------------------------------------------*\
 *
 *  INT section.
 *
\*-----------------------------------------------------------------------------------*/

void vSetADCCallback( ADCProc proc )
{
	sADCProc = proc;
}


void vInt_ADC0_EOC (void) interrupt 10
{
	if ( sADCProc )
	{
		sADCProc();
	}
}