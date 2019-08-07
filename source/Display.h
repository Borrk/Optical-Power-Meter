/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Display.h
*   Author:
*   Description:    Display API declaration. 
*   
*   Author:  Huang Yu Qi
*   Date: 04/01/2012
*
******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "DisplayDrv.h"


/// LED light bit definition
#define LED_UNIT_dBm   		0x02
#define LED_UNIT_dB   		0x04
#define LED_LAMDA1 			0x08    		
#define LED_LAMDA2     		0x10
#define LED_LAMDA3     		0x40
#define LED_LAMDA4     		0x80
#define LED_SIGNAL		    0x20 

#define LED_ALL         	0xFF
#define LED_LAMDA_ALL		(LED_LAMDA1 | LED_LAMDA2 | LED_LAMDA3 | LED_LAMDA4)


#define DIG_SIGNAL   1
#define DIG_1		 3
#define DIG_10		 2

typedef enum  
{
    Disp_Static = 0,
    Disp_DynamicDEC, 
    Disp_DynamicLED
}eSparkleType;


/// Basic APIs
eBOOL 	InitScreen( );
void    DisplayDEC( tINT8 num, tUINT8 bUpdate );
void    DisplayFloat( float num, tUINT8 bUpdate );
void    DisplayFreq(tINT8 freqId, tUINT8 bUpdate );
void    DisplayLED( tUINT8 ledIdx, tUINT8 val, tUINT8 bUpdate );
void    LedOn( tUINT8 ledIndex, tUINT8 bUpdate );
void    LedOff( tUINT8 ledIndex, tUINT8 bUpdate );
void    Update( tUINT8 bForced );
tUINT8  IsLedOn( tUINT8 led );


/// Application APIs
void	TriggerDisplaySparkle();
void 	SetDisplayType( eSparkleType eDisplayType );	///< make the DEC led sparkle.

#endif  // end of DISPLAY_H
