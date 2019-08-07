/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    KeyDrv.h
*   Author:
*   Description:    Key driver API declaration. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/27/2012
*
******************************************************************************/

#ifndef  KEYDRV_H
#define  KEYDRV_H

#include "Typedefs.h"
#include "C8051F350.h"


/// Key definition
typedef enum {
	KEY_START = 0,
	KEY_1 = KEY_START,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_NoOfValues
}eKeyCode;

/// Functional Key definition
typedef enum {
	FUNC_KEY_START = 0,
	FUNC_KEY_POWER = FUNC_KEY_START,
	FUNC_KEY_UNIT,
	FUNC_KEY_REF,
	FUNC_KEY_NoOfValues
}eFuncKeyCode;

typedef enum
{
	KEY_Release,
	KEY_Pressed
}eKeyState;

/// Functional key IO
sbit KEY_F_POWER = P0^7;
sbit KEY_F_UNIT  = P1^7;
sbit KEY_F_LAMDA = P1^6;
sbit KEY_F_POWER_EN = P0^6;

/// Basic APIs
void vKeyDrvInitHW();

/// Application APIs
void vInitKeyDrv( );

void vResetKeyDrvParam();

void  vSetKeyStatus( eKeyCode key, eKeyState state );
eKeyState vGetKeyStatus( eKeyCode key );
tUINT8 KeyDrvOnSampling( tUINT32 argVal, tUINT8 flag  );

#endif  // end of KEYDRV_H