/*****************************************************************************
* 
*   Copyright Photonic Ltd. (C) 2011
*   All rights reserved.
*
*   Module Name:    Speaker.h
*   Author:
*   Description:    Speaker(Beeper) API. 
*                   The speaker use timer 2 as speaker pulse.
*   
*   Author:  Huang Yu Qi
*   Date: 26/11/2011
*
******************************************************************************/

#ifndef SPEAKER_H
#define SPEAKER_H

#include "Typedefs.h"

typedef enum
{
	AlarmStartup = 0,
	AlarmPowerLow,
	AlarmPowerHigh,
	AlarmFreqSwitch,
	AlarmBattaryLow
}eAlarmType;

typedef struct
{
   tUINT16	myBeepCount;	///< Beep count
   tUINT16	myFrequency;	///< Frequency
   tUINT16 	myInterval; 	///< Unit: ms
   tUINT16 	myElapse; 		///< Unit: ms
   tINT16 	myTimesPerBeep; 	///< 
   tINT16 	myTimesPerInterval; 	///< 
} stSpeaker;


sbit SPK = P1^0; //P0^7;  		//·äÃùÆ÷¿ØÖÆÐÅºÅ



/// HW APIs
void 	SpearkerInitHW();
void 	EnableSpeaker( eBOOL argEnable  );


/// Application APIs
eBOOL 	InitSpeaker( );
void 	Beep( tUINT16 argCount, tUINT16 argFreq, tUINT16 argElapse/*ms*/, tUINT16 argInterval/*ms*/ );

void    Alarm( eAlarmType argAlarmType );
void 	EmergencyAlarm(tUINT8 bOn );

#endif  // end of SPEAKER_H