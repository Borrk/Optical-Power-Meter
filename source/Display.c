/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Display.c
*   Author:
*   Description:    Display API. 
*   
*   Author:  Huang Yu Qi
*   Date: 04/01/2012
*
******************************************************************************/

#ifndef DISPLAY_C
#define DISPLAY_C

#include <math.h>
#include <string.h>
#include <absacc.h>
#include <intrins.h>

#include "Hw.h"
#include "Display.h"
#include"Timer.h"

#define LED_OFF 0xFF
#define HAS_DIG_1    1
#define HAS_DIG_10   0
#define HAS_LED   	 1

/// Show dicimal
#define DISPLAY_DICIMAL  

typedef struct
{
    tINT8	myLED0;	///< LEDs for Freq, Direction, Battary...
    tINT8	myLED1;	///< LED for 十位 /符号位
    tINT8	myLED2;	///< LED for 个位 /十位
    tINT8	myLED3;	///< LED for  小数点后一位 /个位
    tINT8   myChanged;
    tINT8   myShowDot;
	tINT8   mySparkleState;
	eSparkleType	myDisplayType; ///< Sparkle or not
} stVirtualScreen;

stVirtualScreen xdata sVirtualScreen;

void vResetData()
{
    sVirtualScreen.myLED0 = LED_OFF;
    sVirtualScreen.myLED1 = CHAR_OFF;
    sVirtualScreen.myLED2 = CHAR_OFF;
    sVirtualScreen.myLED3 = CHAR_OFF;
    sVirtualScreen.myChanged = 0;
    sVirtualScreen.myShowDot = 0;
	sVirtualScreen.myDisplayType = Disp_Static;
	sVirtualScreen.mySparkleState = 0;
}

eBOOL InitScreen( )
{
	vInitDisplayDrv();
    vResetData();

	return TRUE;
}

void Update( tUINT8 bForced )
{
    if ( (0 == bForced) &(0 == sVirtualScreen.myChanged) )
    {
        return;
    }
    
    vDisp_Digit_Signal( sVirtualScreen.myLED3 );	///符号位
    vDisp_Digit_10( sVirtualScreen.myLED2, sVirtualScreen.myShowDot );   ///十位
    vDisp_Digit_1( sVirtualScreen.myLED1, 0 );   ///个位
    vDisp_LEDs( sVirtualScreen.myLED0 );    ///Leds
    
	/// After send all data, output display trigger signle.
	vDisp_Trigger_Update();

    sVirtualScreen.myChanged = 0;
}


void DisplayDEC(tINT8 num, tUINT8 bUpdate )
{
    tUINT8 data j,k;
    
    k = abs(num) % 10;
    j = abs(num) / 10;
    //if ( (k != sVirtualScreen.myLED2) || (j != sVirtualScreen.myLED1) )
    {
        sVirtualScreen.myLED1 = (num<0) ? CHAR_HYPHON : CHAR_OFF;
        sVirtualScreen.myLED2 = j;
        sVirtualScreen.myLED3 = k;
        sVirtualScreen.myChanged = 1;
    }

    if ( (1 == bUpdate) )
        Update( 1 );
}

void DisplayFloat(float num, tUINT8 bUpdate )
{
    tUINT8 data j,k;
    float val = fabs(num);

    sVirtualScreen.myShowDot = 1;

    k = ((tUINT8)val) % 10;
    j = ((tUINT8)val) / 10;

    {
        sVirtualScreen.myLED1 = j;
        sVirtualScreen.myLED2 = k;
        sVirtualScreen.myLED3 = (tUINT8)((val-((tUINT8)val))*10);
		if ( num > 0 )
			LedOff( LED_SIGNAL, 0 );
		else
			LedOn( LED_SIGNAL, 1 );

        sVirtualScreen.myChanged = 1;
    }

    if ( (1 == bUpdate) )
        Update( 1 );
}


void DisplayFreq(tINT8 freqId, tUINT8 bUpdate )
{    
	LedOff( LED_SIGNAL, 0 );
    sVirtualScreen.myShowDot = 0;

    switch ( freqId )
	{
	case 1: /// 270
        sVirtualScreen.myLED1 = 2;
        sVirtualScreen.myLED2 = 7;
        sVirtualScreen.myLED3 = 0;
		break;

	case 2: /// 330
        sVirtualScreen.myLED1 = 3;
        sVirtualScreen.myLED2 = 3;
        sVirtualScreen.myLED3 = 0;
		break;

	case 3: /// 1000
        sVirtualScreen.myLED1 = CHAR_OFF;
        sVirtualScreen.myLED2 = 1;
        sVirtualScreen.myLED3 = CHAR_HI;
		break;

	case 4:
        sVirtualScreen.myLED1 = CHAR_OFF;
        sVirtualScreen.myLED2 = 2;
        sVirtualScreen.myLED3 = CHAR_HI;
		break;

	default:
        sVirtualScreen.myLED1 = 0; //CHAR_OFF;
        sVirtualScreen.myLED2 = 0; //CHAR_OFF;
        sVirtualScreen.myLED3 = 0; //CHAR_OFF;
	}

    if ( (1 == bUpdate) )
        Update( 1 );
}

