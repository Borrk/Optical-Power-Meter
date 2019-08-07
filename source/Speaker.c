/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Speaker.c
*   Author:
*   Description:    Speaker(Beeper) API. 
*   
*   Author:  Huang Yu Qi
*   Date: 26/11/2011
*
******************************************************************************/

//#include <stdio.h>
#include <string.h>
#include <absacc.h>
#include <intrins.h>

#include "Hw.h"
#include "Speaker.h"


 stSpeaker idata sSpeakerInfo;
 tUINT8    idata sIsEmergencyAlarm;	///< flag for auto repeat alarm when the alarm count to 0

/*-----------------------------------------------------------------------------------*\
 *
 *  HardWare initilization section.
 *
 *  Use Timer 2 as speaker pulse, timer clock = SYSCLK/12, tick = 250us; 
 *  works in auto-reload mode.
\*-----------------------------------------------------------------------------------*/
 
void SpearkerInitHW()
{
	SPK = 0;

	/// Use Timer 2 as speaker pulse 
   	CKCON &= ~0x60;                     // Timer2 uses SYSCLK/12
   	TMR2CN &= ~0x01;

   	TMR2RL = (0x10000-0.25*SYSCLK_HZ/1000/12);  // Reload value(250us) to be used in Timer2
   	TMR2 = TMR2RL;                      // Init the Timer2 register

   	ET2 = 1;                            // Timer2 interrupt enabled
}

static void vConfigSpeakerHW( tUINT16 argFreq )
{	
   	TMR2CN = 0x0;                      		// Disable Timer2 in auto-reload mode
   	TMR2RL = 0x10000-(SYSCLK_HZ/argFreq/12);// Reload value to be used in Timer2
   	TMR2 = TMR2RL;                      	// Init the Timer2 register
 
   	//TMR2CN = 0x04;                      	// Enable Timer2 in auto-reload mode
}

void EnableSpeaker( eBOOL argEnable  ) 
{
	if ( TRUE == argEnable )
	{
 	  	TMR2CN = 0x04;                      // Enable Timer2 in auto-reload mode
	}
	else
	{
 	  	TMR2CN = 0x00;                      // Disable Timer2 in auto-reload mode
	}

	SPK = 0;
}

void vInt_TIM2 (void) interrupt 5
{
	if ( --sSpeakerInfo.myTimesPerBeep >= 0 )
	{
		SPK ^= 1;   // Toggle the Speaker
	}
	else
	{
		if ( --sSpeakerInfo.myTimesPerInterval >= 0 )
		{
		/// do nothing, just during elapse
		}
		else
		{
			if ( --sSpeakerInfo.myBeepCount > 0 )
			{
				sSpeakerInfo.myTimesPerBeep = sSpeakerInfo.myElapse / (1000/sSpeakerInfo.myFrequency);

				if ( sSpeakerInfo.myBeepCount > 0 )
					sSpeakerInfo.myTimesPerInterval = sSpeakerInfo.myInterval / (1000/sSpeakerInfo.myFrequency);			

				/// for emergency mode, increase the counter automatically
				if ( sIsEmergencyAlarm == 1 )
					sSpeakerInfo.myBeepCount++;
			}
			else
			{
				EnableSpeaker( FALSE );
				SPK = 0;
			}
		}
	}

   	TF2H = 0;	// Reset Interrupt
}

/*-----------------------------------------------------------------------------------*\
 *
 *  API section.
 *
\*-----------------------------------------------------------------------------------*/

eBOOL InitSpeaker( )
{
	/// Initialize beep information
	sSpeakerInfo.myBeepCount 	= 0;
	sSpeakerInfo.myFrequency 	= 0;
	sSpeakerInfo.myElapse 		= 0;
	sSpeakerInfo.myInterval	 	= 0;
	sSpeakerInfo.myTimesPerBeep = 0;
	sSpeakerInfo.myTimesPerInterval = 0;

	/// Initialize HW 
	//SpearkerInitHW();

	sIsEmergencyAlarm = 0;

	return TRUE;
}

void  Beep( tUINT16 argCount, tUINT16 argFreq, tUINT16 argElapse/*ms*/, tUINT16 argInterval/*ms*/ )
{
	/// When in emergency alarm state, the normal one will be ignored!!!
	if ( sIsEmergencyAlarm == 1 )
		return;

	EnableSpeaker( FALSE );

	sSpeakerInfo.myBeepCount = argCount;
	sSpeakerInfo.myFrequency = argFreq;
	sSpeakerInfo.myElapse	 = argElapse;
	sSpeakerInfo.myInterval	 = argInterval;
	sSpeakerInfo.myTimesPerBeep = argElapse / (1000/argFreq/12);
	sSpeakerInfo.myTimesPerInterval = argInterval / (1000/argFreq/12);
   
    vConfigSpeakerHW( argFreq );

	EnableSpeaker( TRUE );
}


void Alarm( eAlarmType argAlarmType )
{
	switch ( argAlarmType )
	{
	case AlarmStartup:
		Beep( 1, 4000, 2000, 200 );
		break;

	case AlarmPowerLow:
		Beep( 2, 2000, 500, 200 );
		break;

	case AlarmPowerHigh:
		Beep( 2, 2000, 500, 200 );
		break;

	case AlarmFreqSwitch:
		Beep( 1, 4000, 500, 200 );
		break;

	case AlarmBattaryLow:
		Beep( 2, 4000, 500, 200 );
		break;
	}
}

void EmergencyAlarm(tUINT8 bOn )
{
	if ( bOn )
	{
		if ( sIsEmergencyAlarm == 0 )
		{
			Beep( 10, 4000, 500, 500 );
			sIsEmergencyAlarm = 1;
		}
	}
	else if ( sIsEmergencyAlarm == 1 )
	{
		sIsEmergencyAlarm = 0;
		Beep( 0, 0, 0, 0 );
	}
}