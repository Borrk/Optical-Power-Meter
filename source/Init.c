/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Init.c
*   Author:
*   Description:    Initialize Module 
*   
*   $Workfile: Init.c $
*   $Author:  $
*   Date: 9/11/2011
*
******************************************************************************/

#include <string.h>
#include <absacc.h>
#include <intrins.h>

#include "FIBER_IDENTIFIER.h"
#include "Gdata.h"
#include "Speaker.h"
#include "FreqReader.h"
#include "DisplayDrv.h"
#include "Timer.h"
#include "ADCDrv.h"
#include "Uart.h"

//-----------------------------------------------------------------------------
// Resource assign
//-----------------------------------------------------------------------------
/*
. Timer0 as system clock tick (2ms).


. Input:
  Lamda 1: AIN0.0
  Lamda 2: AIN0.1
  Lamda 3: AIN0.2
  Lamda 4: AIN0.3
  Battary    : AIN0.5
  Laser Power: AIN0.6
  Frequency  : P1.4
  Comparor Ref. Voltage: P1.5
  Unit key: P1.6
  Ref  key: P1.7
  Power key pressed: P0.7

. Output:
  

*/

//-----------------------------------------------------------------------------
// vInitOscillator
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This routine initializes the system clock to use the internal 24.5MHz
// oscillator as its clock source.  Also enables missing clock detector reset.
//
//-----------------------------------------------------------------------------
void vInitOscillator(void)
{
    // oscillator initialization
    OSCICN = 0x83;  // internal 24.5MHz. 
}

//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports
//
//-----------------------------------------------------------------------------
void vInitGPIO(void)
{

    XBR0 = 0x00;    // disable UART0
    XBR1 = 0x00;
    
    P2MDOUT = 0xff; // Pushpull 

    P0 = 0x84;
    P1 = 0xff;		
    P2 = 0xff;

    XBR1 = 0x40; // Enable Cross Bar
}


//----------------------------------------------------
//             MCU  Initialization
//----------------------------------------------------

void vInitICReg(void)
{
    vInitOscillator(); // initialize oscillator: internal 24.5MHz.
 
    // HW initialization
    vInitGPIO();
  
	/// Init Timer0 and Timer1
    TimerInitHW();
 
    TH0_BYTE = TIMER0_2ms_HIGHBYTE;
    TL0_BYTE = TIMER0_2ms_LOWBYTE;

	/// Timer 0 as the tick
	/// Timer 1 as UART baud clock source(Mode2)
    CKCON = 0; 		// Timer0&1 uses the system clock divided by 12. it's default
    TMOD = 0x21; 	// Timer0 in Mode1(16bit mode) for system tick, Timer 1 in mode 2(for UART baud)
  
    TH0 = TH0_BYTE;
    TL0 = TL0_BYTE;
    ET0 = 1; 	// enable Timer1 interrupts.
    PT0 = 1; 	// It's in high Priority
	TR0=1;		// Timer0 enabled

    TH1 = BAUD_9600;
    //ET1 = 1; 	// enable Timer1 interrupts.
    PT1 = 0; 	// It's in Low Priority
	TR1=1;		// Timer1 enabled

    XBR1 = 0x40;    // XBARE=1: Crossbar Enable

	vADC0DrvInitHW();
	vDisplayDrvInitHW();
	FreqReaderInitHW();

#ifdef HAS_UART
	UartInitHW();
#endif
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