#endif

void DisplayLED( tUINT8 ledIdx, tUINT8 val, tUINT8 bUpdate )
{
    if ( 0 == ledIdx )
    {
        sVirtualScreen.myLED0 = val;
        sVirtualScreen.myChanged = 1;
    }
    else if ( 1 == ledIdx )
    {
        sVirtualScreen.myLED1 = val;
        sVirtualScreen.myChanged = 1;
    }
    else if ( 2 == ledIdx )
    {
        sVirtualScreen.myLED2 = val;
        sVirtualScreen.myChanged = 1;
    }
    else if ( 3 == ledIdx )
    {
        sVirtualScreen.myLED3 = val;
        sVirtualScreen.myChanged = 1;
    }
  
    if ( (1 == bUpdate)/* || (1 == sVirtualScreen.myChanged)*/ )
        Update( 1 );
}

void LedOff( tUINT8 ledIndex, tUINT8 bUpdate )
{
    //if ( ledIndex != IsLedOn(ledIndex) )
    {
        sVirtualScreen.myLED0 = sVirtualScreen.myLED0 & (~ledIndex);
        sVirtualScreen.myChanged = 1;
    }
 
    if ( (1 == bUpdate) /* || (1 == sVirtualScreen.myChanged)*/ )
        Update( 1 );
}

void LedOn( tUINT8 ledIndex, tUINT8 bUpdate )
{
    //if ( ledIndex != IsLedOn(ledIndex) )
    {
        sVirtualScreen.myLED0 = sVirtualScreen.myLED0 | ledIndex;
        sVirtualScreen.myChanged = 1;
    }    
 
    if ( (1 == bUpdate) /*|| (1 == sVirtualScreen.myChanged)*/ )
        Update( 1 );
}

//tUINT8 IsLedOn( tUINT8 ledIndex )
//{
//    return (sVirtualScreen.myLED0 & ledIndex);
//}


void vUpdate( tUINT8 bOn )
{
	if ( bOn )
	{
	    vDisp_Digit_Signal( sVirtualScreen.myLED3 );	///符号位
    vDisp_Digit_10( sVirtualScreen.myLED2, sVirtualScreen.myShowDot );   ///十位
	    vDisp_Digit_1( sVirtualScreen.myLED1, 0 );   ///个位
	    vDisp_LEDs( sVirtualScreen.myLED0 );    ///Leds
	}
	else
	{
		if ( sVirtualScreen.myDisplayType == Disp_DynamicDEC )
		{
		    vDisp_Digit_Signal( CHAR_OFF );	///符号位
		    vDisp_Digit_10( CHAR_OFF, 0 );   ///十位
		    vDisp_Digit_1( CHAR_OFF, 0 );   ///个位
		    vDisp_LEDs( sVirtualScreen.myLED0 );	///Leds
		}
		else if ( sVirtualScreen.myDisplayType == Disp_DynamicLED )
		{
		    vDisp_Digit_Signal( sVirtualScreen.myLED3 );	///符号位
    		vDisp_Digit_10( sVirtualScreen.myLED2, sVirtualScreen.myShowDot );   ///十位
		    vDisp_Digit_1( sVirtualScreen.myLED1, 0 );   ///个位
		    vDisp_LEDs( LED_OFF );	///Leds
		}
	}

	/// After send all data, output display trigger signle.
	vDisp_Trigger_Update();
}

void 	SetDisplayType( eSparkleType eDisplayType )
{
	if ( sVirtualScreen.myDisplayType != eDisplayType )
	{
		sVirtualScreen.myDisplayType = eDisplayType;
		//sVirtualScreen.mySparkleState = 0;
	}
}

void	TriggerDisplaySparkle()
{
	vUpdate( sVirtualScreen.mySparkleState );
	sVirtualScreen.mySparkleState = (sVirtualScreen.mySparkleState == 0 ) ? 1 : 0;
}

#endif  ///< end of DISPLAY_C
