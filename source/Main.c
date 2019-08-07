/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Main.c
*   Author:         Huang Yuqi
*   Description:    Main functions
*   
*   Date: 1/11/2012
*
******************************************************************************/

#include <string.h>
#include <absacc.h>
#include <math.h>
#define MAIN_C
#define GDATA_C

#include "FIBER_IDENTIFIER.h"
#include "Gdata.h"
#include "ADCDrv.h"
#include "Display.h"
#include "FreqReader.h"
#include "Timer.h"
#include "Battary.h"
#include "Task.h"
#include "KeyDrv.h"


#define DEBUG_VER  0

extern void vInitICReg(void);
void DebugTest();

//--------------------------------Timer	 
 tUINT16 xdata   wTmrA2D;		  // ADC Timer
 tUINT16 xdata   wTmr2;		  	  //  Timer
 tUINT16 xdata   wTmr3;		  	  //  Timer
 tUINT16 xdata   wTmrUART;		  // CalibrationMode ADC Timer


void main(void)
{
	/// Initialization
    DisableWatchDog_();
    DisableGlobalInterrupts_();

 	InitTask();

	// default display "Low" power value
	vDisplayDrvEnable( TRUE );
	DisplayLED( DIG_1, CHAR_LO, 1 );

	// wait for user release the power key.
	P0 |= 0x80;
	while ( KEY_F_POWER == 0 )
	{
		// wait for user release power key
	}


#ifdef HAS_FREQ
	EIP1 = 0x60;	/// The Freq. counter(Comparator 0) INT should have higher priority than the ADC's.
	//FreqReaderEnable( FALSE );
#endif

    EnableGlobalInterrupts_();    

    wTmrA2D = wGetTimer();
    wTmr2 = wGetTimer();
    wTmr3 = wGetTimer();
    wTmrUART = wGetTimer();
		

//    EnableWatchDog_(); // 42ms WD Interval.
    TriggerWatchDog = FALSE; 

	/// Start ADC tasks	
	StartTasks( TRUE );

    FOREVER                                                                
    {   
		#if 1
		if ( sInCalibrationMode == 0 )
		{
			#ifdef HAS_FREQ
	        if ( tTimeout( TIMER_500ms, &wTmrA2D ) )
			{
				FreqProc();
			} 
			#endif

	        if ( tTimeout( TIMER_500ms, &wTmr2 ) )
			{
				if ( gSystemState.myBattState == BattaryLow )
					TriggerDisplaySparkle();
			} 

	        if ( tTimeout( TIMER_100ms, &wTmr3 ) )
			{
				FuncKeyProc();
			} 
		}
		#endif

		#ifdef HAS_UART
	    {
		    if ( tTimeout( TIMER_1s, &wTmrUART ) )
			{
				SendRangeInfo();
			} 
		}
		#endif
				


 //       FEED_DOG_();
	

		//================================================
		// Debug section
		//================================================
		#if DEBUG_VER
		DebugTest();
		#endif
    }
}




#if DEBUG_VER
void DebugTest()
{
	static char aCnt = 0, temp;
//   	LONGDATA data rawValue;
//	float power = 0;
	
	#if 0
	EnableADC( FALSE );
    if ( tTimeout( TIMER_1s, &wTmrSPK ) )
	{
		temp = aCnt;
		DisplayLED(0, temp, 1);
		aCnt++;
		if ( aCnt > 17 ) aCnt = 0;
	} 
    #endif			
	
	#if 1
        if ( tTimeout( TIMER_1s, &wTmrSPK ) )
		{
			temp = 1 << aCnt;
			DisplayLED(0, temp, 0);     ///led
			DisplayLED(1, aCnt, 0);  ///signal
	        DisplayLED(2, aCnt, 0 );    /// 10
	        DisplayLED(3, aCnt, 1 ); /// 1
			aCnt++;
			if ( aCnt >= 17 ) aCnt = 0;
		} 

    #endif			

	#if 0
    if ( tTimeout( TIMER_1s, &wTmrA2D ) )
	{
		ADC0MD  = 0x80;
		AD0INT = 0;                         // Clear ADC0 conversion complete flag
		ADC0MUX = 0x68;
		ADC0MD  = 0x83;
		
			rawValue.Byte[Byte3] = 0x00;
			rawValue.Byte[Byte2] = (unsigned char)ADC0H;
			rawValue.Byte[Byte1] = (unsigned char)ADC0M;
			rawValue.Byte[Byte0] = (unsigned char)ADC0L;	
			power = rawValue.result / 6710.0 * 2; 
			OnSampling( rawValue.result );
			power = GetBattaryPower();

			if ( power < 2200 )
			{
				SetDisplayType(Disp_DynamicDEC);
			}
			else
			{
				SetDisplayType(Disp_Static);
			}

	
		TriggerDisplaySparkle();
	} 
    #endif			

	#if 0
    if ( tTimeout( TIMER_1s, &wTmrSPK ) )
	{
		aFreqCnt = GetCurFreqCount();
		if ( aFreqCnt < 200 ) DisplayLED(0, 10, 1); // led off
		else if ( aFreqCnt < 290 ) DisplayLED(0, 1, 1);
		else if ( aFreqCnt < 400 ) DisplayLED(0, 2, 1);
		else if ( aFreqCnt < 1200 ) DisplayLED(0, 3, 1);
		else       DisplayLED(0, 4, 1);

		ResetFreqReaderParam();
	} 

    #endif			
}
#endif


