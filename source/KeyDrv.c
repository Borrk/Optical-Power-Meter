/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    DisplayDrv.c
*   Author:
*   Description:   Key driver. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/27/2012
*
******************************************************************************/

#include "C8051F350.h"
#include "KeyDrv.h"

tUINT8  idata sKeyMapTable = 0;


/*-----------------------------------------------------------------------------------*\
 *
 *  HardWare initilization section.
 *
 *  Used HW resource:
 *  - ADC0.
 *  -AIN0.0~0.3 ???
\*-----------------------------------------------------------------------------------*/

void vKeyDrvInitHW()
{
	/// P0.7
  	P0MDIN  |= 0x80;
  	P0MDOUT |= 0x80; // Pushpull 
	P0SKIP  |= 0x80;
  
  	/// P1.6 P1.7
  	P1MDIN  |= 0xc0;
  	P1MDOUT |= 0xc0; // Pushpull 
	P1SKIP  |= 0xc0;
}

/// Get current HW key state
void vKeySyncWithHW()
{
	/// do nothing  now. the sync will be done in main console.
}

void vInitKeyDrv( )
{
	vResetKeyDrvParam();
}


/*-----------------------------------------------------------------------------------*\
 *
 *  API section.
 *
\*-----------------------------------------------------------------------------------*/
void vResetKeyDrvParam()
{
	sKeyMapTable = 0;
}

eKeyState vGetKeyStatus( eKeyCode key )
{
	return  (eKeyState)((sKeyMapTable >> key) &0x01);
}

void  vSetKeyStatus( eKeyCode key, eKeyState state )
{
	if ( state == KEY_Pressed )
		sKeyMapTable &= ~(1 << key);
	else	 
		 sKeyMapTable |=  (1 << key);
}

tUINT8 KeyDrvOnSampling( tUINT32 argVal, tUINT8 flag  ) /// flag is the Key's code
{
	if ( flag < KEY_NoOfValues )
	{
		if ( argVal >= 0xffff00 )
		{
			vSetKeyStatus( flag, KEY_Pressed);
		}
		else
		{
			vSetKeyStatus( flag, KEY_Release);
		}
	}
	
	return 1;
}